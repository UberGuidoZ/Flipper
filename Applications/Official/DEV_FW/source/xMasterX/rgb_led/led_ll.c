#include "led_ll.h"
#include <core/check.h>

#include <furi_hal_interrupt.h>
#include <furi_hal_resources.h>
#include <furi_hal_gpio.h>
#include <furi.h>

#include <stdint.h>
#include <stm32wbxx_ll_tim.h>
#include <stm32wbxx_ll_dma.h>

#define TAG "LED_LL"

#define LED_BPP 24
#define ARR_TX 59

#define CCR_US_PERIOD 125
#define CCR_US_0 40
#define CCR_US_1 80
#define CCR_0 (ARR_TX * CCR_US_0 / CCR_US_PERIOD)
#define CCR_1 (ARR_TX * CCR_US_1 / CCR_US_PERIOD)
#define CCR_RESET 0
#define CCR_BUFFER_LEDS_HALF 8
#define CCR_BUFFER_LEDS (CCR_BUFFER_LEDS_HALF * 2)
#define CCR_BUFFER_LEN (CCR_BUFFER_LEDS * LED_BPP)


static uint32_t rgb_buf[LED_COUNT];
static uint32_t *rgb = rgb_buf;

static uint32_t rgb_led_buf[LED_COUNT];
static uint32_t *rgb_led = rgb_led_buf;

static uint32_t rgb_offset = 0;
static uint8_t ccr_buffer[CCR_BUFFER_LEN];

static uint8_t brightness_i = 3;
static uint8_t is_transmitting = 0;

static uint8_t apply_brightness(uint8_t v);
static uint8_t apply_gamma(uint8_t v);
static void fill_ccr_buffer(uint32_t ccr_led_offset, uint32_t ccr_led_count);

const GpioPin gpio_led = {.port = GPIOA, .pin = LL_GPIO_PIN_4};

static void handle_dma()
{
  FURI_LOG_D("LED_LL", "HandleDMA Called");
  furi_crash("DMA ISR");

   if(LL_DMA_IsActiveFlag_TE1(DMA1)) {
      LL_DMA_ClearFlag_TE1(DMA1);
      furi_crash("LED DMA Transfer did not work");
  }
  else if (LL_DMA_IsActiveFlag_TC1(DMA1))
  {
    if (rgb_offset > LED_COUNT)
    {
      LL_TIM_DisableCounter(TIM1);
      is_transmitting = 0;
    }

    fill_ccr_buffer(CCR_BUFFER_LEDS_HALF, CCR_BUFFER_LEDS_HALF);
    rgb_offset += CCR_BUFFER_LEDS;

    LL_DMA_ClearFlag_TC1(DMA1);
  }
  else if (LL_DMA_IsActiveFlag_HT1(DMA1))
  {
    fill_ccr_buffer(0, CCR_BUFFER_LEDS_HALF);
    LL_DMA_ClearFlag_HT1(DMA1);
  }
}

void led_init()
{
  FURI_LOG_D(TAG, "Initializing");

  furi_hal_gpio_init_ex(&gpio_led, GpioModeAltFunctionPushPull, GpioPullNo, GpioSpeedLow, GpioAltFn1TIM1);
  furi_hal_interrupt_set_isr_ex(FuriHalInterruptIdDma1Ch1, 4, handle_dma, NULL);

  FURI_CRITICAL_ENTER();
  LL_TIM_DeInit(TIM1);
  FURI_CRITICAL_EXIT();

  LL_TIM_InitTypeDef tim_init = {0};
  LL_TIM_OC_InitTypeDef tim_oc_init = {0};

  tim_init.Prescaler = 0;
  tim_init.CounterMode = LL_TIM_COUNTERMODE_UP;
  tim_init.Autoreload = ARR_TX;
  tim_init.RepetitionCounter = 0;

  LL_TIM_Init(TIM1, &tim_init);
  LL_TIM_EnableAllOutputs(TIM1);
  LL_TIM_EnableDMAReq_CC3(TIM1);
  LL_TIM_EnableIT_CC3(TIM1);

  tim_oc_init.OCMode = LL_TIM_OCMODE_PWM1;
  tim_oc_init.OCState = LL_TIM_OCSTATE_ENABLE;
  tim_oc_init.CompareValue = 0;

  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH1, &tim_oc_init);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH1);

  LL_DMA_InitTypeDef dma_config = {0};
  dma_config.PeriphOrM2MSrcAddress = (uint32_t) & (TIM1->CCR3);
  dma_config.MemoryOrM2MDstAddress = (uint32_t)NULL;
  dma_config.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  dma_config.Mode = LL_DMA_MODE_NORMAL;
  dma_config.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
  dma_config.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
  dma_config.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
  dma_config.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
  dma_config.NbData = 0;
  dma_config.PeriphRequest = LL_DMAMUX_REQ_TIM1_UP;
  dma_config.Priority = LL_DMA_PRIORITY_VERYHIGH; //LL_DMA_PRIORITY_MEDIUM; // LL_DMA_PRIORITY_VERYHIGH
  LL_DMA_Init(DMA1, LL_DMA_CHANNEL_1, &dma_config);
  LL_DMA_ClearFlag_TE1(DMA1);
  LL_DMA_ClearFlag_TC1(DMA1);
  LL_DMA_ClearFlag_HT1(DMA1);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);
}

void led_deinit()
{
  FURI_CRITICAL_ENTER();
  LL_TIM_DeInit(TIM1);
  LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_1);
  furi_hal_interrupt_set_isr(FuriHalInterruptIdDma1Ch1, NULL, NULL);
  FURI_CRITICAL_EXIT();
}

int8_t led_update()
{
  FURI_LOG_D(TAG, "LED UPDATE");
  FURI_LOG_D(TAG, "LL_DMA_IsEnabledChannel = %lu", LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_1));

  if(is_transmitting) {
    // ONLY FOR DEBUGGING RIGHT NOW
    FURI_LOG_D(TAG, "Led Update but still transmitting");
    return -1;
  }
  /*while (is_transmitting)
  {
  }*/
  is_transmitting = 1;

  for (uint8_t i = 0; i < LED_COUNT; i++)
  {
    uint32_t v = rgb[i];
    uint8_t r = RGB_R(v);
    uint8_t g = RGB_G(v);
    uint8_t b = RGB_B(v);

    r = apply_gamma(r);
    r = apply_brightness(r);

    g = apply_gamma(g);
    g = apply_brightness(g);

    b = apply_gamma(b);
    b = apply_brightness(b);

    rgb_led[i] = RGB_UINT(r, g, b);
  }

  rgb_offset = 0;
  fill_ccr_buffer(0, CCR_BUFFER_LEDS);
  rgb_offset += CCR_BUFFER_LEDS;

  FURI_LOG_D(TAG, "Led Update with buffer %d %d:%d:%d:%d:%d:%d", CCR_BUFFER_LEN, 
  ccr_buffer[0], 
  ccr_buffer[1], 
  ccr_buffer[2], 
  ccr_buffer[3], 
  ccr_buffer[4], 
  ccr_buffer[5]
  );

  FURI_CRITICAL_ENTER();
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)ccr_buffer);
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, CCR_BUFFER_LEN);
  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
  LL_TIM_GenerateEvent_UPDATE(TIM1);
  LL_TIM_EnableCounter(TIM1);
  FURI_CRITICAL_EXIT();

  FURI_LOG_D(TAG, "Led Update. Timer is %lu", LL_TIM_CC_IsEnabledChannel(TIM1, LL_TIM_CHANNEL_CH1));

  return 0;
}

uint8_t led_get_brightness() { return brightness_i; }

uint32_t led_get(uint16_t i) { return rgb[i]; }

void led_set(uint16_t i, uint32_t v) { rgb[i] = v; }

void led_set_rgb(uint16_t i, uint32_t r, uint32_t g, uint32_t b)
{
  if (i >= LED_COUNT)
  {
    return;
  }

  if (r > 255)
    r = 255;
  if (g > 255)
    g = 255;
  if (b > 255)
    b = 255;

  led_set(i, RGB_UINT(r, g, b));
}

void led_set_rgbf(uint16_t i, float r, float g, float b)
{
  if (r < 0)
    r = 0;
  if (g < 0)
    g = 0;
  if (b < 0)
    b = 0;

  if (r > 1)
    r = 1;
  if (g > 1)
    g = 1;
  if (b > 1)
    b = 1;

  led_set_rgb(i, r * 255, g * 255, b * 255);
}

void led_clear()
{
  for (uint32_t i = 0; i < LED_COUNT; i++)
  {
    led_set(i, 0);
  }
}

void led_set_brightness(uint8_t brightness)
{
  if (brightness > LED_BRIGHTNESS_MAX - 1)
    brightness = LED_BRIGHTNESS_MAX - 1;

  brightness_i = brightness;
}

static uint8_t apply_brightness(uint8_t v)
{
  static uint8_t brightness_table[LED_BRIGHTNESS_MAX] = {50, 25, 12};

  return (uint16_t)v *
         brightness_table[(LED_BRIGHTNESS_MAX - 1) - brightness_i] / 255;
}

static uint8_t apply_gamma(uint8_t v)
{
  static uint8_t gamma_table[256] = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2,
      2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5,
      5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
      10, 10, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
      17, 17, 18, 18, 19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 25,
      25, 26, 27, 27, 28, 29, 29, 30, 31, 31, 32, 33, 33, 34, 35,
      36, 36, 37, 38, 39, 40, 40, 41, 42, 43, 44, 45, 45, 46, 47,
      48, 49, 50, 51, 52, 53, 54, 55, 55, 56, 57, 58, 59, 60, 61,
      62, 63, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 77, 78,
      79, 80, 81, 82, 84, 85, 86, 87, 88, 90, 91, 92, 93, 95, 96,
      97, 99, 100, 101, 103, 104, 105, 107, 108, 109, 111, 112, 114, 115, 117,
      118, 119, 121, 122, 124, 125, 127, 128, 130, 131, 133, 135, 136, 138, 139,
      141, 142, 144, 146, 147, 149, 151, 152, 154, 156, 157, 159, 161, 162, 164,
      166, 168, 169, 171, 173, 175, 176, 178, 180, 182, 184, 186, 187, 189, 191,
      193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221,
      223, 225, 227, 229, 231, 233, 235, 237, 239, 241, 244, 246, 248, 250, 252,
      255};

  return gamma_table[v];
}

static void fill_ccr_buffer(uint32_t ccr_led_offset, uint32_t ccr_led_count)
{
  for (uint32_t ccr_led_i = 0; ccr_led_i < ccr_led_count; ccr_led_i++)
  {
    uint8_t rgb_i = rgb_offset + ccr_led_offset + ccr_led_i;
    uint32_t rgb_value = rgb_led[rgb_i];

    uint16_t buff_offset = (ccr_led_offset + ccr_led_i) * LED_BPP;
    for (uint8_t bit_i = 0; bit_i < 24; bit_i++)
    {
      uint16_t buff_i = buff_offset + bit_i;

      if (rgb_i >= LED_COUNT)
      {
        ccr_buffer[buff_i] = CCR_RESET;
      }
      else
      {
        uint8_t rgb_bit_offset = 23 - bit_i;
        uint8_t rgb_bit = (rgb_value >> rgb_bit_offset) & 1;

        ccr_buffer[buff_i] = rgb_bit ? CCR_1 : CCR_0;
      }
    }
  }
}


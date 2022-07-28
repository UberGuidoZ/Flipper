https://github.com/flipperdevices/flipperzero-firmware/tree/dev/firmware/targets/f7/furi_hal/furi_hal_subghz.c

Change `furi_hal_subghz_is_frequency_valid` and `furi_hal_subghz_set_frequency_and_path`

Reminder, the unofficially supported freq are 281-361 MHz, 378-481 MHz, and 749-962 MHz

281000000 - 361000000<br>
378000000 - 481000000<br>
749000000 - 962000000

```
bool furi_hal_subghz_is_frequency_valid(uint32_t value) {
    if(!(value >= 281000000 && value <= 361000000) &&
       !(value >= 378000000 && value <= 481000000) &&
       !(value >= 749000000 && value <= 962000000)) {
        return false;

uint32_t furi_hal_subghz_set_frequency_and_path(uint32_t value) {
    value = furi_hal_subghz_set_frequency(value);
    if(value >= 281000000 && value <= 361000000) {
        furi_hal_subghz_set_path(FuriHalSubGhzPath315);
    } else if(value >= 378000000 && value <= 481000000) {
        furi_hal_subghz_set_path(FuriHalSubGhzPath433);
    } else if(value >= 749000000 && value <= 962000000) {
        furi_hal_subghz_set_path(FuriHalSubGhzPath868);
    } else {
        furi_crash("SubGhz: Incorrect frequency during set.");
    }
    return value;
```

Don't forget to add your new freq to the [user settings](https://github.com/UberGuidoZ/Flipper/tree/main/Sub-GHz/Settings) file.

# GPIO Logic Analyzer v2.01 for Flipper Zero

By: UberGuidoZ | https://github.com/UberGuidoZ/Flipper

Code: Applications/My_Creations/gpio_logic_analyzer

A logic analyzer that monitors all 8 exposed GPIO pins on the Flipper Zero
18-pin header, draws live scrolling waveforms on the 128x64 OLED, and logs
every sample to a timestamped CSV file on the SD card, with instructions.

---

## Quick start

1. Build and side-load (see Building section below) or grab from App Store.
2. On the Flipper, navigate to **Apps > GPIO > GPIO Logic Analyzer**.
3. Select **Instructions** to review the voltage limits and control map.
4. Select **Start Analyzer** to begin capture.
5. CSV logs are written automatically to `/ext/gpio_analyzer/` while capturing.

---

## Main Menu

| Item              | Action                                  |
|-------------------|-----------------------------------------|
| Start Analyzer    | Opens the live capture screen           |
| Instructions      | Four-page on-screen reference guide     |
| About             | Version, author, and project URL        |

---

## Analyzer Screen

```
GPIO Analyzer         RUN 100Hz*
---------------------------------
A7 [~~~~|____|~~~~]         H
A6 [____|~~~~|____]         L
A4 [________________]       L
B3 [~~~~~~~~~~~~~~~~]       H
B2 [________________]       L
C3 [~~|___|~~~|____]        H
C1 [__|~~~|______]          L
C0 [~~~|___|~~]             H
```

- Each row is one GPIO pin.
- The waveform shows the last 104 samples, scrolling left as new samples arrive.
- The newest sample is always the rightmost pixel column.
- `H` / `L` at the right edge shows the instantaneous pin state.
- Vertical lines in the waveform mark state transitions (rising/falling edges).
- `*` after the sample rate in the header means CSV logging is active.

---

## Controls

| Button      | Action                          |
|-------------|---------------------------------|
| OK (short)  | Toggle capture on / off         |
| OK (long)   | Clear waveform history          |
| UP          | Increase sample rate            |
| DOWN        | Decrease sample rate            |
| BACK        | Stop capture, close CSV, exit   |

---

## Monitored Pins

| Row | Label | Pin Name | Header Pin | Notes         |
|-----|-------|----------|------------|---------------|
|  1  | A7    | PA7      |  Pin  2    | 3.3V max      |
|  2  | A6    | PA6      |  Pin  3    | 3.3V max      |
|  3  | A4    | PA4      |  Pin  4    | 5V tolerant   |
|  4  | B3    | PB3      |  Pin  5    | 3.3V max      |
|  5  | B2    | PB2      |  Pin  6    | 3.3V max      |
|  6  | C3    | PC3      |  Pin  7    | 3.3V max      |
|  7  | C1    | PC1      |  Pin 15    | 3.3V max      |
|  8  | C0    | PC0      |  Pin 16    | 3.3V max      |

All pins are configured as digital inputs with no internal pull resistor.
They are returned to high-impedance analog mode when you exit the analyzer.

**Voltage warning**: Do not connect signals above 3.3V to any pin except PA4.
No pull-up or pull-down resistors are enabled. Add external resistors to
your circuit if the signals you are probing have undefined idle states.

FAILURE TO FOLLOW THIS WARNING COULD CAUSE FLIPPER TO LEAK MAGIC SMOKE!

---

## Sample Rates

| Rate   | Period | Notes                                          |
|--------|--------|------------------------------------------------|
|  10 Hz | 100 ms | Safest for SD write overhead at any card speed |
|  50 Hz |  20 ms | Good general-purpose rate                      |
| 100 Hz |  10 ms | Default (balanced speed and reliability)       |
| 200 Hz |   5 ms | Fast; occasional sample jitter near SD writes  |
| 500 Hz |   2 ms | Best-effort; may drop samples during CSV flush |

Rates are FreeRTOS timer-based. The Flipper OS scheduler and SD write latency
can introduce jitter at high rates. For precision timing analysis, use a
dedicated logic analyzer. This tool is intended for qualitative signal
inspection and basic protocol debugging. YMMV, caveat emptor, all that.

---

## CSV Logging

A new CSV file is created automatically each time you start capture.

**Location on SD card:**
```
/ext/gpio_analyzer/gla_YYYYMMDD_HHMMSS.csv
```

Example: `/ext/gpio_analyzer/gla_20240315_143022.csv`

**File format (comma-separated, CRLF line endings):**
```
sample,A7,A6,A4,B3,B2,C3,C1,C0
0,1,0,1,0,0,1,0,1
1,1,0,1,0,0,1,0,0
2,0,0,1,0,0,1,1,0
...
```

- `sample` - monotonically incrementing sample number (starts at 0 each session).
- Pin columns - `1` = HIGH (3.3V), `0` = LOW (0V).
- Column order matches the on-screen row order (A7 top, C0 bottom).
- CRLF endings ensure correct display in Excel and LibreOffice Calc.

If the SD card is absent or the directory cannot be created, the app
continues without logging (no `*` in the header) and flashes the error LED.

---

## Building

Assumes repo is cloned to `gpio_logic_analyzer` and requires [ufbt](https://github.com/flipperdevices/flipperzero-ufbt):

```sh
pip install ufbt           # install ufbt once
cd gpio_logic_analyzer
ufbt                       # build only (outputs build/.../gpio_logic_analyzer.fap)
ufbt launch                # build and side-load over USB
```

To build inside the full firmware tree instead:

```sh
# Place gpio_logic_analyzer/ under applications_user/
./fbt fap_gpio_logic_analyzer
```

To add a custom icon, drop a 10x10 pixel PNG named `gpio_analyzer_icon.png`
into this directory and add `fap_icon="gpio_analyzer_icon.png"` to
`application.fam`.

---

## File Structure

```
gpio_logic_analyzer/
  application.fam          -- ufbt app manifest
  gpio_analyzer.h          -- shared types, constants, app struct
  gpio_analyzer_app.c      -- entry point, alloc/free, scene handler table
  scene_menu.c             -- main menu (Submenu widget)
  scene_analyzer.c         -- GPIO capture, waveform renderer, CSV logging
  scene_instructions.c     -- 4-page on-screen reference (LEFT/RIGHT to page)
  scene_about.c            -- about screen
  README.md                -- this file
```
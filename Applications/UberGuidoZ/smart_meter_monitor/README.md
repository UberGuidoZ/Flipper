# Smart Meter Monitor - Flipper Zero FAP  v1.11

By: UberGuidoZ | https://github.com/UberGuidoZ/Flipper

Code: [Applications/UberGuidoZ/smart_meter_monitor/Source](https://github.com/UberGuidoZ/Flipper/tree/main/Applications/UberGuidoZ/smart_meter_monitor/Source)

Monitors the 433 MHz, 868 MHz, and 915 MHz bands for smart meter
radio transmissions using the Flipper Zero's built-in CC1101
sub-GHz radio. Displays live RSSI as a scrolling waveform,
detects signal bursts, and tracks transmission intervals.

---

## What It Does

Most North American electric and gas meters use the ERT (Encoder
Receiver Transmitter) protocol at 915 MHz, broadcasting
unencrypted SCM (Standard Consumption Message) packets every
30-90 seconds. European meters typically use 868 MHz. Some water
meters use 433 MHz.

This app monitors RSSI (signal strength) in real time. When a
meter transmits, a spike appears in the waveform and is counted
as a detection event. The app tracks how often transmissions
occur, giving you the broadcast interval of meters in range.

No packet decoding is performed - this is energy detection only.

---

## Display

```
Meter Monitor     915MHz 500ms
[RSSI waveform - 128 samples, newest on right]
[dashed threshold line across waveform]
-90dBm Cnt:003
Int: 47s Avg: 52s
```

- Waveform bars show RSSI from -120 dBm (bottom) to -40 dBm (top)
- Dashed line marks the burst detection threshold
- Cnt: total detected transmissions since last reset
- Int: interval between last two detections (seconds)
- Avg: exponential moving average of all intervals

When fewer than 2 bursts have been detected, line 2 shows the
current threshold and "---" for interval.

---

## Controls

| Button      | Action                                 |
|-------------|----------------------------------------|
| OK (short)  | Pause / resume capture (II indicator)  |
| OK (long)   | Reset stats and waveform history       |
| UP          | Raise threshold (less sensitive)       |
| DOWN        | Lower threshold (more sensitive)       |
| LEFT        | Previous frequency (cycle 433/868/915) |
| RIGHT       | Next frequency                         |
| BACK        | Stop radio, return to menu             |

---

## Frequencies

| Frequency  | Region         | Meter type           |
|------------|----------------|----------------------|
| 433.92 MHz | Worldwide      | Some water meters    |
| 868.30 MHz | Europe         | Electric / gas       |
| 915.00 MHz | North America  | Electric / gas (ERT) |

Change the frequency on the fly with LEFT/RIGHT while monitoring.
NOTE: Changing frequency resets stats and restarts the radio.

---

## Sample Rates and History Window

| Rate   | History at 128 samples |
|--------|------------------------|
| 100 ms |  13 seconds            |
| 250 ms |  32 seconds            |
| 500 ms |  64 seconds (default)  |
| 1 s    | 128 seconds (~2 min)   |
| 2 s    | 256 seconds (~4 min)   |

UP/DOWN cycles the sample rate while monitoring.

---

## Threshold Adjustment

The dashed line on the waveform marks the burst detection
threshold. Adjust with UP/DOWN in 5 dBm steps (-110 to -60 dBm).

Default: -90 dBm. Start here and watch the ambient RSSI noise
floor. Raise the threshold until it sits clearly above the noise
floor but below the meter transmission spikes.

Typical noise floor: -100 to -105 dBm
Typical meter transmission RSSI: -60 to -90 dBm (distance-dependent)

---

## Building

```sh
pip install --upgrade ufbt
ufbt update
cd smart_meter_monitor
ufbt launch
```

Output: `dist/smart_meter_monitor.fap`
Install to: `/ext/apps/Sub-GHz/smart_meter_monitor.fap`

---

## File Structure

```
smart_meter_monitor/
  application.fam        - ufbt app manifest
  smart_meter.h          - shared types, constants, app struct
  smart_meter_app.c      - entry point, alloc/free, scene handler table
  scene_splash.c         - splash screen (3s auto-advance)
  scene_menu.c           - main menu
  scene_freq.c           - frequency selector submenu
  scene_monitor.c        - CC1101 capture, waveform, burst detection
  scene_instructions.c   - 4-page on-screen reference
  scene_about.c          - about screen
  README.md              - this file
```

---

## Notes

- The app uses the CC1101 HAL directly. Do not launch while the
  built-in Sub-GHz app is actively scanning.
- Burst detection uses hysteresis: 2 consecutive samples above
  threshold to enter, 3 consecutive samples below to exit. This
  prevents spurious counts from momentary noise spikes.
- The ERT protocol broadcasts every 30-90 seconds depending on
  meter model and utility configuration. Allow 2-3 minutes of
  monitoring before drawing conclusions from interval data.
# PWM GravityFalls Player

Embedded STM32 project (STM32L476RG) generated from STM32CubeMX and extended with custom application logic:
- LED animation controller
- software scheduler for deferred callbacks
- PWM-based melody playback on TIM2
- button event processing with click-pattern actions

## Overview

The firmware runs a lightweight event loop in main:
1. collect button edge events in EXTI interrupt
2. consume events in the main loop
3. schedule and execute deferred actions through a callback scheduler

This structure keeps ISR work short and moves higher-level logic to non-interrupt context.

## Hardware Assumptions

- Board class: Nucleo-L476RG (or compatible STM32L476)
- User button: `B1_Pin` (external interrupt enabled)
- LED: `LD2_Pin` (PA5)
- PWM output: `TIM2`, channel 1

## Project Functionality

### 1) Button Handling and Gesture Logic

Implemented in `Core/Src/main.c`:
- EXTI callback (`HAL_GPIO_EXTI_Callback`) pushes button state + timestamp into a ring buffer
- `process_button_events()` consumes buffered edges in the main loop
- press duration is used to classify interaction:

Rules:
- long press (`duration > 500 ms`):
  -blink once
- short press (`50 ms < duration <= 500 ms`):
  - once click
  - bouble click
  - triple click

Click evaluation callback (`evaluate_clicks_cb`):
- 1 click: toggle LED
- 2 clicks: next LED animation + start next song
- 3+ clicks: previous LED animation + start next song

Overflow behavior:
- if the ISR ring buffer is full, LED animation is forced to `LED_BLINK` as a visible fault indicator.

### 2) LED Animation Controller

Implemented in `Core/Src/led/led_controller.c`, API in `Core/Inc/led/led_controller.h`.

Supported modes (`LED_Animation_t`):
- `LED_OFF`
- `LED_ON`
- `LED_BLINK`
- `LED_SHORT_PULSE`
- `LED_LONG_PULSE`

Behavior:
- direct register access is used for fast LED operations on PA5
- animation state transitions are scheduled using `ScheduleController_Schedule(...)`
- repeating patterns are implemented as self-rescheduling callbacks

Timing profiles:
- Blink: active blinking window with 50 ms toggles, then pause, then restart
- Short pulse: 300 ms ON / 700 ms OFF
- Long pulse: 500 ms ON / 500 ms OFF

### 3) Software Scheduler

Implemented in `Core/Src/schedule_controller.c`, API in `Core/Inc/schedule_controller.h`.

Capabilities:
- schedule callback after N milliseconds
- cancel all pending entries for a callback
- process due callbacks in main loop
- ISR-safe enqueue variant (`ScheduleController_ScheduleFromISR`)

### 4) PWM Tone and Song Playback

Implemented in `Core/Src/controller/pwm_controller.c`, API/data in `Core/Inc/controller/pwm_controller.h`.

Features:
- TIM2 CH1 PWM start/stop control
- note playback by setting prescaler from desired frequency
- fixed PWM period (`ARR`) and low duty cycle for square-wave style buzzer output
- song sequencer driven by scheduler callbacks

Song flow:
- `PWM_Controller_Next_Song()` stops current song if active, then starts predefined melody
- each note schedules:
  - note-off callback after note duration
  - next-note callback after same duration
- rest notes are represented by frequency `0`

Current song data:
- predefined `song_gravity_falls[]` note sequence
- note frequency constants from C0..B8 are declared in the header

## Runtime Flow

Main loop (`while (1)`):
- `process_button_events();`
- `ScheduleController_Process();`


## Code Structure

- `Core/Src/main.c`: initialization, event queue, button logic, main loop
- `Core/Src/schedule_controller.c`: callback scheduler
- `Core/Src/led/led_controller.c`: LED mode logic
- `Core/Src/controller/pwm_controller.c`: buzzer/PWM and song sequencing
- `Drivers/`: STM32 HAL and CMSIS
- `cmake/stm32cubemx/`: CubeMX-generated CMake integration

## Build

This repository is CMake-based and uses ARM GCC toolchain file `cmake/gcc-arm-none-eabi.cmake`.

### Configure + Build (Debug)

```bash
cmake --preset Debug
cmake --build --preset Debug
```

Expected output directory:
- `build/Debug/`

### Configure + Build (Release)

```bash
cmake --preset Release
cmake --build --preset Release
```

## Flash / Debug

The workspace includes VS Code launch configuration targeting STM32Cube ST-Link debug integration (`.vscode/launch.json`).

Typical workflow:
1. Build project (`Debug` preset)
2. Start ST-Link debug launch from VS Code
3. Run from `main`

If your environment does not provide STM32Cube-specific debug command integration, use your standard ST-Link + GDB/OpenOCD flow with the generated ELF.

## Notes

- Scheduler currently uses dynamic allocation (`malloc/realloc`). 
- ISR queue in `main.c` is intentionally small (`EVENT_QUEUE_SIZE = 10`).
- `Core/Inc/controller/pwm_controller.h` contains large musical constant tables and song data by design for single-file portability.

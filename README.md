# Body Control Module (BCM) in Automotives
  A CAN-bus based Body Control Module simulation implemented on the LPC2129 "(ARM7TDMI-S)" microcontroller. The system emulates a real automotive BCM — a central ECU that manages body electronics like turn indicators and wiper motors by communicating with distributed slave nodes over the "CAN protocol".

## Overview

In real vehicles, the Body Control Module is the ECU responsible for coordinating body-related functions — lighting, wipers, indicators, door locks, etc. — often by talking to other ECUs over CAN rather than driving hardware directly.

This project recreates that architecture on a small scale using **three LPC2129 nodes**:

| Node | Role |
|---|---|
| **Main BCM Node** (`main.c`) | Reads driver switch inputs, displays status on a 16x2 LCD, transmits CAN commands, and drives the wiper motor locally via PWM |
| **Left Node** (`left.c`) | Receives CAN messages and blinks the left turn-indicator LEDs |
| **Right Node** (`right.c`) | Receives CAN messages and blinks the right turn-indicator LEDs |

## Features

- **CAN-based communication** between master (BCM) and two slave nodes, avoiding direct wiring for every function
- **16x2 LCD status display** showing indicator and wiper state in real time
- **Left/Right turn indicator control** — switch press on the main node triggers a CAN message; the corresponding slave node blinks its LED bank sequentially
- **PWM-based wiper motor control** with speed ramping (via LPC2129 PWM5 channel) and a switch-triggered stop condition
- **Modular driver headers** for LCD and motor control, reusable across nodes

## Hardware Used

- LPC2129 ARM7TDMI-S microcontroller boards (x3 — one main + two slave nodes)
- CAN transceiver module (e.g., TJA1050 / MCP2551) per node
- 16x2 character LCD (8-bit interface)
- Push buttons (SW1: Left indicator, SW2: Right indicator, SW3: Wiper)
- LED banks (8 LEDs per node) for indicator simulation
- DC/servo motor or PWM-driven actuator for wiper simulation

## System Architecture

```
                     ┌────────────────────┐
                     │   Main BCM Node     │
                     │  (main.c)           │
                     │  - LCD display      │
                     │  - Switch inputs    │
                     │  - PWM wiper driver │
                     └─────────┬───────────┘
                               │  CAN Bus
                 ┌─────────────┴─────────────┐
                 │                           │
        ┌────────▼────────┐        ┌────────▼─────────┐
        │   Left Node      │        │   Right Node      │
        │   (left.c)       │        │   (right.c)       │
        │  byteA == 0x01   │        │  byteA == 0x02    │
        │  → blink LEDs    │        │  → blink LEDs     │
        └──────────────────┘        └───────────────────┘
```

## How It Works

1. **Indicator Control**: When SW1 (or SW2) is pressed on the main node, it updates the LCD, builds a CAN frame with `byteA = 0x01` (left) or `0x02` (right), and transmits it (`can_tx`). The corresponding slave node's `can_rx` unblocks, matches the byte value, and blinks its LED sequence with a 400 ms delay per step.
2. **Wiper Control**: When SW3 is pressed, the LCD updates and `driver()` runs — a PWM routine (PWM5 channel, 20 ms period) that ramps the motor's pulse width upward in steps, resetting once it hits an upper bound, simulating variable-speed wiper sweep. The loop exits when the wiper switch is released.
3. **LCD Driver**: `lcdheader.h` implements a simple 8-bit parallel LCD interface (`INIT`, `CMD`, `DATA`, `STR`, `INT`) used to print status strings and numbers.

## File Structure

| File | Description |
|---|---|
| `main.c` | Main BCM node logic — switches, LCD status, CAN transmission, wiper trigger |
| `left.c` | Left indicator slave node — CAN receive and LED blink |
| `right.c` | Right indicator slave node — CAN receive and LED blink |
| `lcd.c` | Standalone LCD driver test |
| `lcdheader.h` | 16x2 LCD interfacing driver (init, command, data, string, integer print) |
| `motor_driver.h` | PWM initialization and wiper motor speed-ramp control logic |

> Note: `candriver.h` (CAN init/tx/rx and `CAN_MSG` struct) is referenced by these files but not included here — add it to the repo if you have it, so the project builds standalone.

## Tools Used

- Keil µVision (ARM7 compiler/IDE)
- Flash Magic (for flashing LPC2129 over UART)

## Future Scope

- Add door lock/unlock and headlight control nodes over the same CAN bus
- Replace fixed LED banks with real indicator relay control
- Add CAN error handling and message acknowledgment
- Migrate to a CAN-FD capable controller for higher data rates

## Author

Rajalakshmi.s

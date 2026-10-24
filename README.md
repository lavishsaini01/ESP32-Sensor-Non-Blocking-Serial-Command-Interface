# ESP32 Sensor & Non-Blocking Serial Command Interface

An embedded firmware project built for the ESP32 using the Arduino framework. It integrates environmental sensing (DHT11), light detection (LDR), multi-sample analog voltage conversion (Potentiometer), and a robust, non-blocking serial command-line interface.

---

## Key Features

* **Non-Blocking Architecture:** Utilizes `millis()` for independent task scheduling, ensuring sensor polling and serial communication never block the main loop.
* **Environmental Monitoring:** Periodically reads temperature (°C / °F), relative humidity, and heat index using a DHT11 sensor.
* **Analog & Digital Inputs:**
* Reads digital light/dark status via an LDR sensor.
* Performs 10-sample oversampling and averaging on a potentiometer ADC input to calculate stable percentage and voltage values ($3.3\text{V}$ reference).


* **Non-Blocking Serial CLI:** Implements a custom character-by-character parser with a fixed buffer (`32` bytes) to prevent overflows and safely process user commands at runtime.
* **Dynamic Configuration:** Allows updating sensor reporting intervals on the fly via the Serial Monitor.

---

## Pinout Configuration

| Component | ESP32 GPIO Pin | Description |
| --- | --- | --- |
| **DHT11 Sensor** | `GPIO 35` | Data pin for temperature and humidity |
| **LDR Sensor** | `GPIO 12` | Digital input for light/dark detection |
| **Potentiometer** | `GPIO 4` | Analog input (ADC) for voltage and percentage calculation |

---

## Serial Command Reference

Open your Serial Monitor at **115200 baud** (with newline/carriage return enabled) to use the following commands:

| Command | Description |
| --- | --- |
| `help` | Displays the list of available commands. |
| `status` | Shows the current sensor reporting interval in milliseconds. |
| `set interval <ms>` | Updates the main sensor reporting interval (valid range: `100` to `10000` ms). |

---

## Getting Started

### Prerequisites

* [PlatformIO](https://platformio.org/) or [Arduino IDE](https://www.arduino.cc/) configured with the ESP32 board package.
* The `DHT sensor library` by Adafruit installed in your environment.
* An ESP32 development board, DHT11 module, LDR module/circuit, and a potentiometer wired to the specified GPIO pins.

### Compilation & Upload

1. Clone or copy this repository into your project workspace.
2. Ensure required libraries (`DHT.h`) are included in your build dependencies.
3. Connect your ESP32 board via USB, compile, and upload the firmware.

---

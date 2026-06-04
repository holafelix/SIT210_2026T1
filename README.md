# SIT210 — Embedded Systems Development

> Deakin University · Trimester 1, 2026 · Learning portfolio

![Board](https://img.shields.io/badge/Board-Arduino%20Nano%2033%20IoT-00979D)
![MCU](https://img.shields.io/badge/MCU-SAMD21-blue)
![Also](https://img.shields.io/badge/Also-Raspberry%20Pi-c51a4a)
![Language](https://img.shields.io/badge/Language-C%2B%2B%20%7C%20Python-orange)
![IDE](https://img.shields.io/badge/Built%20with-Arduino%20IDE-teal)

This repo is my weekly task portfolio for **SIT210 Embedded Systems Development**. Each task is a
small embedded project, and together they build up into one bigger idea: a **smart assisted-living
home** that helps an elderly resident by automating lights, sensing the environment, talking to the
cloud, and reacting to motion, gestures, and even voice.

The tasks roughly follow the unit's weekly topics — starting from basic GPIO and modular code, then
moving through serial/I2C, web services, MQTT, interrupts, web control, Raspberry Pi GUIs, and
finishing with speech recognition over Bluetooth.

## Hardware used across the tasks

- **Arduino Nano 33 IoT** (SAMD21, built-in WiFi + BLE) — main microcontroller
- **Raspberry Pi** — used for the GUI and voice-control tasks
- BH1750 ambient light sensor (I2C)
- HC-SR501 PIR motion sensor
- HC-SR04 ultrasonic distance sensor
- DHT11 temperature/humidity sensor
- LDR (photoresistor) for analogue light reading
- USB microphone (for voice control)
- SPDT slide switch, LEDs + 220Ω resistors, breadboard, jumper wires

## Tasks

| Task | Title | What it does |
|------|-------|--------------|
| **1.1P** | Switching ON Lights | Switch-triggered porch + hallway lights written with a **modular** approach (small helper functions instead of one big `loop()`). |
| **2.1P** | Temperature & Light to the Web | Reads a DHT11 and a light sensor, then pushes both to **ThingSpeak** over WiFi for an online dashboard. |
| **3.1P** | Sensor Trigger / Notification | A BH1750 watches the light level and fires an **IFTTT webhook** when it crosses a threshold (e.g. sunlight on/off). |
| **3.3C** | Network of Things (MQTT) | An ultrasonic sensor detects "wave" vs "pat" gestures and publishes them over **MQTT**; subscribers switch LEDs on/off. |
| **4.1P** | Interrupt-Based Lighting | Motion lighting driven entirely by **hardware interrupts** — PIR + BH1750 so lights only come on in the dark, with a slide switch as a manual backup. |
| **4.2D** | Web-Controlled Lights | The Nano runs a small **web server** that serves a page to toggle the living room, bathroom, and closet lights from a browser. |
| **5.1P** | Raspberry Pi GUI | A **tkinter** desktop GUI on the Pi using `gpiozero` to pick which room light is on. |
| **5.2** | Pi GUI with Dimming | Adds a brightness **PWM** slider for the living room plus checkboxes for the other lights. |
| **6.1P** | Project Pitch | The assisted-living node concept: interrupt motion lighting **+** an inactivity alert for a carer **+** cloud logging to ThingSpeak. |
| **8.1HD** | Voice Control | The Pi runs offline speech recognition (**Vosk**) and sends commands ("lights on/off", "fan on/off") to the Nano over **Bluetooth LE**. |

## Repo structure

Each task lives in its own folder with its sketch (`.ino`) or Python file inside.

```
SIT210_2026T1/
├── Task1.1P/                  # 1.1P  Switching ON Lights (.ino)
├── Task2.1WebHook/            # 2.1P  Temperature & Light to the Web (.ino)
├── Task3.1Trigger/            # 3.1P  Sensor trigger via IFTTT (.ino)
├── Task3.3CMQTT/              # 3.3C  MQTT gesture control (.ino)
├── Task4.1Interrupts/         # 4.1P  Interrupt-based lighting (.ino)
├── Task4.2DCloudFunction/     # 4.2D  Web-controlled lights (.ino)
├── Task5.1GUI/                # 5.1P  Raspberry Pi tkinter GUI (.py)
├── Task5.2GUI/                # 5.2   Pi GUI with PWM dimming (.py)
├── Task6.1ProjectPitch/       # 6.1P  Project pitch node (.ino)
├── Task8.1HDAudioProcessing/  # 8.1HD Voice control over BLE (.py)
└── README.md
```

## Running the tasks

**Arduino tasks (`.ino`)**
1. Open the sketch in the Arduino IDE.
2. Install any libraries via **Tools → Manage Libraries** (common ones used here: `WiFiNINA`,
   `ThingSpeak`, `BH1750`, `DHT sensor library`, `ArduinoMqttClient`).
3. Select **Arduino Nano 33 IoT** and the correct COM port, wire up the components, then upload.

**Raspberry Pi tasks (`.py`)**
1. Copy the script to the Pi.
2. Install what it needs (`gpiozero` for the GUIs; `vosk`, `sounddevice`, `bleak` for voice control).
3. Run with `python3 <file>.py`.


---

*holafelix · SIT210 T1 2026*

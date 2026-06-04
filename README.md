# SIT210 — Embedded Systems Development

> Deakin University · Trimester 1, 2026 · Learning portfolio

![Board](https://img.shields.io/badge/Board-Arduino%20Nano%2033%20IoT-00979D)
![MCU](https://img.shields.io/badge/MCU-SAMD21-blue)
![IDE](https://img.shields.io/badge/Built%20with-Arduino%20IDE-teal)
![Language](https://img.shields.io/badge/Language-C%2B%2B-orange)

This repo holds my weekly tasks for **SIT210 Embedded Systems Development**. Each task is a small,
self-contained embedded project sensing the environment, reacting to inputs, and (where the task
asks for it) pushing data to the web. Everything runs on an **Arduino Nano 33 IoT** breadboarded
with a handful of sensors, LEDs, and switches.

The point of the unit is to go from "make an LED blink" up to interrupt-driven sensing and
IoT-connected devices, so the tasks get a bit more involved as they go.

## Hardware I'm using

- Arduino Nano 33 IoT (SAMD21)
- BH1750 ambient light sensor (I2C, 3V3)
- HC-SR501 PIR motion sensor (needs 5V from VUSB)
- DHT temperature sensor
- SPDT slide switch
- LEDs + 220Ω resistors, breadboard, jumper wires

## Tasks

| Task | Title | What it does |
|------|-------|--------------|
| **1.1P** | Switching ON Lights | A porch + hallway lighting system driven by a slide switch, written with a **modular programming** approach (small helper functions instead of one big `loop()`). |
| **2.1P** | Temperature & Light to the Web | Reads temperature and ambient light, then pushes the data to **ThingSpeak** over WiFi so it shows up on an online dashboard. |
| **4.1P** | Interrupt-Based Lighting | A motion-activated lighting system using **hardware interrupts** — a PIR sensor + BH1750 (lights only come on in the dark) with a slide switch as a manual backup. |

> More tasks get added here as I work through the trimester.

## Repo structure

Each task lives in its own folder. Folder and sketch names use underscores (not dots) so the
Arduino IDE handles the files properly.

```
SIT210_2026T1/
├── Task1_1Lights/          # Task 1.1P
├── Task2_1WebData/         # Task 2.1P
├── Task4_1Interrupts/      # Task 4.1P
└── README.md
```

(Folder names match the actual sketch folders in the repo.)

## How to run any task

1. Open the task's `.ino` file in the Arduino IDE.
2. Install any libraries the task needs via **Tools → Manage Libraries** (each task notes what it uses).
3. Select **Arduino Nano 33 IoT** as the board and the right COM port.
4. Wire up the components per that task's pin table, then upload.

## Notes

- This is coursework — it's written at a student level on purpose, not as production code.
- AI tools were used to help while building and learning these tasks, declared in line with
  Deakin's GenAI and academic integrity requirements where the unit asks for it.

---

*holafelix · SIT210 T1 2026*

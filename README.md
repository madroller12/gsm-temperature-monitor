# GSM-Based Temperature Monitoring System

An automated IoT prototype built on Arduino that monitors ambient temperature and provides remote data access via the cellular network using AT commands.

## Description
This project utilizes an ATmega-based microcontroller integrated with an LM35 analog temperature sensor and a SIM800 GSM module. The system actively monitors environmental conditions and features an automated remote trigger: when a user places a phone call to the system, the device automatically rejects the incoming call (to eliminate cellular charges) and instantly replies with an SMS containing the exact real-time temperature data.

## Features
- **Remote Data Retrieval:** Triggers real-time SMS temperature updates via missed calls.
- **Automated Rejection Logic:** Uses standard AT commands (`ATH`) to hang up automatically.
- **Threshold Alerts:** Monitors temperature limits and triggers local LED status indicators and urgent notifications when specific limits are exceeded.
- **Hardware Serial Bridge:** Built-in debugging bridge for direct manual AT command execution via the Serial Monitor.

## Hardware Stack
- Arduino Board (ATmega328P)
- SIM800 GSM/GPRS Module
- LM35 Analog Temperature Sensor
- Status LEDs & Resistors

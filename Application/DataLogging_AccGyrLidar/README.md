# MOST-BBArduino

**Board target:** Arduino Nano 33 BLE Sense (or compatible)  

---

## 📘 Overview

MOST-BBArduino is an Arduino-based firmware project designed to **log motion and proximity data** from onboard sensors to an SD card, while simultaneously streaming live readings over the serial interface.

The firmware reads data from:

- **BMI270 + BMM150** – a 6-axis IMU (accelerometer + gyroscope + magnetometer)
- **APDS9960** – a proximity and gesture sensor
- **SD card module** – for persistent data logging via SPI

This project is part of the **MOST** system, intended for embedded data acquisition and motion tracking.

---

## ⚙️ Features

- Real-time reading of IMU and proximity sensors  
- Automatic CSV-formatted data logging to SD card  
- Timestamped sensor data with millisecond precision  
- Serial monitor output for live debugging  
- Simple modular design (easily extendable)  
- Compatible with standard Arduino libraries

---

## 🧩 Hardware Requirements

| Component | Description | Notes |
|------------|-------------|-------|
| **Arduino Nano 33 BLE Sense** | Built-in BMI270 + BMM150 + APDS9960 | Recommended board |
| **Micro SD Card Module** | For data storage | Connected via SPI |
| **Micro SD Card** | FAT32 formatted | Minimum 2 GB |
| **USB Cable** | For power and serial connection | – |

### 🪛 Pin Connections

| Arduino Pin | Connected To | Description |
|--------------|--------------|-------------|
| D10 | SD Module CS | Chip Select |
| D11 | SD Module MOSI | SPI Data |
| D12 | SD Module MISO | SPI Data |
| D13 | SD Module SCK | SPI Clock |
| 3V3 / GND | SD Module Power | Power lines |

---

## 📦 Dependencies

These libraries must be installed through the Arduino Library Manager:

```text
Arduino_BMI270_BMM150
Arduino_APDS9960
SD
SPI
```

---

## 🧠 Operation Flow

### 1. Initialization (`setup()`)

1. Starts serial communication at 9600 baud.  
2. Initializes the SD card.  
3. Creates a data file on the SD card with a timestamped filename (`IMU_XXXXX.txt`).  
4. Initializes the IMU (BMI270) and proximity sensor (APDS9960).  
5. Prints the sensor sampling frequencies.

### 2. Data Logging (`loop()`)

1. Waits until IMU and proximity data are available.  
2. Reads:
   - Accelerometer (X, Y, Z)
   - Gyroscope (X, Y, Z)
   - Proximity
3. Generates a timestamp (`millis()`).  
4. Writes one CSV line to the SD card:  
   ```
   log#, timestamp_ms, ax, ay, az, gx, gy, gz, proximity
   ```  
5. Prints readings to the serial monitor.

---

## 📄 Example Output

**Serial Monitor:**
```
Log #15, Timestamp: 4521 ms, Acceleration in X: 0.0234 g, ...
Gyroscope in Z: 0.0017 dps
Proximity: 180
```

**SD Card (IMU_4521.txt):**
```
15, 4521, 0.0234, 0.0021, 1.0012, 0.0056, 0.0042, 0.0017, 180
```

---

## 🧾 Data Format

| Field | Unit | Description |
|--------|------|-------------|
| **log#** | – | Sequential sample number |
| **timestamp_ms** | ms | Time since power-on |
| **ax, ay, az** | g | Acceleration in X/Y/Z |
| **gx, gy, gz** | dps | Angular velocity in X/Y/Z |
| **proximity** | raw (0–255) | Distance reading from APDS9960 |

---

## ⚠️ Limitations

- Sampling rate depends on loop speed (no fixed frequency).  
- Each record opens/closes the SD file → possible performance loss.  
- Data file name generated from `millis()` may repeat on quick restarts.  
- No headers in data files (optional improvement).  

---

## 📈 Performance

| Metric | Estimate |
|---------|----------|
| IMU sample rate | ~100 Hz |
| Logging rate | 10–50 Hz (depends on SD speed) |
| Log file growth | ~30 KB per minute |
| Typical run time | Continuous until SD full |

---


## 🏁 Quick Start

1. Connect the SD module to the Arduino as per wiring table.  
2. Open `most_bbarduino.ino` in the Arduino IDE.  
3. Select **Arduino Nano 33 BLE Sense** as the board.  
4. Upload the sketch.  
5. Open **Serial Monitor** (9600 baud) to view live data.  
6. After a run, remove the SD card to analyze the CSV logs.

---

## 🧮 Future Extensions

- Add temperature, magnetometer, or gesture data.  
- Implement SD file rollover after N MB.  
- Stream live data via BLE or MQTT.  
- Integrate with MOST cloud or dashboard module.  

---

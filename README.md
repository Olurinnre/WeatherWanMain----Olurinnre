---

# WeatherWAN & IoT-Integrated Weather Monitoring System

This repository contains the design, implementation, and deployment of a LoRaWAN-based environmental monitoring system, developed as part of a Student Industrial Work Experience Scheme (SIWES) at the Communications Research Group, Department of Electronic and Electrical Engineering, Obafemi Awolowo University, Ile-Ife, Nigeria.

## 🛰️ Overview

WeatherWAN is a low-cost, power-efficient IoT weather monitoring solution using LoRa and GSM technologies. The system supports long-range communication and includes sensors for atmospheric data collection, gateways, and data visualization through web interfaces.

## 📍 Project Scope

* **Institution**: Obafemi Awolowo University, Ile-Ife, Nigeria
* **Department**: Electronic and Electrical Engineering
* **Duration**: Feb 13,2023 - Till date
* **Author**: Odejobi Olurinnre Ayinde (EEG/2017/076)
* **Organisation**: Communications Research Group, O.A.U

## 🔧 Technologies & Components

* **Communication**: LoRa (SX1278), GSM (SIM800L/SIM900A)
* **Microcontrollers**: ESP32, Arduino
* **Sensors**:

  * ATMOS 14 / 41
  * BME280
  * Precipitation Sensor
  * MET014A Anemometer
  * Adafruit Anemometer
  * SenseCAP ORCH S4
* **Power Supply**:

  * Lithium-ion Battery
  * Lead-acid Battery
  * Super Capacitors
  * Solar Panels
* **PCB Tools**: Autodesk Eagle, Proteus, LTspice, KiCad
* **3D Printing**: PLA, ABS, PETG using 123D Autodesk
* **Packaging**: IP65-rated industrial casing

## 📡 System Architecture

* **Star-of-Star LoRaWAN Topology**
* **Gateways** transmit data to cloud via GSM
* **Nodes** collect environmental data
* **Data Visualization** through weatherwan.io interface

## 📊 Data Monitored

* Air temperature
* Humidity
* Wind speed and direction
* Barometric and vapor pressure
* Rainfall
* Solar radiation
* Light intensity


## 🚀 Features

* Real-time data acquisition
* Long-range LoRa transmission (up to 15 km)
* Redundant sensors for failover
* Solar-powered with efficient battery management
* Modular PCB design

## 🧠 Additional Work

* **Firmware design** for sensor communication and data parsing
* **Embedded system programming** with ESP32
* **Circuit amplification** for sensor output

## 🛠️ Build & Setup

> Detailed circuit schematics, firmware files, and setup instructions can be found in the `docs/` folder.

### Prerequisites

* ESP32 Dev Board
* Arduino IDE
* LoRa SX1278 modules
* Sensors (ATMOS14/BME280, etc.)
* GSM module
* Power supply units (battery/solar)

### Setup Steps

1. Clone this repo:

   ```bash
   git clone https://github.com/your-username/WeatherWAN.git
   ```
2. Upload firmware via Arduino IDE
3. Assemble hardware components following schematics
4. Power the system via solar or battery
5. Access data via GSM + backend dashboard

## 📷 Project Media

![WeatherWAN Setup Diagram](docs/images/system_overview.png)
![White House Node](docs/images/whitehouse_node.jpg)


## 📞 Contact

For inquiries or collaboration:

**Odejobi Olurinnre Ayinde**
Email: odejobiolurin@gmail.com
Department of Electronic and Electrical Engineering,
Obafemi Awolowo University, Ile-Ife, Nigeria.

---



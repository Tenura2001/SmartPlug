# 🔌 SmartPoint with Power Monitoring – IoT Consumer Electronics Project

![Smart Power Monitor Banner](assets/banner.png)

A fully functional IoT-based **Smart Power Monitor** designed as part of a product development cycle training program, covering **end-to-end prototyping**, **PCB design**, **firmware development**, and **enclosure fabrication**.  
The project integrates **ESP32**, **Node-RED**, **MQTT**, and a **custom-designed PCB** to measure and monitor power usage in real time.

---

##  Table of Contents
1. [ Introduction](#-introduction)
2. [ Features](#-features)
3. [ Learned Things](#-learned-things)
4. [ Technologies Used](#-technologies-used)
6. [ Hardware Components](#-hardware-components)
7. [ Firmware Development](#-firmware-development)
8. [ IoT Integration](#-iot-integration)
10. [ Images & Demonstrations](#-images--demonstrations)
11. [ License](#-license)
12. [ Credits](#-credits)

---

##  Introduction

This project demonstrates the **complete product development cycle** for an IoT consumer product — from **conceptualization** to **final working prototype**.  
The **Smart Power Monitor** measures power usage, sends data to a dashboard, and provides a **real-time IoT interface** using Node-RED and MQTT.

It was developed as part of a comprehensive hands-on training program covering:
- Product development cycle & prototyping
- PCB design with KiCAD & fabrication
- Firmware development for ESP32 (PlatformIO & Wokwi)
- IoT dashboard creation using Node-RED and MQTT
- 3D CAD enclosure design using OnShape
- Final assembly & soldering

---

##  Features
- **Custom PCB** with optimized schematic & layout
- **ESP32 firmware** for real-time data acquisition
- **IoT-enabled dashboard** with Node-RED & MQTT
- **3D-printed enclosure** with UK-standard plug
- Support for **through-hole & SMT soldering**
- **Portable & compact** for consumer use

---

## 📚 Learned Things
This project was developed as part of a professional course covering the following:

### Topics:
- Product development cycle and prototyping for IoT products
- Prototype design case study for a consumer electronics product (Smart Power Monitor)
- Understanding product requirements through a Business Requirement Document
- Schematic and PCB layout designing using KiCAD and DXF board outlines
- Best practices in PCB schematic and layout design
- Introduction to firmware development for ESP32 using Wokwi
- Fabrication of a 2-layer PCB
- Introduction to CAD design for consumer electronics using OnShape
- Enclosure design for IoT-based consumer products using OnShape and DXF enclosure drawings
- Firmware development and testing using VSCode and PlatformIO
- Step-by-step through-hole/SMT soldering of a custom PCB using a soldering station
- Fundamental concepts of IoT with ESP32
- Introduction to Node-RED for IoT
- Integrating ESP32, Node-RED, and MQTT for a final product
- Final product demonstration of the Smart Power Monitor

---

## 📚 Technologies Used
- **Hardware Design:** KiCAD, DXF Board Outlines
- **Firmware Development:** PlatformIO (VSCode), Wokwi
- **IoT Dashboard:** Node-RED, MQTT
- **CAD Design:** OnShape, DXF Enclosure Drawings
- **PCB Fabrication:** 2-layer custom PCB
- **Assembly:** Through-hole & SMT soldering

---

## Hardware Components

| Qty | Value          | Description |
|-----|---------------|-------------|
| 1   | 0.1uF         | CAP PP 0.1uF 10% 310VAC Leaded |
| 1   | 820uF         | CAP ALU ELECTROLYTIC 820uF 20% 6.3V SMD |
| 1   | 100uF         | CAP ALU ELECTROLYTIC 100uF 20% 35V SMD |
| 1   | 22uF          | CAP ALU ELECTROLYTIC 22uF 20% 450V Leaded |
| 2   | 2.2uF         | CAP CER 2.2uF 20% 250VAC Radial Leaded Disc |
| 6   | 4.7uF         | CAP CER 4.7UF 25V X5R 0603 |
| 15  | 0.1uF         | CAP CER 0.1UF 50V X7R 0603 |
| 2   | 0.47uF        | CAP CER 0.47UF 50V X7R 0603 |
| 1   | 1uF           | CAP CER 1uF ±10% 25V X7R 0603 V |
| 2   | 22 pF         | CAP CER 22PF 50V C0G/NP0 0603 |
| 1   | 15nF          | CAP CER 0.015UF 50V C0G/NP0 0805 |
| 1   | 10uF          | CAP CER 10UF 10V X7R 0805 |
| 1   | 22uF          | CAP CER 22UF 16V X5R 0805 |
| 1   | brass_pins    | Brass pins connector |
| 1   | SMBJ7.0A      | TVS Diode 600W 7V 5% Uni-Directional |
| 3   | RED LED 0603  | LED SMD Red 2.4 V 0603 |
| 1   | GREEN LED     | LED Through hole Green |
| 1   | 0697H0250-02  | FUSE Through Hole 250mA 350V |
| 1   | SZ1608K182CTF | FERRITE BEAD 200mA 1800Ω ±25% 0603 1LN |
| 1   | Header 1×4 2.54mm | CONN HEADER 4POS VERT T/H TIN |
| 1   | Header 1×2 2.54mm | CONN HEADER 2POS VERT T/H TIN |
| 1   | 1.2mH         | INDUCTOR SMD 1.2 mH 2.2A ±10% 9.8mm |
| 1   | 4.7uH         | INDUCTOR SMD 4.7 uH 2.2A ±20% 5×5mm |
| 1   | Buzzer        | Buzzer SMD 4000Hz 5×5mm |
| 4   | M3            | M3 mounting hole M3 × 0.5 Pan Head |
| 1   | PBO-5C-3      | AC/DC Power Modules, 5 W, 3.3 Vdc, single output, open PCB |
| 1   | FDV303N       | MOSFET N-Channel 0.115A Id, 60V Vds, SOT-23 |
| 1   | 12R           | RES Through Hole 12Ω 5% 3W - Wirewound Resistor |
| 1   | 3M            | RES SMD 3MΩ 5% 1/2W 0805 |
| 8   | 150R          | RES SMD 150Ω 1% 1/10W 0603 |
| 4   | 10K           | RES SMD 10kΩ 5% 1/10W 0603 |
| 1   | 0.0007R       | RES SMD 0.7mΩ 1% 5W PWR METAL STRIP |
| 1   | 0R            | RES SMD 0Ω 0% 1/10W 0603 |
| 1   | 1k            | RES SMD 1kΩ 0.1% 1/4W 1206 |
| 4   | 249k          | RES SMD 249kΩ 0.5% 1/4W 1206 |
| 1   | 56R           | RES SMD 56Ω 1% 250mW 0603 |
| 1   | 33R           | RES SMD 33Ω 1% 250mW 0603 |
| 1   | 1K            | RES SMD 1kΩ 5% 100mW 0603 |
| 1   | 100k          | RES SMD 100kΩ 5% 100mW 0603 |
| 1   | ADW1203HLW    | RELAY GEN PURPOSE SPST 16A 5V |
| 1   | SWITCH        | SWITCH TACTILE SPST-NO 0.05A 12V |
| 1   | SWITCH        | SWITCH TACTILE SHORT TRAVEL SWITCH 6×6, 12.5MM |
| 5   | Test Point    | PC TEST POINT MINIATURE SMT |
| 1   | ADE9153A      | Energy Measurement 32-Pin LFCSP EP Tray |
| 2   | DRDC3105F-7   | 6 V 500 mA SMT Integrated Relay Inductive Load Driver - SC-74 |
| 1   | ESP32-WROOM-32E | ESP32-WROOM-32E (M113EH6400PH3Q0) 8 MB flash |

## Firmware Development
[View Firmware Code](firmware/src/main.cpp)

## IoT Integration
The Smart Power Monitor utilizes **Node-RED** for IoT data processing, visualization, and user interaction. Node-RED is a powerful, open-source, flow-based development tool ideal for building IoT applications with minimal coding.

### Why Node-RED?

- **Visual Flow Editor:** Easily create workflows by connecting nodes graphically.
- **Rapid Prototyping:** Quickly develop dashboards and integrate protocols like MQTT.
- **Extensible:** Supports a wide variety of nodes including MQTT, HTTP, database, and dashboard widgets.
- **Lightweight & Portable:** Runs on multiple platforms such as Raspberry Pi, servers, or cloud.

### Integration Architecture

1. **Data Acquisition:**  
   The custom ESP32 firmware reads power consumption from sensors on the PCB and publishes the data via **MQTT** protocol to an MQTT broker.

2. **Data Reception & Processing:**  
   Node-RED subscribes to MQTT topics to receive the real-time sensor data. Using function nodes, it processes, filters, or analyzes the data as needed.

3. **Visualization:**  
   Data is displayed on a **Node-RED Dashboard** using gauges, charts, and text elements, enabling intuitive real-time monitoring of power consumption.

4. **Alerts & Automation:**  
   Node-RED flows can be extended to trigger alerts or perform automated actions based on thresholds or specific conditions.

### System Diagram

```mermaid
graph LR
    ESP32 -->|MQTT Publish| MQTT_Broker
    MQTT_Broker -->|MQTT Subscribe| NodeRED
    NodeRED --> Dashboard



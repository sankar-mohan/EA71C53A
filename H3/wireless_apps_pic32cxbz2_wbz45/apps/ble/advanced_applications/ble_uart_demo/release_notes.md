---
parent: Release notes
title: BLE UART Demo
has_children: false
has_toc: false
nav_order: 1

family: PIC32CX-BZ
function: BLE
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# PIC32CX-BZ BLE UART Demo Firmware Release Notes

____
## Release v0.9.0.4

### New Features
1. Support central Role multi-link (up to 6 Peripheral Devices).
    - Including massive data transmition.
2. Sleep mode supported.
    - For power saving purpose, extended Adv is disabled when sleep mode feature is enabled.
    - Data transmision over UART is not supported due to HW re-work is required. (PC tool needs to control a GPIO pin to wake up PIC32CX-BZ before it starts performing data transmision over UART.)

### Bug fixes
1. Fix unexpected disconnection isse of mulit-link when the filter policy of advertising is set as accept list only. (#619)
2. Improve the multi-link connection stablility. (#627, #720, #623, #626, #627, #640)

### Known Issues
1. The data might be lost at PC Tool side during the massive UART data transmission due to MCP2200.
2. Some iOS devices (such as iPad Air 3, iOS 14.4.1) might have connection failed issue when PIC32CX-BZ enables Extended Advertising (#577, #600).
3. Sleep mode:
    - If SOSC is chosen for the low power clock source, the unexpected disconnection might be observed. (#822)
    - Not allow the system to enter sleep mode if there is no BLE activity. (#821)
4. Unexpected diconnection is observed during the data transmision when PIC32CX-BZ is connected with 4 phones.(#811, #875)

### Development Tools
- MPLAB X v5.50 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB® XC32 C/C++ Compiler v3.01 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.80)
____
## Release v0.7.2.6

### New Features
1. Support Peripheral Role Multi-link (up to 4 Mobile Phones).
2. Support Central Role multi-link (up to 6 Peripheral Devices, connection establishment only, without massive data transmition).
3. Support Extended Advertising and Extended Scan.
4. Support Long Range Advertising and Connection based on Coded PHY.
5. Support Trusted Device Connectable Mode via enabling Advertising Accept List.
6. Handle and Store Connection Priority Information of bonded devices into flash


### Bug fixes
1. Improve multi-link connection stablility (#463, #467).

### Known Issues
1. The data might be lost at PC Tool side during the massive UART data transmission due to MCP2200.
2. Some iOS devices (such as iPad Air 3, iOS 14.4.1) might have connection failed issue when PIC32CX-BZ enables Extended Advertising (#577, #600).
3. When PC32CX-BZ works as peripheral role under multi-link, abnormal disconnection might be observed (Probability: 1/10 ).<br>And the data comparison might be failed during data transmission of loopback mode (Probability: 1/10 ).
4. When PC32CX-BZ works as central role under multi-link, abnormal disconnection might be observed after the connection is established (Probability: 2/10 ).

### Development Tools
- MPLAB X v5.45 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB® XC32 C/C++ Compiler v3.00 TC6 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.73)
____
## Release v0.7.1.7

### New Features
1. Central Role Supported:
   - PIC32CX-BZ can scan and connect with another PIC32CX-BZ device.
2. Transparent Profile Client Role Supported:
   - Data Transmission Demo Mode:
     1. Loopback Mode
     2. UART Mode
3. Multi-link With Two Mobile Phones Supported.
4. Pairing Supported. (Default Disabled by a Build Option)
5. Perisistent Data Server Library Included for Storing User Data Into Flash.
6. Button Contol:
   - Support short/long press to initate advertising/scan, switch demo mode or terminate the link based on differnt state.
7. LED Indication for Peripheral and Central Role.


### Bug fixes
1. Fix if the user forces close the mobile app under data transmition, the LE link can’t be established again (#243, #281).

### Known Issues
1. The data might be lost at PC Tool side during the massive UART data transmission due to MCP2200.
2. LE link might be disconnected under multi-link case. (The probability is quite low, less than 1/10.) (#463, #467)

### Development Tools
- MPLAB X v5.45 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB® XC32 C/C++ Compiler v2.50 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.55)
____
## Release v0.7.0.2

### New Features
1. Supported Role: Peripheral
   - Advertising control:
     - Data beacon is assigned in the advertising data
     - Device name is assigned in the scan response data
2. Supported BLE Profile/Service:
   - Transparent Profile and Service (TRP/TRS)
   - Transparent Credit Based Profile and Service (TRCBP/TRCBS)
   - Device Information Service (DIS)
3. UART supported:
   - Up to 921600 kbps
   - H/W flow control supported.
4. Data Transmission Demo Mode:
   - Burst Mode
     1) Checksum Mode
     2) Fixed Pattern Mode
     3) Loopback Mode
     4) UART Mode
   - Text Mode
     1) Loopback Mode
     2) UART Mode
5. LED Indication:
   - Advertising State: The green LED is flashed one time every 500 ms.
   - LE Connected State: The green LED is solid on.


### Bug fixes
- None

### Known Issues
1. Force close the mobile app when the data is transmitting, then the LE link can’t be established again.

### Development Tools
- MPLAB X v5.45.04.2695 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB® XC32 C/C++ Compiler v2.40 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.54)

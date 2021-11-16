---
parent: Release notes
title: BLE Extended Advertising
has_children: true
has_toc: false
nav_order: 267
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# PIC32CX-BZ/WBZ451 Extended Advertisements Firmware Release Notes
____
## **Release v0.7.3.0** (11/9/2021)

### New Features
+ Updated the application to latest BLE stack
+ MPLAB X project - Device Name changed from "PIC32CX1012BZ25048"  to "WBZ451" module
+ Enabled BLE Low Power Mode

### Bug fixes
- None

### Known Issues
- None

## Release v0.7.2.1

### Features
1. Supported Role: Broadcaster
   - Advertising control:
     - Extended Advertisements (ADV_EXT_IND and ADV_AUX_IND PDU types)
     - Permitted PHY's (1M, 2M, Coded PHY (125Kbps))
2. UART Settings:
   - Speed: 115200, Data: 8-bit, Parity: none, stop bits: 1 bit, Flow control: none


### Bug fixes
- None First Release

### Known Issues
- None

### Development Tools
- [MPLAB X v5.50]((https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers))
- [MPLAB® XC32 C/C++ Compiler v3.01](https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.80)

---
parent: Release notes
title: BLE Scan Extended Advertising
has_children: false
has_toc: false
nav_order: 280

family: PIC32CX-BZ
function: BLE
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# PIC32CX-BZ/WBZ451 Extended Advertisement Scanner Firmware Release Notes
____
## **Release v0.7.3.1** (02/02/2022)

### New Features
+ Example converted to MCC based project

### Bug fixes
- None

### Known Issues
- None

## Release v0.7.3.0

### New Features
+ Updated the application to latest BLE stack
+ MPLAB X project - Device Name changed from "PIC32CX1012BZ25048"  to "WBZ451" module

### Bug fixes
- None

### Known Issues
- None


## Release v0.7.2.1

### Features
1. Supported Role: Observer/Scanner
   - Scan Advertising control:
     - Scan Extended Advertisements (ADV_EXT_IND and ADV_AUX_IND PDU types)
     - Permitted PHY's (1M , 2M, Coded PHY (125Kbps))
2. UART Settings:
   - Speed: 115200, Data: 8-bit, Parity: none, stop bits: 1 bit, Flow control: none


### Bug fixes
- None First Release

### Known Issues
- None

### Development Tools
- MPLAB X v5.50 (https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers)
- MPLAB?XC32 C/C++ Compiler 3.01
- Device Pack: PIC32CX-BZ_DFP (1.0.80)

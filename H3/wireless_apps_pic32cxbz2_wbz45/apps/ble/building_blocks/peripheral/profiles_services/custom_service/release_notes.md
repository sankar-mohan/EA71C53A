---
parent: Release notes
title: BLE Tranparent UART (Peirpheral)
has_children: true
has_toc: false
nav_order: 269
---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# PIC32CX-BZ/WBZ451 Custom Service [Button-RGB LED] (Peripheral) Firmware Release Notes
____
## **Release v0.1.0.1** (02/02/2022)

### New Features
+ Example converted to MCC based project

### Bug fixes
- None

### Known Issues
- None

## Release v0.1.0.0

### Features
1. Supported Role: Peripheral
2. Custom Service (Button-RGB LED) - server
3. UART Settings:
   - Speed: 115200, Data: 8-bit, Parity: none, stop bits: 1 bit, Flow control: none


### Bug fixes
- None First Release

### Known Issues
- During low power (wake-up) operation UART transmission/reception may be affected by baud rate error due to clock configuration/stabilization. 

### Development Tools
- [MPLAB X v5.50]((https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers))
- [MPLAB® XC32 C/C++ Compiler v3.01](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers#tabs) 
- Device Pack: PIC32CX-BZ_DFP (1.0.80)

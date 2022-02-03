---
parent: Release notes
title: Zigbee Multiprotocol Lights Prov App
has_children: false
has_toc: false
nav_order: 350

---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# PIC32CX-BZ2/WBZ45 Combo Light and Provisioning Firmware Release Notes

## Release v0.2.0.1 (02/02/2022)

### Enhancements
+ Example converted to MCC based project

### Bug fixes
- None

### Known Issues
- None

## Release v0.2.0.0

### Enhancements
+ Updated the application to latest Zigbee and BLE stack repo
+ Added support for WBZ451 module

### Bug fixes
- None

### Known Issues
- None

## Release v0.1.0.2

### Major Changes
1. Updated to latest Zigbee and BLE stack repo
2. Fixed the Advertisement payload not updated when light status is changed from zigbee
3. Changed the commisioning status notification

### Bug fixes
- CHMRSW-675


### Known Issues
- None

## Release v0.1.0.1

### Major Changes
1. Updated to latest Zigbee stack repo
2. Wrong Commisioning failure status value when Finding and binding is not success case is fixed
3. Changed BLE notification when 2 deg difference in temperature instead of 1 deg.

### Bug fixes
- CHMRSW-665

### Known Issues
- None

## Release v0.1.0.0

### New Features
1. The first release for combo light and zigbee provisioning
2. Supported Zigbee device type: Extended Light
3. Supported BLE Service: DIS, TRS,
4. Supported BLE Profile: TRPS,
5. Default UART baurate: 115200 kbps

### Bug fixes
- None

### Known Issues
- None

### Development Tools
- [MPLAB X v5.45](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v3.01](https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.80)


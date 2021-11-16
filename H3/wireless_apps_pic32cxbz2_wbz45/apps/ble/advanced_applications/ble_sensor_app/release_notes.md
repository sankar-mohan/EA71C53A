---
parent: Release notes
title: BLE Sensor Release notes
has_children: false
has_toc: false
nav_order: 255

family: PIC32CX-BZ/WBZ45
function: BLE

---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# PIC32CX-BZ/WBZ45 BLE Sensor App Firmware Release Notes

____
## Release v0.7.3.2

### New Features
+ Updated the application to latest BLE stack repo
+ Added support for WBZ451 module

### Bug fixes
- None

### Known Issues
- None

## Release v0.7.2.1

### New Features
1. Update to BLE SDK Stack v0.7.3.5
2. Update to Latest Wireless H3 repo
3. Stack size for BLE task is changed to 2*1024
4. Project name is updated with ble_sensor_app_wbz451_curiosity_board

### Bug fixes
- None

### Known Issues
- None

### Development Tools
- [MPLAB X v5.45](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v3.0](https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.73)

## Release v0.7.2.0

### New Features
1. Update to BLE SDK Stack v0.7.3.3
2. Updated to 1.0.73 DFP version

### Bug fixes
- None

### Known Issues
- None

### Development Tools
- [MPLAB X v5.45](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v3.0](https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.73)


## Release v0.7.1.1

### New Features
1. Added pairing feature (triggered by TRS CCCD permission)
2. Update to BLE SDK Stack v0.7.2.1
3. Integrate PDS and app_idle_task and enable IDLE HOOK
4. Updated to 1.0.55 DFP version

### Bug fixes
- Fixed CHMRSW-471 (If iOS APP cancels the pairing, it will be stuck on the scan page)

### Known Issues
- None

### Development Tools
- ([MPLAB X v5.45])(https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v2.50](https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.55)

## Release v0.7.0.2

### New Features
1. The first release for Beta EA
2. Supported role: Peripheral/Server
3. Supported Service: DIS, TRS,
4. Supported Profile: TRPS,
5. Default UART baudrate: 115200 kbps

### Bug fixes
- None

### Known Issues
- None

### Development Tools
- [MPLAB X v5.45](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v2.40](https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ_DFP (1.0.54)

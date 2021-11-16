---
parent: Release notes
title: Zigbee Multi-Sensor and commissioning
has_children: false
has_toc: false
nav_order: 351

---

[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# PIC32CX-BZ/WBZ451 BLE Zigbee Provisioning in Multi-Sensor Device (Low power)
____
## Release v0.1.0.0

### New Features
- The current consumption observed during the sleep time is 745uA approximately. The measuring setup is shown below.
  <div style="text-align:center"><img src="assets/current_measurement.jpg" /></div>
  


### Bug fixes
- None

### Known Issues
- Intermittent 5.5mA power consumption observed after BLE Advertisement.
- When the device is goes through a power reset after commisioning, the sleep current consumption increases by 250uA (approx)


### Development Tools
- [MPLAB X v5.50]((https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers))
- [MPLAB® XC32 C/C++ Compiler v3.01](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers#tabs) 
- Device Pack: PIC32CX-BZ_DFP (1.0.80)

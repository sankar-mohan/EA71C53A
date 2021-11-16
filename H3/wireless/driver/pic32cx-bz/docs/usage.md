---
grand_parent: Driver libraries
parent: PIC32CX-BZ Device Support Component Library
title: PIC32CX-BZ Device Support Library Usage
nav_order: 1
---

# PIC32CX-BZ2 Device Support Library Usage

## Configuring the library

There is no configuration of this library.

## Using the library

When the device is first powered on, the system will be calibrated with its factory settings by calling:
RF_SYS_Initialize(void);

*Note:  This is automatically called by the Harmony 3 framework.

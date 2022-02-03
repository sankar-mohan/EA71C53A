---
grand_parent: Examples applications
parent: Zigbee
title: Zigbee light Distributed Network
has_children: true
has_toc: false
nav_order: 250

family: PIC32CX-BZ
market:
  - wireless
---


[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)
# Zigbee Application Demo: Light Forming Distributed Network

---
## WBZ451 Curiosity Board

Devices (Device): \| **PIC32CX1012BZ25048(MCU) on WBZ451 module**  \|

Peripherals (Used, On-Board): \| **RGB LED** \| **User Button**\| **UART-USB Converter**\|

## Introduction

This page describes the zigbee network formation in light devices on WBZ451 Curiosity boards. Though zigbee lights are Router device types, they have the capability of creating its own distributed network. Other routers and end-devices can enter into the network once it is formed. The demo steps explained here can be followed for ZLO devices like color scene controller to get into the light formed distributed network.

## Prerequisites :

+ ### Hardware Required

    |**Tool**                | **Qty**  |
    |------------------------| ---------|
    |WBZ451 Curiosity Boards  | 1         |
    |Micro USB cable         | 1       |
    |Personal Computer     | 1        |

+ ### SDK Setup -   [SDK Setup](../../docs/pic32cx_bz2_wbz45x_sdk_setup.md)

+ ### Terminal Software - [TeraTerm](https://ttssh2.osdn.jp/index.html.en)

## Demo Description

|**Application**                | **Zigbee Logical Device Type**  | **Functionality**|
|------------------------| ---------|---------|
|Extended Lights        | Router        |             Is a lighting device that can form the distributed network.

WBZ451 Curiosity board is programmed with Combined Interface which can act as Zigbee Gateway/Coordinator.

Program the pre-compiled hex image [Programming the precompiled hex file using MPLABX IPE](#tasks) on curiosity boards to run the demo.


- If want to do changes in the demo code and would like to program/debug the customized code follow the instruction in [Build and Program The Application](#tasks_1)
- If want to generate an application from scratch refer: [Creating New Application Device Types](#tasks_3)

### On-board Resource Usage
| **Sensor**         | **Peripheral Used**                   | **I/O Pin**   | **Comment**                                                                                     |
| ------------------ | ------------------------------------- | ------------- | ----------------------------------------------------------------------------------------------- |
| LED Control        | TC2 (WO0, WO1), TC3 (WO0) in PWM Mode | PB0, PB3, PB5 | RGB LED color control.                                                                          |
| Switch             | EXTINT0                               | PB4           | SW1 button in curiosity board to do Reset To Factory new                                         |

<a name="tasks_2">
</a>

## Demo Steps: Distributed Network Formation with Light

1. Supply power to WBZ451 Curiosity Board consisting of Light application by connecting a USB cable. Power Supply (PS) Green LED will turn on when connect to PC.


<div style="text-align:center"><img src="assets/Curiosity_USER_LED_USB.jpg" /></div>

2. The application activity is shown as "Console Log" through on board UART-USB converter
    - Open Terminal(eg: Tera Term) with the setup as shown below to look for these logs

    On the PC side virtual COM port connection that corresponds to the board shall have following settings:

    - BAUD RATE: 115200 (as configured in SERCOM configuration)
    - PARITY: None
    - DATA BITS: 8
    - STOP BITS: 1
    - FLOW CONTROL: None

    Additionally, local echo and sending line ends with line feeds shall be enabled in the PC serial terminal application.

     <div style="text-align:center"><img src="assets/terminal.png" /></div>

3. If light device wouldn't be able to find any network to join, it will create a distributed network. Any device shall be able to join the network via appropriate authentication process. It will open up the network for other zigbee devices to join for first 180 seconds from the first powerON.
* Shows the success console output from a Extended Light Application which is a Zigbee router and establishes a Distributed Network. For example Color Scene controller, which is end device type joins the light formed network :
<div style="text-align:center"><img src="../doc/resources/distributed-network_ext-lights_csc.png" /></div>

* To open up the network after 180 seconds to allow other devices to join, we have to input the below commands in light, before commissioning is initiated in another device.

  1. *setPermitJoin 180*  -> This command opens up the network for next 180sec
  2. *invokeCommissioning 8 0*  -> This command opens up the network for "finding and binding procedure"

---
<a name="tasks">
</a>

## Programming the precompiled hex file using MPLABX IPE

1.  Precompiled Hex file Extended light is located <a href="precompiled_hex/"> Extended Light </a>

2.  Follow the steps mentioned [here](https://microchipdeveloper.com/ipe:programming-device)

 **Caution:** Users should choose the correct Device and Tool information

3. Follow the steps for [running the demo](#tasks_2)

---

<a name="tasks_1">
</a>

## Build and Program The Application

The source code of demo application is available here <a href="firmware"> Extended Light </a> . If want to do changes in the demo code and would like to program/debug the customized code follow the below instruction.


1. Open the "firmware/zigbee_ExtendedLight.X" MPLABX project from MPLABX

	  <div style="text-align:center"><img src="../doc/resources/mplabx1.png" /></div>
	  <div style="text-align:center"><img src="../doc/resources/mplabx.png" /></div>    


2. Do your changes in the code. Clean and build your application by clicking on the Clean and Build button as shown below.

	  <div style="text-align:center"><img src="../doc/resources/mplabx2.png" /></div>

3. Program your application to the device, by clicking on the Make and Program button as shown below.

	  <div style="text-align:center"><img src="../doc/resources/mplabx3.png" /></div>

---

<a name="tasks_3">
</a>

## Creating Application Device Types From Scratch Using MCC

All the supported device types including this Extended Colour light projects can be generated by following the steps in <a href="../zigbee_project_generation.md"> Generating project from MCC

---

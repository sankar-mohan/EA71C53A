---
grand_parent: Examples applications
parent: Zigbee
title: Zigbee Multi-Sensor Low Power
has_children: true
has_toc: false
nav_order: 211

family: PIC32CX-BZ
market:
  - wireless
---
# PIC32CX-BZ2/WBZ45 ZigBee Low Power Application Demo: Zigbee Multi-Sensor

This tutorial will help users to create a low power enabled zigbee project from Microchip H3 project generator framework. The step by step procedure will help the user to generate a Zigbee Multi-Sensor application from scratch.

## Hardware Required

  |**Tool**                | **Qty**  |
  |------------------------| ---------|
  |WBZ451 Curiosity Board  | 1        |
  |Micro USB cable         | 1        |

## SDK Setup

[SDK Setup](../../../apps/docs/pic32cx_bz2_wbz45x_sdk_setup.md)

## Demo Description

This application demonstrate the Zigbee Multi-Sensor end device joining to Zigbee Coordinator (Combined Interface or third-party gateway's such as Amazon Echo plus can also be used instead of CI.) After joining, multisensor device will start ZCL attribute reporting of sensor data such as temperature, occupancy, light, and humidity after connecting to the network.

<div style="text-align:center"><img src="assets/zigbee_link.jpg" /></div>

## Developing this Application from scratch using Harmony 3
The following steps will help to understand the PIC32CXBZ2 device ZIGBEE stack programming structure. We recommend to follow the steps.

### Pull-in H3 Components

1.  Create a new MPLAB Harmony 3 Project -- [link](../../../apps/docs/creating_new_mplabx_harmony_project.md) for instructions

2.  Open MPLABx Harmony 3 Configurator. The Wireless components will be displayed in available components. Select Multi Sensor from Available Components --> Wireless --> Zigbee as shown in the below figure.

	<div style="text-align:center"><img src="assets/available_components.jpg" /></div>

3.  Drag Multi Sensor component from available components to project graph area as shown in below figure. Accept Dependencies or satisfiers, select "Yes".

	<div style="text-align:center"><img src="assets/project_graph_multisensor.jpg" /></div>

4.  Select the Multi Sensor Zigbee device component by click on the component in the project graph. Now the *Configuration Options* tab will list the Configurations for the selected Zigbee device.
	Ensure that the *Automatic(Default) Configuration* is selected as shown in below figure.
	
	<div style="text-align:center"><img src="assets/configuration_options_ms.jpg" /></div>
 
### Low power Configuration

5.  For Zigbee applications, the System Sleep mode is automatically enabled when using the following device types - Multisensor, Intruder Alarm System and Color Scene controller as per the Zigbee End Device Spec. There is no separate configuration that a user has to select to enable the System Sleep.

6.  Upon adding Multi-Sensor, FreeRTOS related settings will be set automatically.
	- Tick Mode will be set to Tickless_Idle
	- Expected idle time before sleep will be set to 5 (ms)
	- Tick Hook will be enabled (For user to add any custom code needed to be executed within each tick interrupt)
	- RTC peripheral library will be added and configured 
	<div style="text-align:center"><img src="assets/sleep_freertos.jpg" /></div>
	
	**Note:** RTC counter should not be reset (RTC_Timer32CounterSet()) arbitrarily when the system is running
	
7.  RTC clock source should be set manually, there are 4 options to choose from
	 - FRC (±1% offset)
	 - LPRC ( with larger offset, < ±5%)
	 - POSC  <- Candidate of the clock source (better clock accuracy)
	 - SOSC <- Candidate of the clock source (better clock accuracy)

	Users can select POSC or SOSC as the RTC clock source. In this example SOSC is configured as RTC clock source.

8.  In Harmony 3 configurator, Click "Tools" select "Clock Configuration"
	<div style="text-align:center"><img src="assets/Tools_Clock_selection.jpg" /></div>

9.  In Clock Diagram, 
	 - Enable Secondary Oscillator (SOSC) by setting `SOSCEN` to `ON`
	 - Select `SOSC` as clock source for `VBKP_32KCSEL` and set `LPCLK_MOD` to `DIV_1_024` as hown in below below.
	 <div style="text-align:center"><img src="assets/ClockDiagram.jpg" /></div>
	 - It is recommended to use 48MHz as SYS_CLOCK for better power savings. This can be configured by setting SPLLPOSTDIV1 to 2 as shown below.
	 <div style="text-align:center"><img src="assets/sys_clock.jpg" /></div>
	 - Ensure that JTAG Enable is disabled by clearing the JTAGEN bit in CFGCON0 (Configuration Control Register 0) as shown below.\
     `CFG_REGS->CFG_CFGCON0CLR = CFG_CFGCON0_JTAGEN_Msk;`
	 **Note:** This is done in `GPIO_Initialize()` as shwon below.
	 <div style="text-align:center"><img src="assets/jtagen.jpg" /></div>

10. All Unused pins in the application needs to be set in input mode and the pulldown should be enabled for these pins. This can be configured through pin configuration in Harmony3 Configurator as shown below.
     <div style="text-align:center"><img src="assets/lowpower_pin_config.jpg" /></div>

11. Open app_user_edits.c file. Comment out or remove the #error line. Update the freertos_hooks.c as mentioned in app_user_edits.c file

12. Compile and Run the project in WBZ45x device.

### Demo Steps: Commissioning

13. The Zigbee Multi-Sensor can be connected to any zigbee network.
    The steps explained in [Discovery of Multi-Sensor from Amazon Echo]() can be followed to connect Zigbee Multi-Sensor to Amazon Echo, if Amazon Echo has Built-in Zigbee smart home hub.
	The steps explained in [Discovery of Multi-Sensor from WBZ451 Combined Interface (CI) Coordinator]() B can be followed for WBZ451 based combined interface acts as Zigbee Coordinator.

#### Discovery of Multi-Sensor from Amazon Echo

14. Multi-Sensor can be added to Alexa's Zigbee network by voice commands or by using Alexa mobile app
	- **Voice Commands:** Open Alexa to discover the Multi-Sensor device. Say “Discover my devices”. or 
	- **Alexa App:**
	1. Launch Alexa app, from the menu, select the Add Device.
	2. Select the type of smart home device “Motion Sensor” and select other.
	3. Initiate Discover Devices
	4. Power on the Multi-Sensor device. The Multi-Sensor will be discovered and shown as motion sensor in Alexa app as shown below.

	<div style="text-align:center"><img src="assets/alexa_app.jpg" /></div>

	**Note:** Echo Plus is in discovery mode for 45 secs. Devices wanting to join Echo Plus should initiate joining/connecting procedure within this time limit for a successful join

#### Discovery of Multi-Sensor from WBZ451 Combined Interface (CI) Coordinator

15. Supply power to another WBZ451 Curiosity Board which is programmed with Combined interface image by connecting a USB cable. Power Supply (PS) Green LED will turn on when connect to PC.
	- Program Combined interface precomplied hex file by following the procedure
	- Open Terminal (eg: Tera Term)and setup UART.
	- send command: *resetToFN* and look for the below logs for successful zigbee network formation on CI
	<div style="text-align:center"><img src="assets/CI_1.png" /></div>

	- CI will open up the network for other zigbee devices to join for first 180sec from the first powerON. If commissioning of Multi-Sensor is initiated after this 180sec, Multi-Sensor will not get joined. This is same as Alexa saying "Discovering and put the device is pairing mode". To open up the network after 180sec, send the below commands in CI, before commissioning is initiated in combo device.
    1. *setPermitJoin 180*  -> This command opens up the network for next 180sec
    2. *invokeCommissioning 8 0*  -> This command opens up the network for "finding and binding procedure"
	<div style="text-align:center"><img src="assets/CI_2.png" /></div>
	3. Supply power to WBZ451 Curiosity Board which is programmed with Multi-Sensor image by connecting a USB cable. The Multi-Sensor will search for Zigbee coordinator device and will join to network and intiate Finding & Binding.
	4. Once Multi-Sensor finishes Finding & Binding procedure, it will start attribute reporting. The Combined interface terminal log will print the recived attribute information as shown below.
	<div style="text-align:center"><img src="assets/CI_teraterm.jpg" /></div>
	
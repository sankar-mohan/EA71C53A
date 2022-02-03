---
grand_parent: Examples applications
parent: Zigbee
title: Zigbee Green Power
has_children: true
has_toc: false
nav_order: 223

family: PIC32CX-BZ
market:
  - wireless
---
[![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)](https://www.microchip.com)

# Zigbee Green Power

---

 Green Power is a feature of Zigbee that allows for energy-harvesting technology to be used directly with the Zigbee stack. Green Power is the global wireless standard for large installations where changing batteries and accessing devices becomes costly.

 ## Green Power Infrastructure Device Types :

  **1. Green Power Proxy (GPP) or Proxy:**
 A fully compliant Zigbee device, which in addition to the core zigbee specification also implements proxy functionality of the Green Power feature, basic or advanced.
 The proxy is able to handle GPDFs (Green Power Data Frame) and tunnel the received GPDF to the sink on the zigbee network.

  **2. Green Power Sink (GPS) or Sink:**
 A fully compliant Zigbee device, which in addition to a core zigbee specification also implements the sink functionality of the Green Power feature, basic or advanced.
 The sink is thus capable of receiving, processing and executing GPD commands, tunneled and optionally also directly received.
 Since all the Zigbee Routers supports proxy functionality, Sink exists always in combination with proxy as Green power combo which is explained below.

  **3. Green Power Combo (GPC) or Combo**
 A fully compliant Zigbee device, which in addition to a core zigbee specification also implements both the proxy and the sink functionality of the Green Power feature.
 A Combo can thus receive, process and execute both tunneled and directly received GPD commands (in its sink role), as well as forward them to other GP nodes (in its proxy role).

 The serial console commands supported by combo-basic are listed in [Serial Console Commands doc](./consoleCommands.md) to verify Application / Stack behavior.

## Green Power End Device (GPD) Type :
A Green Power device may be considered a fourth logical device type. They are designed to be used in very low power operation, where they are intended to only send very simple commands. These small commands are called GPDFs, which is short for Green Power Data Frames.

**Note:** <i> Currently Green power infrastructure device types alone supported in Microchip zigbee stack. The Green power end device type is expected to be supported in upcoming Microchip zigbee stack. </i>

## Green Power Device Commissioning
Commissioning is done between a GPD and sink/Combo basic. Once commissioning is done, the sink will process the frames received from the particular GPD which got commissioned.
To commission a green power device (GPD) with the endpoint on a target sink (present as a combo on Zigbee device), the sink has to be put in commissioning mode.
If no endpoint in sink that supports the GPD device type, commissioning will fail. If the GPD is not in the vicinity of the sink, commissioning can happen via a proxy device. Commissioning mode is enabled in proxy by broadcasting proxy commissioning mode command from console of sink.
On receipt of GP Proxy Commissioning Mode command, the proxy enters or exits the commissioning mode based on the options field in the command. Commissioning commands from GPD are verified in proxy and gets forwarded to sink via GPCommissioningNotification command.
Note that GPD always sends messages in Green power frame format, whereas the commands that are transferred between proxy and sink are always in zigbee frame format as ZCL Green power cluster commands.
Sink/Proxy can also send commands to GPD, but only when GPD indicates that it is available for reception. This will be indicated in RxAfterTx bit in the data sent by GPD.

 <div style="text-align:center"><img src="doc/resources/ZGPCommissioning.png" /></div>


## Zigbee Green Power Operation</b>
Based on the device type of a GPD, it can act as OnOff switch, Temperature sensor, illuminance sensor etc. After commissioning the GPD might send different commands and attribute reporting according to the device type. The data reaches the sink either directly or via proxy by GP notification command. Both proxy and sink performs security processing on the data sent by the GPD. The GP cluster endpoint of sink receives this GPD data first and forwards it to the appropriate endpoint implemented in sink device.


For more details regarding the Zigbee Green Power Feature, please refer to, the specification from Zigbee Alliance ->
<a href="https://zigbeealliance.org/wp-content/uploads/2019/11/docs-09-5499-26-batt-zigbee-green-power-specification.pdf" target="_top">Link to Zigbee PRO Green Power feature Specification by Zigbee Alliance</a>


MPLAB Code Configurator (MCC) allows to configure the Zigbee Green Power feature as below :

 <div style="text-align:center"><img src="doc/resources/green_power.png" /></div>

 ---

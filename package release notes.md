Early Adopter Package Release Notes
   
## EA3 Release Package(BLE + Zigbee Support):  

### current version 3.1.3 (01/26/2022)

Changes since last Early Adopter package releases 
- v3.1.3 (01/26/2022)
  - [Documentation] Updated Migration Guide and Known Issues document with current information 
  - [Documentation] Datasheet, Errata, Product Brief Updated, to reflect "preliminary" status of document
  - [Documentation] Datasheet, Errata, Product Brief Updated, to remove the SOC variant "PIC32CX1012BZ25032" references
  - [Documentation] README.md, Getting Started Updated, to remove the Kit information Update process as it is no longer applicable
  - [Documentation] Schematic, Updated version of schematics made available 
    - WBZ451 module and associated peripherals powered by separate 3.0V regulator.
    - MikroBus UART Rx, Tx signals connected by sharing with MikroBus SPI SCK and MOSI
  

- v3.1.2 (01/06/2022)
  - [Documentation] Updated instructions in SDK Setup Doc - Documentation/PIC32CX-BZWBZ45x SDK Setup.pdf

- v3.1.1 (12/15/2021)
  - BLE Custom Service Low Power App Example Added
  - Updated instructions for BLE TRP UART central and peripheral examples
  - Updated images of ble building blocks introduction pages

- v3.1 (11/12/2021)
  - BLE & Zigbee Stack Support System Sleep/Standby Mode
  - Sleep Mode examples (Legacy_Adv, Ext_Adv, BLE_Custom_Service, Zigbee Multisensor) added
  - new system services repo added which includes BLE Provisioner H3 System Service and Application Timer, users can use this component to provision any zigbee device type
  - Instructions added on Kit Information Update (mandatory step)
  - Device Name for all application examples changed to "WBZ451"
  - Product Brief - New document Added
  - known issues document updated with known limitations related to Sleep Mode implementation
  - Migration Guide updated to accomodate EA3.1 release
  - Datasheet updates (Updates made since v3.02 release) 
    - Edits made to electrical characteristics to better understand conditions of test and maintain consistency with other parts of DS
    - Removed secure element references
    - Added more info- system configuration section, info related to alternate cfg registers being prsent
    - Added profiles supported/planned to be supported
    - Added WBZ450 info in configuration summary
    - Added diff variants pin outs
    - Corrected pincounts for Module
    - Added clarification of what is 32A variant correspond to
    - Updates made based of A0 jira parser review
    - Minor edits
  - Errata updates (updates made since v3.02 release)
    - Added clarification to TCC WO Errata
    - TC WO errata workaround updated 
    - new errata TCC (ALOCK)


- v3.02 (08/11/2021)
  - Datasheet updates (Updates made since v3.01 release) 
    - Added Confidential Watermark to the Datasheet
    - Removed unspported feature - HCI interface via UART

- v3.01 (07/13/2021)
  - Added Migration Guide
  - Getting Started.html converted to a markdown format
  - Datasheet updates (Updates made since EA1)
    - Added WBZ451 Module Mechanical Drawing
    - Added Introduction section
    - Updated/Added Electrical Characteristics 85 and 125 C
    - Minor edits made based on feedback received since EA1

- v3.0 (07/05/2021)
First BLE + Zigbee (Multiprotocol) Support Package, Updated Errata, Updated Datasheet, added Support documentation to create BLE, Zigbee or Multiprotocol Application from scratch, updated H3 components, added mechanical drawings for WBZ451




## EA2 Release Package(Zigbee only Support): 

### current version 2.0 (03/19/2021)
Changes since last Early Adopter package releases

- v2.0 (03/19/2021) First Package with Zigbee Support, Support documentation like Datasheet and Errata inherited from earlier version of EA1 packages


## EA1 Release Package(BLE only Support):  

### current version 1.2 (03/11/2021)
Changes since last Early Adopter package releases
- v1.2 (3/11/2021)
Updated images and text in SDK and Firmware Examples/Building a new BLE Application.pdf 
Datasheet has new chapters WDK, AES, ICM and minor edits
- v1.11 (2/11/2021)
Updated images in SDK and Firmware Examples/Building a new BLE Application.pdf
- v1.1 (2/3/2021)
Changes since last release
Datasheet includes new chapters CCL, Electrical Specs 85 C and 125 C
- v1.0 (01/25/2021) Initial Release - First Package with BLE Support
Changes since last release




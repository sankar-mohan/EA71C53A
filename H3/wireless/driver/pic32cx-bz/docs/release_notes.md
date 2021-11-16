# Microchip PIC32CX-BZ2 Device Support Release Notes

## Release v0.1.12

### New Features
- [CHMRSW-217] API change RF_ClkGen_Initialize(void) -> SYS_ClkGen_Config(void), RF_SYS_Initialize(void) -> SYS_Load_Cal(wssEnable_t wssEnable)

### Bug fixes
- [CHMRSW-603] Remove the input parameter when calling system sleep/wake up API
- [CHMRSW-603] Clear RCON sleep mode bit when system wakes up
- [CHMRSW-217] Add wss_enable == None support and change API name

### Known Issues
- None
### Development Tools
- MPLAB X v5.50 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB XC32 C/C++ Compiler v3.01 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ-ENG_DFP (1.0.65)
____
## Release v0.1.11

### New Features
- [CHMRSW-799]: for ble only application, don't touch zb regs.use g_wssEnable to know which wss is enable

### Bug fixes
- [CHMRSW-603]: FW must clear RCON sleep bit (44000a30 bit 3) at DEVICE_ExitSleepMode

### Known Issues
- None
### Development Tools
- MPLAB X v5.50 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB XC32 C/C++ Compiler v3.01 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ-ENG_DFP (1.0.65)
____

## Release v0.1.10

### New Features
- Remove the input parameter when calling system sleep/wake up API
- Create softfp lib version.

### Bug fixes
- None

### Known Issues
- None
### Development Tools
- MPLAB X v5.50 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB XC32 C/C++ Compiler v3.01 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ-ENG_DFP (1.0.65)
____

## Release v0.1.9

### New Features
- add system sleep/wake API's
- Updates to rf register initial settings

### Bug fixes
- None

### Known Issues
- None
### Development Tools
- MPLAB X v5.50 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB XC32 C/C++ Compiler v3.01 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ-ENG_DFP (1.0.65)
____

## Release v0.1.8

### New Features
- add IB_GetAntennaGain() API
- Initial rf register 0x64, 0x3d for power control so that it works even without valid IB

### Bug fixes
- None

### Known Issues
- None
### Development Tools
- MPLAB X v5.50 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB XC32 C/C++ Compiler v3.01 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ-ENG_DFP (1.0.45)
____

## Release v0.1.6

### New Features
- Add RF_ClkGen_Initialize API
- Reduce number of ACLB registers to initialize
- Add new API's for reading various Information Block paratmeters
- Rename Library file

### Bug fixes
- Change Arbiter interrupt mask to 0x7F for Zigbee stack

### Known Issues
- None
### Development Tools
- MPLAB X v5.45 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB XC32 C/C++ Compiler v2.50 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ-ENG_DFP (1.0.45)
____

## Release v0.1.5

### New Features
- Power Mode support
- Timer Based RF Calibration
- Loading of PMU Calibration Values
- Workaround for improper PMU calibrated parts (version < v1.0)
- Updated PMU Seed Values

### Bug fixes
- None

### Known Issues
- None
### Development Tools
- MPLAB X v5.45 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB XC32 C/C++ Compiler v2.50 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ-ENG_DFP (1.0.45)
____

## Release v0.1.4

### New Features
- Power Mode support
- Timer Based RF Calibration
- Loading of PMU Calibration Values
- Workaround for improper PMU calibrated parts (version < v1.0)

### Bug fixes
- None

### Known Issues
- None
### Development Tools
- MPLAB X v5.45 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB XC32 C/C++ Compiler v2.50 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ-ENG_DFP (1.0.45)
____

## Release v0.1.3

### New Features
- Power Mode support
- Timer Based RF Calibration
- Loading of PMU Calibration Values
- Workaround for improper PMU calibrated parts (version < v1.0)

### Bug fixes
- None

### Known Issues
- None
### Development Tools
- MPLAB X v5.45 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB XC32 C/C++ Compiler v2.50 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ-ENG_DFP (1.0.45)
____

## Release v0.1.2

### New Features
- Inital Release

### Bug fixes
- None

### Known Issues
- Loading of PMU regs from FTCFG is not supported

### Development Tools
- MPLAB X v5.40 (https://www.microchip.com/mplab/mplab-x-ide)
- MPLAB XC32 C/C++ Compiler v2.40 (https://www.microchip.com/mplab/compilers)
- Device Pack: PIC32CX-BZ-ENG_DFP (1.0.26)
____
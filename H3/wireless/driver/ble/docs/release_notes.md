---
parent: Release notes
title: BLE Driver Library
has_children: false
has_toc: false
nav_order: 110

family: PIC32CX-BZ
function: BLE
---

# BLE Driver Library Release notes

## BLE Release v0.9.0.5
### New Features
+ BLE Stack Lib
	+ BT SYS APIs for sleep mode feature
	    + BT_SYS_GetSleepMode
	    + BT_SYS_EnterSleepMode
	    + BT_SYS_AllowSystemSleep
	    + BT_SYS_RfSuspendReq
	+ Support BLE AFH feature
	+ Support enhanced Advertising feature
	    + Secondary Advertising and Synchronization skip
	    + Channel Classification and update on Secondary and Periodic Advertising
	+ Support virtual sniffer
	+ APIs for CW test mode feature
	    + BLE_DTM_StartCwTest
	    + BLE_DTM_StopCwTest

+ BLE Middleware
    + BT LOG APIs for virtual sniffer feature (bt_log)
        + BLE_LOG_Init
        + BLE_LOG_StackLogHandler

+ BLE Profile/Service
    
    N/A 

+ BLE Stack H3 Component
    + Sleep mode option
    + Customized service table creation
    + Local name configuration in scan response field
    + Build-in service configuration
    + BLE SCM module option 


### Changes
+ BLE Stack Lib
    + Move direct test mode feature from ble_gap to ble_dtm group
    + Add average RSSI information in DTM end test
        
+ BLE Middleware
    + Change ble_dm event BLE_DM_EVT_BONDED_CONNECT to BLE_DM_EVT_CONNECTED (ble_dm)
    + Add ble_dm event BLE_DM_EVT_DISCONNECTED (ble_dm)
    + Add ble_dd event BLE_DD_EVT_CONNECTED and BLE_DD_EVT_CONNECTED (ble_dd)
    + Add ble_scm event BLE_SCM_EVT_CONFIGURED (ble_scm)

+ BLE Profile/Service
    
    N/A

+ BLE Stack Component
    + TX power setting sync with antenna gain configuration

### Bug Fixes
+ BLE Stack Lib
    + BLE Scan window fall short of 80ms (#591)
    + ACL Tx failure after connection parameter update (#709)
    + Link lost or ADV lost issues under multi-link and multi-role scenarios (#827, #844, #845, #866, #720, #607)
    + ACL data stuck after a failed encryption procedure (#843)
    + FW halt when user enable local privacy and set RPA timer (#867)
    + L2cap returns incorrect credit number when receiving fragment data (#563)
    + SMP timeout occurs when local device issues security request to peer device over a bonded link (#809)


+ BLE Middleware
    
    N/A 

+ BLE Profile/Service
    + Profile will return an error code when the application attemps to send TX data but there are some queued tasks in profile such as returning credit/write response. (#830)

+ BLE Stack Component
    + Service UUID endian incorrectly generated (#883)

### Known Issues
+ BLE Stack Lib
    + Sleep Mode limitation:
        + Cannot use SOSC as low power clock for BLE Central role connection (#822)
        + Disallow go to Sleep Mode if no any BLE activity (#821)
    + Multi-link stability issue: (#811, #813)  
        + During 4 phones transfer data with Chimera, one BLE connection may be lost if air interference is worse
    + HW limitation: ECC public key generation and Tx/Rx task operation time could not overlap (#873)
        + It may cause other BLE connections dropped if a BLE pairing procedure is executing

+ BLE Middleware
    
    N/A

+ BLE Profile/Service
    
    N/A

**********************************

## BLE Release v0.7.3.5
### New Features
+ BLE Stack Lib
	+ Enable BT5.2 LL RF Tx Power Control feature
    + Fully GAP privacy feature supported
    + GAP API for setting Extended Advertising preferred Coded Phy
        + BLE_GAP_SetExtAdvCodingScheme
    + GAP API for reading the maximum and minimum factory RF transmit power level
        + BLE_GAP_ReadFactoryTxPowerLevel


+ BLE Middleware
    + DM API for GAP privacy feature (ble_dm)
        + BLE_DM_SetResolvingList
    + Service change management for GATT client role (ble_gcm)
    + Service discovery configuration for specific connection (ble_dd)

+ BLE Profile/Service
    
    N/A 

+ BLE Stack H3 Component

### Changes
+ BLE Stack Lib
    + Extend the maximum connections to 6 for Central role
    + Revise build-in Generic Access Service
        + BLE_GAP_ConfigureBuildInService
    + Revise API argument
        + BLE_GAP_SetExtScanningParams
        + BLE_GAP_SetPhy
        + BLE_GAP_SetDefaultPhy
        + BLE_GAP_ExtCreateConnection
    + Add more system parameters for BLE Stack configure
        + BT_SYS_Init
    + APIs enhancement
        + Add BLE_GAP_SetAdvTxPowerLevel and BLE_GAP_SetConnTxPowerLevel to replace BLE_GAP_SetTxPowerLevel
        
+ BLE Middleware
    
    N/A 

+ BLE Profile/Service
    
    N/A

+ BLE Stack Component
    
    N/A

### Bug Fixes
+ BLE Stack Lib
    + Fix the incorrect data type and typo (#513, #514, #516, #609)
    + Fix link lost and incorrect data exchange issues under multi-link and multi-role scenarios (#575, #576, #578, #580, #607, #617, #623, #626, #627)

+ BLE Middleware
    
    N/A 

+ BLE Profile/Service
    + Revise TRCBP credit calculation for SDU multi-frame re-assemble case
    + Flush all queued data when the TRP/TRCBP link is disconnected

+ BLE Stack Component
    + Fix some issues feedback (#534, #566, #512, #634, #635)

### Known Issues
+ BLE Stack Lib
    
    N/A

+ BLE Middleware
    
    N/A

+ BLE Profile/Service
    
    N/A

**********************************
## BLE Release v0.7.2.3
### New Features
+ BLE Stack Lib
    + Added support for GAP privacy advertiser(using legacy advertising) and peripheral role. Refer to below API descriptions in detail.
        + BLE_GAP_GetLocalPrivacy
        + BLE_GAP_SetLocalPrivacy
    + Added support Periodic Advertising transmission. Refer to below API descriptions in detail. 
        + BLE_GAP_PeriodicAdvInit
        + BLE_GAP_SetPeriAdvParams
        + BLE_GAP_SetPeriAdvData
        + BLE_GAP_SetPeriAdvEnable
    + Added support Sync feature for Periodic Advertising receiving. Refer to below API descriptions in detail.
        + BLE_GAP_SyncInit
        + BLE_GAP_CreateSync
        + BLE_GAP_CreateSyncCancel
        + BLE_GAP_TerminateSync
        + BLE_GAP_SetDevToPeriAdvList
        + BLE_GAP_SetPeriAdvRxEnable
    + Added support RF Tx Power Setting. Refer to below API descriptions in detail.
        + BLE_GAP_SetTxPowerLevel
        + BLE_GAP_ReadTxPowerLevel
    + New APIs for Extended Advertising feature. Refer to below APIs descriptions in detail.  
        + BLE_GAP_RemoveExtAdvSet
        + BLE_GAP_ClearExtAdvSet
    + New APIs for GAP utility feature. Refer to below API descriptions in detail.  
        + BLE_GAP_ReadChannelMap
        + BLE_GAP_ReadRemoteTxPowerLevel
        + BLE_GAP_SetTxPowerReportingEnable
    + Added support for GAP scan filter. Refer to below API descriptions in detail.
        + BLE_GAP_SetScanningFilter
    + Added support for GAP extended connection creation. Refer to below API descriptions in detail.
        + BLE_GAP_ExtCreateConnection
    + Added support for SMP OOB pairing method. Refer to below API descriptions in detail.
        + BLE_SMP_OobDataReply
        + BLE_SMP_ScOobDataReply
    + Added support for SMP keypress notification. Refer to below API descriptions in detail.
        + BLE_SMP_Keypress
    + Added BLE Spec vulnerability (ANSSI) fixed solution. (#257)


+ BLE Middleware
    + Added connection parameter update handle. Refer to below API descriptions in detail. (ble_dm)
        + BLE_DM_ConnectionParameterUpdate

+ BLE Profile/Service
    + Add an API to configurate the permission for the Characteristic of Transparent Service (ble_trs)


### Changes
+ BLE Stack Lib
    + Revise the naming event definition
        + BLE_GAP_EVT_ADVERTISING_REPORT -> BLE_GAP_EVT_ADV_REPORT
        + BLE_GAP_EVT_EXT_ADVERTISING_REPORT -> BLE_GAP_EVT_EXT_ADV_REPORT
        + BLE_GAP_EVT_ADVERTISING_TIMEOUT -> BLE_GAP_EVT_ADV_TIMEOUT
    + Removed events
        + BLE_GAP_EVT_DATA_LENGTH_CHANGE
    + Revise the naming of APIs
        + BLE_GAP_SetAdvertisingData -> BLE_GAP_SetAdvData
        + BLE_GAP_SetScanResponseData -> BLE_GAP_SetScanRspData
        + BLE_GAP_SetAdvertisingParams -> BLE_GAP_SetAdvParams
        + BLE_GAP_SetAdvertisingEnable -> BLE_GAP_SetAdvEnable
        + BLE_GAP_StartEncryption -> BLE_GAP_EnableEncryption
        + BLE_GAP_SetExtAdvertisingParams -> BLE_GAP_SetExtAdvParams
        + BLE_GAP_SetExtAdvertisingData -> BLE_GAP_SetExtAdvData
        + BLE_GAP_SetExtScanResponseData -> BLE_GAP_SetExtScanRspData
        + BLE_GAP_SetExtAdvertisingEnable -> BLE_GAP_SetExtAdvEnable
    + Removed APIs
        + BLE_GAP_Reset
        + GATTS_SendMultipleHandleValue
    + Complete BLE_GAP_SetPhy implementation
        + PHY selection policy for Central role
        + Implement prefer PHY Option S=2 or 8. The original design is fixed PHY option S=8
    + Allow back to back GAP advertising or scan disable/enable APIs access without pending error code
        
+ BLE Middleware
    + Remove BLE Scan Middleware (ble_scan)
    + Rename BLE GATT Manage Middleware (ble_gm) to BLE GATT Client Manage Middleware (ble_gcm)
    + Move BLE Device Discovery Middleware (ble_dd) to BLE GATT Client Manage Middleware (ble_gcm)
    + Move BLE Connection Middleware (ble_conn) to BLE Device Manage Middleware (ble_dm)
    + Added support for RPA resolving (ble_dm)
    + Added support for paired device record by using PDS (ble_dm)

+ BLE Profile/Service
    + Increase the initial value and the maximum return value of the credit number (ble_trsps, trspc)
    + Enhance the input queue management for multi-link (ble_trsps)


### Bug Fixes
+ BLE Stack Lib
    + GAP incorrect error code definition
    + Data might not be able to be transmitted after a massive data transmission (#276)
    + L2cap packet reassemble fail when packet size is larger than 676 (#207)
    + RSSI value in advertising report event is not available (#219)
    + Scan filter cache not clear at scan disable issue (#422)
    + The controller as a slave role will not wait for the ACK packet after it sends LL_TERMINATE_IND PDU (#424)
    + Abnormal termination when termination procedure and other LLCP procedure occur at the same connection event (#424)
    + LE coded PHY in the slave role will miss the next three Rx after correctly received one packet (#428)
    + One skip connection event when there are 14 packets in a 20 ms connection interval (#388)


+ BLE Middleware
    + Unexpected event triggered after connection encrypted (ble_gcm) (#426)

+ BLE Profile/Service
    + Profile doesn't return a valid value of received data length when the L2CAP CoC link is not established (ble_trcbps) (#373)

### Known Issues
+ BLE Stack Lib
    + Observed BLE disconnection rarely under multi-link BLE scenario (#132)
    + BLE Stack halts sometimes after enabling RF calibration in app_idle_task (#434)
    + BLE Stack halts once the application issues "BLE_SMP_NumericComparisonConfirmReply()" to reply the numeric comparison confirm in an unexpected state (#478)
    + Connection with channel selection algorithm 2, after channel updated, may cause channel map not match happen (#450)

+ BLE Middleware
    + Miss to update the connection list once LE is connected. (#456)

+ BLE Profile/Service

    N/A

**********************************
## BLE Release v0.7.0.1
### New Features
+ BLE Stack Lib
    + First released for Beta EA

+ BLE Middleware
    + BLE Device Discovery Middleware (ble_dd)
    + BLE Device Manage Middleware (ble_dm)

+ BLE Profile/Service
    + Transparent Profile and Service (TRP/TRS)
    + Transparent Credit Based Profile and Service (TRCBP/TRCBS)
    + Device Information Service (DIS)
    + Battery Service (BAS)


### Known Issues
+ BLE Stack Lib
    + Exchanging data with peer device might be unsuccessful after BLE connection is established if previous BLE connection was terminated while data was exchanging with peer device. (#243)

+ BLE Middleware
    + Incomplete security procedure handling in ble_dd module

+ BLE Profile/Service

    N/A

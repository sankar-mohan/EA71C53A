
ZIGBEE_DEVICE_TYPES= ['ZIGBEE_COLOR_SCENE_CONTROLLER',
                'ZIGBEE_MULTI_SENSOR',
                'ZIGBEE_COMBINED_INTERFACE',
                'ZIGBEE_THERMOSTAT',
                'ZIGBEE_IAS_ACE',
                'ZIGBEE_ON_OFF_LIGHT',
                'ZIGBEE_DIMMABLE_LIGHT',
                'ZIGBEE_COLOR_LIGHT',
                'ZIGBEE_EXTENDED_COLOR_LIGHT',
                'ZIGBEE_TEMPERATURE_COLOR_LIGHT',
                'ZIGBEE_CUSTOM']
                
def check_zigbee_device():
    for comp in Database.getActiveComponentIDs():
        if comp in ZIGBEE_DEVICE_TYPES:
            return comp
    return None
            

def update_BZparamters():
    trpComponent = Database.getComponentByID("profileBLE_TRP")
    if (trpComponent != None):
        if (trpComponent.getSymbolValue("TRP_BOOL_SERVER") == False):
            trpServerChoice = trpComponent.getSymbolByID("TRP_BOOL_SERVER")
            trpServerChoice.setValue(True)
    bleStackComponent = Database.getComponentByID("libBLEStack")
    if (bleStackComponent != None):
        if (bleStackComponent.getSymbolValue("GAP_ADV_DATA_LOCAL_NAME_EN") == True):
            advDataLocalNameEN = bleStackComponent.getSymbolByID('GAP_ADV_DATA_LOCAL_NAME_EN')
            advDataLocalNameEN.setValue(False)
        if (bleStackComponent.getSymbolValue("GAP_ADV_DATA_SERVICE_DATA_EN") == False):
            advSrvDataEN = bleStackComponent.getSymbolByID('GAP_ADV_DATA_SERVICE_DATA_EN')
            advSrvDataEN.setValue(True)
            advSrvDataUUID = bleStackComponent.getSymbolByID('GAP_ADV_DATA_SERVICE_UUID')
            advSrvDataUUID.setValue(0xFEDA)
            advSrvDataVal = bleStackComponent.getSymbolByID('GAP_ADV_DATA_SERVICE_DATA')
            advSrvDataVal.setValue(0xFF03)
        


def onAttachmentConnected(source, target):
    # print('BZ Provisioning --> onAttachmentConnected source= {}'.format(str(source)))
    # print('BZ Provisioning --> onAttachmentConnected target= {}'.format(str(target)))
    if (source['id'] == 'BLE_Device_Information_Dependency'):
        Database.sendMessage("app_service", "APP_PROV", {"appBleDis":True,"appBZProv":True})
    update_BZparamters()
    zb_dev = check_zigbee_device()
    if (zb_dev != None):
        appZbDevType.setValue(zb_dev)
        print('BZ Provisioning --> onAttachmentConnected zb_dev= {}'.format(str(zb_dev))) 
        zbDevComponent = Database.getComponentByID(zb_dev)
        if(zbDevComponent != None):
            if(zbDevComponent.getSymbolValue('MANUAL_CONFIGURATION') == False):
                manualConfig = zbDevComponent.getSymbolByID('MANUAL_CONFIGURATION')
                manualConfig.setValue(True)
            if(zbDevComponent.getSymbolValue('AUTOMATIC_CONFIGURATION') == True):
                autoConfig = zbDevComponent.getSymbolByID('AUTOMATIC_CONFIGURATION')
                autoConfig.setValue(False)
            if(zbDevComponent.getSymbolValue('NETWORK_FORMATION') == True):
                networkForm = zbDevComponent.getSymbolByID('NETWORK_FORMATION')
                networkForm.setValue(False)
            if (zbDevComponent.getSymbolValue('AUTOMATIC_COMMISSIONING_ON_STARTUP') == True):
                autoCommStartup = zbDevComponent.getSymbolByID('AUTOMATIC_COMMISSIONING_ON_STARTUP')
                autoCommStartup.setValue(False)

def onAttachmentDisconnected(source, target):
    # print('BZ Provisioning --> onAttachmentDisconnected source= {}'.format(str(source)))
    # print('BZ Provisioning --> onAttachmentDisconnected target= {}'.format(str(target)))
    if (source['id'] == 'BLE_Device_Information_Dependency'):
        Database.sendMessage("app_service", "APP_PROV", {"appBleDis":False,"appBZProv":False})

def finalizeComponent(ble_zigbee_prov):
    result = Database.connectDependencies([['bz_prov', 'BLE_ZIGBEE_TRP_Dependency', 'profileBLE_TRP', 'BLE_TRP_Capability']])
    result = Database.connectDependencies([['profileBLE_TRP', 'BLE_TRS_Denpendency', 'svcBLE_TRS', 'BLE_TRS_Capability']])
    result = Database.connectDependencies([['bz_prov', 'PDS_Module_Dependency', 'pdsSystem', 'pds_Command_Capability']])
    result = Database.connectDependencies([['bz_prov', 'PIC32CX_BZ2_DevSupport_Dependency', 'pic32cx_bz2_devsupport', 'Device_Support_Capability']])
    result = Database.connectDependencies([['svcBLE_TRS', 'BLE_Stack_Dependency', 'libBLEStack', 'BLE_Stack_Capability']])
    # result = Database.connectDependencies([['bz_prov', 'BLE_Device_Information_Dependency', 'svcBLE_DIS', 'BLE_DIS_Capability']])
    update_BZparamters()
    # Database.setSymbolValue("profileBLE_TRP", "TRP_BOOL_SERVER", True)

        



def instantiateComponent(ble_zigbee_prov):
    print('ble_zigbee_prov')
    configName = Variables.get("__CONFIGURATION_NAME")
    print configName
    processor = Variables.get("__PROCESSOR")
    print processor

    print('HarmonyCore.ENABLE_APP_FILE = {}'.format(str(Database.getSymbolValue("HarmonyCore", 'ENABLE_APP_FILE'))))
    print('HarmonyCore.ENABLE_OSAL = {}'.format(str(Database.getSymbolValue("HarmonyCore", 'ENABLE_OSAL'))))
    print('FreeRTOS.FREERTOS_USE_QUEUE_SETS = {}'.format(str(Database.getSymbolValue("FreeRTOS", 'FREERTOS_USE_QUEUE_SETS'))))
    
    res = Database.activateComponents(["profileBLE_TRP","svcBLE_TRS","pdsSystem","pic32cx_bz2_devsupport","libBLEStack"])

    print('Config Name: {} processor: {}'.format(configName, processor))    

    appTrpsEnable = ble_zigbee_prov.createBooleanSymbol("BZProv_TRPS_ENABLE", None)
    appTrpsEnable.setLabel("Enable BLE Transparent GATT Service")
    appTrpsEnable.setDefaultValue(True)
    appTrpsEnable.setVisible(False)

    global appZbDevType
    appZbDevType = ble_zigbee_prov.createStringSymbol("ZIGBEE_DEV_TYPE", None)
    appZbDevType.setLabel("ZIGBEE Device Type")
    appZbDevType.setDefaultValue("")
    appZbDevType.setVisible(True)
    appZbDevType.setReadOnly(True)
    
    # zigbeeProv_autoConfig = ble_zigbee_prov.createBooleanSymbol("DISABLE_AUTOMATIC_COMM_ON_STARTUP", None)
    # zigbeeProv_autoConfig.setLabel("Disable Automatic Commissioning on Startup")
    # zigbeeProv_autoConfig.setVisible(True)
    # zigbeeProv_autoConfig.setDefaultValue(False)
    # zigbeeProv_autoConfig.setDescription("Disable Automatic Commissioning on Startup in ZigBee Device")

    # Add app_trps.c
    appTrpsSourceFile = ble_zigbee_prov.createFileSymbol(None, None)
    appTrpsSourceFile.setSourcePath('driver/templates/app_trps.c.ftl')
    appTrpsSourceFile.setOutputName('app_trps.c')
    appTrpsSourceFile.setOverwrite(True)
    appTrpsSourceFile.setDestPath('../../app_prov')
    appTrpsSourceFile.setProjectPath('app_prov')
    appTrpsSourceFile.setType('SOURCE')
    appTrpsSourceFile.setMarkup(True)
    appTrpsSourceFile.setEnabled(True)
    
    # Add app_trps.h
    appTrpsHeaderFile = ble_zigbee_prov.createFileSymbol(None, None)
    appTrpsHeaderFile.setSourcePath('driver/templates/app_trps.h.ftl')
    appTrpsHeaderFile.setOutputName('app_trps.h')
    appTrpsHeaderFile.setOverwrite(True)
    appTrpsHeaderFile.setDestPath('../../app_prov')
    appTrpsHeaderFile.setProjectPath('app_prov')
    appTrpsHeaderFile.setType('HEADER')
    appTrpsHeaderFile.setMarkup(True)
    appTrpsHeaderFile.setEnabled(True)

    # Add app_prov_handler.c
    appProvHandlerSourceFile = ble_zigbee_prov.createFileSymbol(None, None)
    appProvHandlerSourceFile.setSourcePath('driver/templates/app_prov_handler.c.ftl')
    appProvHandlerSourceFile.setOutputName('app_prov_handler.c')
    appProvHandlerSourceFile.setOverwrite(True)
    appProvHandlerSourceFile.setDestPath('../../app_prov')
    appProvHandlerSourceFile.setProjectPath('app_prov')
    appProvHandlerSourceFile.setType('SOURCE')
    appProvHandlerSourceFile.setMarkup(True)
    appProvHandlerSourceFile.setEnabled(True)
    
    # Add app_prov_handler.h
    appProvHandlerHeaderFile = ble_zigbee_prov.createFileSymbol(None, None)
    appProvHandlerHeaderFile.setSourcePath('driver/templates/app_prov_handler.h.ftl')
    appProvHandlerHeaderFile.setOutputName('app_prov_handler.h')
    appProvHandlerHeaderFile.setOverwrite(True)
    appProvHandlerHeaderFile.setDestPath('../../app_prov')
    appProvHandlerHeaderFile.setProjectPath('app_prov')
    appProvHandlerHeaderFile.setType('HEADER')
    appProvHandlerHeaderFile.setMarkup(True)
    appProvHandlerHeaderFile.setEnabled(True)

    # Add app_prov.c - generated file ftl
    appProvSourceFile = ble_zigbee_prov.createFileSymbol(None, None)
    appProvSourceFile.setSourcePath('driver/templates/app_prov.c.ftl')
    appProvSourceFile.setOutputName('app_prov.c')
    appProvSourceFile.setOverwrite(True)
    appProvSourceFile.setDestPath('../../app_prov')
    appProvSourceFile.setProjectPath('app_prov')
    appProvSourceFile.setType('SOURCE')
    appProvSourceFile.setEnabled(True)
    appProvSourceFile.setMarkup(True) 

    # Add app_prov.h - static file ftl
    appProvHeaderFile = ble_zigbee_prov.createFileSymbol(None, None)
    appProvHeaderFile.setSourcePath('driver/templates/app_prov.h.ftl')
    appProvHeaderFile.setOutputName('app_prov.h')
    appProvHeaderFile.setOverwrite(True)
    appProvHeaderFile.setDestPath('../../app_prov')
    appProvHeaderFile.setProjectPath('app_prov')
    appProvHeaderFile.setType('HEADER')
    appProvHeaderFile.setEnabled(True)
    appProvHeaderFile.setMarkup(True) 

    # Add app_error_defs.h - static file
    appErrorHeaderFile = ble_zigbee_prov.createFileSymbol(None, None)
    appErrorHeaderFile.setSourcePath('driver/src/app_error_defs.h')
    appErrorHeaderFile.setOutputName('app_error_defs.h')
    appErrorHeaderFile.setOverwrite(True)
    appErrorHeaderFile.setDestPath('../../')
    appErrorHeaderFile.setProjectPath('')
    appErrorHeaderFile.setType('HEADER')
    appErrorHeaderFile.setEnabled(True)
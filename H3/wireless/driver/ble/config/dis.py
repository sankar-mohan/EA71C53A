def instantiateComponent(svcBLE_DISComponent):
    print('svcBLE_DISComponent')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    print('Config Name: {} processor: {}'.format(configName, processor))

    ##########################
    # Manufacture Name Enable
    ##########################
    disManuEnable = svcBLE_DISComponent.createBooleanSymbol('DIS_MANU_NAME_ENABLE', None)
    disManuEnable.setLabel('Manufacturer Name')
    disManuEnable.setDefaultValue(True)
    disManuEnable.setVisible(True)

    # Manufacture Name
    disServiceManufacture = svcBLE_DISComponent.createStringSymbol("DIS_MANU_NAME_STRING", disManuEnable)
    disServiceManufacture.setLabel('Manufacture Name')
    disServiceManufacture.setDefaultValue("Manufacture Name")
    disServiceManufacture.setVisible(True)
    disServiceManufacture.setReadOnly(False)


    ##########################
    # Model Number Enable
    ##########################
    disModelEnable = svcBLE_DISComponent.createBooleanSymbol('DIS_MODEL_NUM_ENABLE', None)
    disModelEnable.setLabel('Model Name')
    disModelEnable.setDefaultValue(True)
    disModelEnable.setVisible(True)
    
    # Model Number
    disServiceModelNum = svcBLE_DISComponent.createStringSymbol("DIS_MODEL_NUM_STRING", disModelEnable)
    disServiceModelNum.setLabel('Model Number')
    disServiceModelNum.setDefaultValue("Model Number")
    disServiceModelNum.setVisible(True)
    disServiceModelNum.setReadOnly(False)

    ##########################
    # Serial Number Enable
    ##########################
    disSerialEnable = svcBLE_DISComponent.createBooleanSymbol('DIS_SERIAL_NUM_ENABLE', None)
    disSerialEnable.setLabel('Serial Name')
    disSerialEnable.setDefaultValue(False)
    disSerialEnable.setVisible(True)
    
    # Serial Number
    disServiceSerialNum = svcBLE_DISComponent.createStringSymbol("DIS_SERIAL_NUM_STRING", disSerialEnable)
    disServiceSerialNum.setLabel('Serial Number')
    disServiceSerialNum.setDefaultValue("Serial Number")
    disServiceSerialNum.setVisible(True)
    disServiceSerialNum.setReadOnly(False)


    ##########################
    # Hardware Revision Enable
    ##########################
    disHardwareEnable = svcBLE_DISComponent.createBooleanSymbol('DIS_HW_REV_ENABLE', None)
    disHardwareEnable.setLabel('Hardware Revision')
    disHardwareEnable.setDefaultValue(False)
    disHardwareEnable.setVisible(True)    
   
    # Hardware Revision
    disServiceHWRevision = svcBLE_DISComponent.createStringSymbol("DIS_HW_REV_STRING", disHardwareEnable)
    disServiceHWRevision.setLabel('Hardware Revision')
    disServiceHWRevision.setDefaultValue("Hardware Revision")
    disServiceHWRevision.setVisible(True)
    disServiceHWRevision.setReadOnly(False)

    ##########################
    # Firmware Revision Enable
    ##########################
    disFirmwareEnable = svcBLE_DISComponent.createBooleanSymbol('DIS_FW_REV_ENABLE', None)
    disFirmwareEnable.setLabel('Firmware Revision')
    disFirmwareEnable.setDefaultValue(False)
    disFirmwareEnable.setVisible(True)  

    # Firmware Revision
    disServiceFirmware = svcBLE_DISComponent.createStringSymbol("DIS_FW_REV_STRING", disFirmwareEnable)
    disServiceFirmware.setLabel('Firmware Revision')
    disServiceFirmware.setDefaultValue("Firmware")
    disServiceFirmware.setVisible(True)
    disServiceFirmware.setReadOnly(False)

    ##########################
    # Software Revision Enable
    ##########################
    disSoftwareEnable = svcBLE_DISComponent.createBooleanSymbol('DIS_SW_REV_ENABLE', None)
    disSoftwareEnable.setLabel('Software Revision')
    disSoftwareEnable.setDefaultValue(False)
    disSoftwareEnable.setVisible(True)
    
    # Software Revision
    disServiceSoftware = svcBLE_DISComponent.createStringSymbol("DIS_SW_REV_STRING", disSoftwareEnable)
    disServiceSoftware.setLabel('Software Revision')
    disServiceSoftware.setDefaultValue("Software")
    disServiceSoftware.setVisible(True)
    disServiceSoftware.setReadOnly(False) 

    ##########################
    # System Id Enable
    ##########################
    disSysIdEnable = svcBLE_DISComponent.createBooleanSymbol('DIS_SYSTEM_ID_ENABLE', None)
    disSysIdEnable.setLabel('System Id')
    disSysIdEnable.setDefaultValue(False)
    disSysIdEnable.setVisible(True)

    # System Id
    disServiceSystemId = svcBLE_DISComponent.createStringSymbol("DIS_SYSTEM_ID_STRING", disSysIdEnable)
    disServiceSystemId.setLabel('System Id')
    disServiceSystemId.setDefaultValue("System Id")
    disServiceSystemId.setVisible(True)
    disServiceSystemId.setReadOnly(False)


    ##########################
    # IEEE 11073-20601 Enable
    ##########################
    disIeeeEnable = svcBLE_DISComponent.createBooleanSymbol('DIS_IEEE_ENABLE', None)
    disIeeeEnable.setLabel('IEEE 11073-20601')
    disIeeeEnable.setDefaultValue(False)
    disIeeeEnable.setVisible(True)
    
    # IEEE 11073-20601 Regulatory Certification Data
    disServiceIeee = svcBLE_DISComponent.createStringSymbol("DIS_IEEE_STRING", disIeeeEnable)
    disServiceIeee.setLabel('IEEE 11073-20601 Data')
    disServiceIeee.setDefaultValue("NULL")
    disServiceIeee.setVisible(True)
    disServiceIeee.setReadOnly(False)    


    ##########################
    # PnP ID Enable
    ##########################
    disPnpIdEnable = svcBLE_DISComponent.createBooleanSymbol('DIS_PNP_ID_ENABLE', None)
    disPnpIdEnable.setLabel('PnP ID')
    disPnpIdEnable.setDefaultValue(False)
    disPnpIdEnable.setVisible(True)

    # PnP ID
    disServicePnpId = svcBLE_DISComponent.createStringSymbol("DIS_PNP_ID_STRING", disPnpIdEnable)
    disServicePnpId.setLabel('PnP Id')
    disServicePnpId.setDefaultValue("NULL")
    disServicePnpId.setVisible(True)
    disServicePnpId.setReadOnly(False) 
     
    # Add ble_dis.c file
    bleDisSourceFile = svcBLE_DISComponent.createFileSymbol(None, None)
    bleDisSourceFile.setSourcePath('driver/ble/templates/ble_dis.c.ftl')
    bleDisSourceFile.setOutputName('ble_dis.c')
    bleDisSourceFile.setOverwrite(True)
    bleDisSourceFile.setDestPath('ble/service_ble/ble_dis')
    bleDisSourceFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_dis')
    bleDisSourceFile.setType('SOURCE')
    bleDisSourceFile.setEnabled(True)
    bleDisSourceFile.setMarkup(True)
    

    # Add ble_dis.h file
    bleDisHeaderFile = svcBLE_DISComponent.createFileSymbol(None, None)
    bleDisHeaderFile.setSourcePath('driver/ble/templates/ble_dis.h.ftl')
    bleDisHeaderFile.setOutputName('ble_dis.h')
    bleDisHeaderFile.setOverwrite(True)
    bleDisHeaderFile.setDestPath('ble/service_ble/ble_dis')
    bleDisHeaderFile.setProjectPath('config/' + configName + '/ble/service_ble/ble_dis')
    bleDisHeaderFile.setType('HEADER')
    bleDisHeaderFile.setEnabled(True)
    bleDisHeaderFile.setMarkup(True)


def finalizeComponent(BLEStackComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(BLEStackComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['libBLEStack']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])

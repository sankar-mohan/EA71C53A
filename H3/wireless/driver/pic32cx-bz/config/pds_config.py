processor = Variables.get('__PROCESSOR')
print('processor={}'.format(processor))

############################################################################
### Add pds_config.h - File
############################################################################
pds_configHeaderFile = libPDS.createFileSymbol(None, None)
pds_configHeaderFile.setSourcePath('driver/pic32cx-bz/templates/pds/pds_config.h.ftl')
pds_configHeaderFile.setOutputName('pds_config.h')
pds_configHeaderFile.setDestPath('driver/pds/include')
pds_configHeaderFile.setProjectPath('config/' + configName + '/driver/pds/include/')
pds_configHeaderFile.setType('HEADER')
pds_configHeaderFile.setOverwrite(True)
pds_configHeaderFile.setMarkup(True)


############################################################################
### Add logic for adding BLE stack 
############################################################################
# This boolean is controlled in:
#   onAttachmentConnected and onAttachmentDisconnected
bleStackLoaded = libPDS.createBooleanSymbol("BLESTACK_LOADED", None)
bleStackLoaded.setDefaultValue('libBLEStack' in Database.getActiveComponentIDs())
bleStackLoaded.setVisible(False)

############################################################################
### Add logic for adding Zigbee stack
############################################################################
# This boolean is controlled in:
#   onAttachmentConnected and onAttachmentDisconnected
zigbeeStackLoaded = libPDS.createBooleanSymbol("ZIGBEESTACK_LOADED", None)
zigbeeStackLoaded.setDefaultValue(['ZIGBEE_COLOR_SCENE_CONTROLLER' or 'ZIGBEE_MULTI_SENSOR' or 'ZIGBEE_COMBINED_INTERFACE' or 
                                  'ZIGBEE_THERMOSTAT' or 'ZIGBEE_IAS_ACE' or 'ZIGBEE_ON_OFF_LIGHT' or 'ZIGBEE_DIMMABLE_LIGHT' or 
                                  'ZIGBEE_COLOR_LIGHT' or 'ZIGBEE_EXTENDED_COLOR_LIGHT' or 'ZIGBEE_TEMPERATURE_COLOR_LIGHT' or 
                                  'ZIGBEE_CUSTOM' or 'ZIGBEE_GPD_SENSOR'] in Database.getActiveComponentIDs())
zigbeeStackLoaded.setVisible(False)


############################################################################
### Create Application configuration items for PDS
############################################################################
PDS_DisplayApps = libPDS.createIntegerSymbol("PDS_APPS_ITEM_IDS", None)
PDS_DisplayApps.setDefaultValue(0)
PDS_DisplayApps.setMax(10)
PDS_DisplayApps.setMin(0)
PDS_DisplayApps.setVisible(True)
PDS_DisplayApps.setReadOnly(False)

PDS_DisplayApps1 = libPDS.createIntegerSymbol("PDS_APPS_DIRECTORY_IDS", None)
PDS_DisplayApps1.setDefaultValue(0)
PDS_DisplayApps1.setMax(10)
PDS_DisplayApps1.setMin(0)
PDS_DisplayApps1.setVisible(True)
PDS_DisplayApps1.setReadOnly(False)


############################################################################
### Linker File OverWrite - Yog( workaround for PDS)
############################################################################
pdsLinkerFile = libPDS.createFileSymbol("PDS_LINKER_FILE", None)
pdsLinkerFile.setSourcePath("driver/pic32cx-bz/templates/pds/pic32cx_bz2_pds.ld")
pdsLinkerFile.setOutputName("{}.ld".format(processor.upper()))
pdsLinkerFile.setMarkup(True)
pdsLinkerFile.setOverwrite(True)
pdsLinkerFile.setType("LINKER")


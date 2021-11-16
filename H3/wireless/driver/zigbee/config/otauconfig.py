#####################################################################################################
#################               OTAU CONFIGURATION                                   ###############
#####################################################################################################

# OTAU Enable
global otauConfigEnable
otauConfigEnable = drvZigbeeComponent.createBooleanSymbol("OTAU_ENABLE", OtauConfigMenu)
otauConfigEnable.setLabel("OTAU Enable")
otauConfigEnable.setDefaultValue(True)
otauConfigEnable.setDescription("OTAU Enable - check the box to enable")

# OTAU Role
otauConfigRole = drvZigbeeComponent.createComboSymbol("OTAU_ROLE",  OtauConfigMenu, ["CLIENT", "SERVER"])
otauConfigRole.setLabel("OTAU Role")
otauConfigRole.setDefaultValue(OTAURoleCheck())
otauConfigRole.setDescription("OTAU Role - check the box to enable")
otauConfigRole.setDependencies(OtauEnableCheck, ["OTAU_ENABLE"])

# Memory to Use
global otauConfigMemorySelection
otauConfigMemorySelection = drvZigbeeComponent.createComboSymbol("OTAU_USEMEM_INTEXT",  OtauConfigMenu, ["INTERNAL", "EXTERNAL"])
otauConfigMemorySelection.setLabel("Memory to use")
otauConfigMemorySelection.setDefaultValue("INTERNAL")
otauConfigMemorySelection.setDescription("Internal memory or external memory for OTAU - check the box to enable")
otauConfigMemorySelection.setDependencies(OtauEnableCheck, ["OTAU_ENABLE"])

# OTAU External Memory Selection
OtauConfigExtMemSelection = drvZigbeeComponent.createComboSymbol("EXTERNAL_MEMORY",  OtauConfigMenu, [""])
OtauConfigExtMemSelection.setLabel("External Memory")
OtauConfigExtMemSelection.setDefaultValue("")
OtauConfigExtMemSelection.setVisible(False)
OtauConfigExtMemSelection.setDescription("OTAU External Memory option - Select the option")
OtauConfigExtMemSelection.setDependencies(OTAUExtMemCheck, ["OTAU_USEMEM_INTEXT"])
#OtauConfigExtMemSelection.setDependencies(OtauEnableCheck, ["OTAU_ENABLE"])

# OTAU Default Image Type
OtauConfigDefaultImageType = drvZigbeeComponent.createComboSymbol("OTAU_DEFAULT_IMAGE_TYPE",  OtauConfigMenu, ["UNSECURED", 
"MANU_SPEC_ENCRYPT_IMAGE_NO_MIC", "MANU_SPEC_ENCRYPT_IMAGE_PLAIN_CRC", "MANU_SPEC_ENCRYPT_IMAGE_PLAIN_MAC",
 "MANU_SPEC_ENCRYPT_IMAGE_ENCRYPT_CRC", "MANU_SPEC_ENCRYPT_IMAGE_ENCRYPT_MAC","CLIENT_SECURITY_CREDENTIAL ", "CLIENT_CONFIGURATION", "SERVER_LOG","PICTURE","WILD_CARD"])
OtauConfigDefaultImageType.setLabel("Default Image Type")
OtauConfigDefaultImageType.setDefaultValue("MANU_SPEC_ENCRYPT_IMAGE_ENCRYPT_MAC")
OtauConfigDefaultImageType.setDescription("Image type - Select the option from the drop down list")
OtauConfigDefaultImageType.setDependencies(OtauEnableCheck, ["OTAU_ENABLE"])


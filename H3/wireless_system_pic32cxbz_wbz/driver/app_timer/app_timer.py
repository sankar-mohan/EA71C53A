            
def onAttachmentConnected(source, target):
    print('APP Timer --> onAttachmentConnected source= {}'.format(str(source)))
    print('APP Timer --> onAttachmentConnected target= {}'.format(str(target)))
    # if (source['id'] == 'App_Timer_Capability'):
        # Database.sendMessage("app_service", "APP_TIMER_SERVICE", {"isEnabled":True})
    useTimerComponent = Database.getComponentByID("FreeRTOS")
    if (useTimerComponent != None):
        useTimerChoice = useTimerComponent.getSymbolByID("FREERTOS_USE_TIMERS")
        useTimerChoice.setValue(True)

def onAttachmentDisconnected(source, target):
    print('APP Timer --> onAttachmentDisconnected source= {}'.format(str(source)))
    print('APP Timer --> onAttachmentDisconnected target= {}'.format(str(target)))
    # if (source['id'] == 'App_Timer_Capability'):
        # Database.sendMessage("app_service", "APP_TIMER_SERVICE", {"isEnabled":False})
    useTimerComponent = Database.getComponentByID("FreeRTOS")
    if (useTimerComponent != None):
        useTimerChoice = useTimerComponent.getSymbolByID("FREERTOS_USE_TIMERS")
        useTimerChoice.setValue(False)

def finalizeComponent(app_Timer_Component):
    print('App Timer -> finalizeComponent - {}'.format(str(app_Timer_Component)))
    result = Database.connectDependencies([['app_timer', 'PIC32CX_BZ2_DevSupport_Dependency', 'pic32cx_bz2_devsupport', 'Device_Support_Capability']])


def instantiateComponent(app_Timer_Component):
    print('App Timer -> instantiateComponent - {}'.format(str(app_Timer_Component)))
    configName = Variables.get("__CONFIGURATION_NAME")
    print configName
    processor = Variables.get("__PROCESSOR")
    print processor

    Database.activateComponents(["pic32cx_bz2_devsupport"])
    useTimerComponent = Database.getComponentByID("FreeRTOS")
    if (useTimerComponent != None):
        useTimerChoice = useTimerComponent.getSymbolByID("FREERTOS_USE_TIMERS")
        useTimerChoice.setValue(True)

    appTimerEnable = app_Timer_Component.createBooleanSymbol("APP_TIMER_ENABLE", None)
    appTimerEnable.setLabel("Enable App Timer")
    appTimerEnable.setDefaultValue(True)
    appTimerEnable.setVisible(False)

    # Add app_timer.c - generated file ftl
    appTimerSourceFile = app_Timer_Component.createFileSymbol(None, None)
    appTimerSourceFile.setSourcePath('driver/templates/app_timer.c.ftl')
    appTimerSourceFile.setOutputName('app_timer.c')
    appTimerSourceFile.setOverwrite(True)
    appTimerSourceFile.setDestPath('../../app_timer')
    appTimerSourceFile.setProjectPath('app_timer')
    appTimerSourceFile.setType('SOURCE')
    appTimerSourceFile.setEnabled(True)
    appTimerSourceFile.setMarkup(True) 

    # Add app_timer.h - static file ftl
    appTimerHeaderFile = app_Timer_Component.createFileSymbol(None, None)
    appTimerHeaderFile.setSourcePath('driver/templates/app_timer.h.ftl')
    appTimerHeaderFile.setOutputName('app_timer.h')
    appTimerHeaderFile.setOverwrite(True)
    appTimerHeaderFile.setDestPath('../../app_timer')
    appTimerHeaderFile.setProjectPath('app_timer')
    appTimerHeaderFile.setType('HEADER')
    appTimerHeaderFile.setEnabled(True)
    appTimerHeaderFile.setMarkup(True) 
    
    # Add app_error_defs.h - static file
    appErrorHeaderFile = app_Timer_Component.createFileSymbol(None, None)
    appErrorHeaderFile.setSourcePath('driver/src/app_error_defs.h')
    appErrorHeaderFile.setOutputName('app_error_defs.h')
    appErrorHeaderFile.setOverwrite(True)
    appErrorHeaderFile.setDestPath('../../')
    appErrorHeaderFile.setProjectPath('')
    appErrorHeaderFile.setType('HEADER')
    appErrorHeaderFile.setEnabled(True)
    
    # Device Name
    appTimerID0 = app_Timer_Component.createStringSymbol("APP_TIMER_ID0", None)
    appTimerID0.setLabel("APP TIMER ID0")
    appTimerID0.setDefaultValue("APP_TIMER_ID_0")
    appTimerID0.setVisible(True)
    appTimerID0.setReadOnly(False)
    print('app_timer --> appTimerID0 = {}'.format(str(appTimerID0)))
    
    appTimerID1 = app_Timer_Component.createStringSymbol("APP_TIMER_ID1", None)
    appTimerID1.setLabel("APP TIMER ID1")
    appTimerID1.setDefaultValue("APP_TIMER_ID_1")
    appTimerID1.setVisible(True)
    appTimerID1.setReadOnly(False)
    
    appTimerID2 = app_Timer_Component.createStringSymbol("APP_TIMER_ID2", None)
    appTimerID2.setLabel("APP TIMER ID2")
    appTimerID2.setDefaultValue("APP_TIMER_ID_2")
    appTimerID2.setVisible(True)
    appTimerID2.setReadOnly(False)
    
    appTimerID3 = app_Timer_Component.createStringSymbol("APP_TIMER_ID3", None)
    appTimerID3.setLabel("APP TIMER ID3")
    appTimerID3.setDefaultValue("APP_TIMER_ID_3")
    appTimerID3.setVisible(True)
    appTimerID3.setReadOnly(False)
    
    appTimerID4 = app_Timer_Component.createStringSymbol("APP_TIMER_ID4", None)
    appTimerID4.setLabel("APP TIMER ID4")
    appTimerID4.setDefaultValue("APP_TIMER_ID_4")
    appTimerID4.setVisible(True)
    appTimerID4.setReadOnly(False)
    
    appTimerID5 = app_Timer_Component.createStringSymbol("APP_TIMER_ID5", None)
    appTimerID5.setLabel("APP TIMER ID5")
    appTimerID5.setDefaultValue("APP_TIMER_ID_5")
    appTimerID5.setVisible(True)
    appTimerID5.setReadOnly(False)

    
    
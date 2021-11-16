# coding: utf-8
##############################################################################
# Copyright (C) 2019-2020 Microchip Technology Inc. and its subsidiaries.
#
# Subject to your compliance with these terms, you may use Microchip software
# and any derivatives exclusively with Microchip products. It is your
# responsibility to comply with third party license terms applicable to your
# use of third party software (including open source software) that may
# accompany Microchip software.
#
# THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
# WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
# PARTICULAR PURPOSE.
#
# IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
# INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
# WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
# BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
# FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
# ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
# THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
##############################################################################

def loadModule():
    print('Load Module: Harmony Wireless')

    pic32cx_bz2_family = {'PIC32CX1012BZ25048',
                          'PIC32CX1012BZ25032',
                          'PIC32CX1012BZ24032',
                          'WBZ451',
                          'WBZ450',
                          }

    processor = Variables.get('__PROCESSOR')
    print('processor={}'.format(processor))

    if( processor in pic32cx_bz2_family):
        ## PIC32CX-BZ System_Service
        execfile(Module.getPath() + '/config/module_pic32cx_bz2_device_support.py')
        ## Persistant Data Storage
        execfile(Module.getPath() + '/config/module_pic32cx_bz_pds.py')
        ## PIC32CX-BZ Radio stacks
        execfile(Module.getPath() + '/config/module_ble.py')
        execfile(Module.getPath() + '/config/module_zigbee.py')

    else:
        ## WIFI Driver
        drvWifiWincComponent = Module.CreateComponent('drvWifiWinc', 'WINC', '/Wireless/Drivers', 'driver/winc/config/drv_winc.py')
        drvWifiWincComponent.addCapability('wdrv_winc', 'WDRV_WINC', True)
        drvWifiWincComponent.addCapability('libdrvWincMac', 'MAC')

        drvWifiPic32mzw1Component = Module.CreateComponent('drvWifiPic32mzw1', 'PIC32MZW1', '/Wireless/Drivers/', 'driver/pic32mzw1/config/drv_pic32mzw1.py')
        drvWifiPic32mzw1Component.addCapability('wdrv_pic32mzw1', 'WDRV_PIC32MZW1', True)
        drvWifiPic32mzw1Component.addCapability('libdrvPic32mzw1Mac', 'MAC')

        ## Serial Bridge Application
        appSerBridge = Module.CreateComponent('appWincSerialBridge', 'Serial Bridge Application', '/Wireless/SerialBridge/', 'apps/serial_bridge/config/app_winc_serial_bridge.py')

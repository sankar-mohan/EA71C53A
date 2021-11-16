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

## PIC32CX-BZ Radio stacks

print('Load Module: Harmony Wireless BLE Driver (Profiles, Services and Stack Libraries)')

## BLE Stack/Library
uniquelibBLEStackComponent = Module.CreateComponent('libBLEStack', 'BLE_Stack', 'Wireless/BLE', 'driver/ble/config/blestack.py')
uniquelibBLEStackComponent.setDisplayType('BLE Stack')
uniquelibBLEStackComponent.addDependency('PIC32CX_BZ2_DevSupport_Dependency', 'Device_Support', None, True, True)
uniquelibBLEStackComponent.addCapability('BLE_Stack_Capability', 'BLE_Stack', True)

## BLE Profiles
# profileBLE_HOGP_Component = Module.CreateComponent('profileBLE_HOGP', 'HID over GATT Profile', 'Wireless/BLE/Profiles', '/driver/ble/config/hogp.py')
# profileBLE_HOGP_Component.setDisplayType('HID over GATT Profile')
# profileBLE_HOGP_Component.addDependency('BLE_HIDS_Denpendency', 'BLE_HIDS', None, True, True)

# profileBLE_ANP_Component = Module.CreateComponent('profileBLE_ANP', 'Alert Notification Profile', 'Wireless/BLE/Profiles', '/driver/ble/config/anp.py')
# profileBLE_ANP_Component.setDisplayType('Alert Notification Profile')
# profileBLE_ANP_Component.addDependency('BLE_ANS_Denpendency', 'BLE_ANS', None, True, True)

profileBLE_TRP_Component = Module.CreateComponent('profileBLE_TRP', 'Transparent Profile', 'Wireless/BLE/Profiles', '/driver/ble/config/trp.py')
profileBLE_TRP_Component.setDisplayType('Transparent Profile')
profileBLE_TRP_Component.addDependency('BLE_TRS_Denpendency', 'BLE_TRS', None, True, True)
profileBLE_TRP_Component.addCapability('BLE_TRP_Capability', 'BLE_TRP', True)

profileBLE_TRCP_Component = Module.CreateComponent('profileBLE_TRCP', 'Transparent Credit Based Profile', 'Wireless/BLE/Profiles', '/driver/ble/config/trcp.py')
profileBLE_TRCP_Component.setDisplayType('Transparent Credit Based Profile')
profileBLE_TRCP_Component.addDependency('BLE_TRCS_Denpendency', 'BLE_TRCS', None, True, True)
                 
## BLE Services
svcBLE_TRSComponent = Module.CreateComponent('svcBLE_TRS', 'Transparent Service', 'Wireless/BLE/Services', '/driver/ble/config/trs.py')
svcBLE_TRSComponent.setDisplayType('Transparent Profile Service')
svcBLE_TRSComponent.addCapability('BLE_TRS_Capability', 'BLE_TRS', True)
svcBLE_TRSComponent.addDependency('BLE_Stack_Dependency', 'BLE_Stack', None, True, True)

svcBLE_TRCSComponent = Module.CreateComponent('svcBLE_TRCS', 'Transparent Credit Based Service', 'Wireless/BLE/Services', '/driver/ble/config/trcs.py')
svcBLE_TRCSComponent.setDisplayType('Transparent Credit Based Service')
svcBLE_TRCSComponent.addCapability('BLE_TRCS_Capability', 'BLE_TRCS', True)
svcBLE_TRCSComponent.addDependency('BLE_Stack_Dependency', 'BLE_Stack', None, True, True)
 
# svcBLE_ANSComponent = Module.CreateComponent('svcBLE_ANS', 'Alert Notification Service', 'Wireless/BLE/Services', '/driver/ble/config/ans.py')
# svcBLE_ANSComponent.setDisplayType('Alert Notification Service')
# svcBLE_ANSComponent.addCapability('BLE_ANS_Capability', 'BLE_ANS', True)
# svcBLE_ANSComponent.addDependency('BLE_Stack_Dependency', 'BLE_Stack', None, True, True)
 
# svcBLE_HIDSComponent = Module.CreateComponent('svcBLE_HIDS', 'HID Service', 'Wireless/BLE/Services', '/driver/ble/config/hids.py')
# svcBLE_HIDSComponent.setDisplayType('HID Service')
# svcBLE_HIDSComponent.addCapability('BLE_HIDS_Capability', 'BLE_HIDS', True)
# svcBLE_HIDSComponent.addDependency('BLE_Stack_Dependency', 'BLE_Stack', None, True, True)
 
svcBLE_DISComponent = Module.CreateComponent('svcBLE_DIS', 'Device Information Service', 'Wireless/BLE/Services', '/driver/ble/config/dis.py')
svcBLE_DISComponent.setDisplayType('Device Information Service')
svcBLE_DISComponent.addCapability('BLE_DIS_Capability', 'BLE_DIS', True)
svcBLE_DISComponent.addDependency('BLE_Stack_Dependency', 'BLE_Stack', None, True, True)

svcBLE_BASComponent = Module.CreateComponent('svcBLE_BAS', 'Battery Service', 'Wireless/BLE/Services', '/driver/ble/config/bas.py')
svcBLE_BASComponent.setDisplayType('Battery Service')
svcBLE_BASComponent.addCapability('BLE_BAS_Capability', 'BLE_BAS', True)
svcBLE_BASComponent.addDependency('BLE_Stack_Dependency', 'BLE_Stack', None, True, True)

svcBLE_CMSComponent = Module.CreateComponent('svcBLE_CMS', 'Customized Service', 'Wireless/BLE/Services', '/driver/ble/config/cms.py')
svcBLE_CMSComponent.setDisplayType('Customized Service')
svcBLE_CMSComponent.addCapability('BLE_CMS_Capability', 'BLE_CMS', True)
svcBLE_CMSComponent.addDependency('BLE_Stack_Dependency', 'BLE_Stack', None, True, True)

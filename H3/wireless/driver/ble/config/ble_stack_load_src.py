''' File:   ble_stack_load_src.py
    Date:   21-Jul-2020

    This module copies the ble_stack source files to the H3 generated project
    and adds the files to the MPLAB X IDE project setting.
    This is only intended for internal Microchip use.  For public release the
    ble_stack will be provided as a binary library.
'''


print('ble_stack_load_src: loading BLE Stack source files')

def add_ble_lib_files(component, flist, ftype, parent, config):
    """
    Create H3 File Symbols for a list of ble_lib source files

    The source files are located in the H3 module folder under:
        driver/ble/src/ble_lib/<parent>/<folder>/<path>/<fname>

    The files are copied to the project path config/<config>:
        lib_src/ble_stack/<parent>/<folder>/<path>'

    The files are added to the MPLAB X IDE project based on the
    type of file; SOURCE or HEADER.  The files are added to the
    MPLABX IDE logical folder under:
        ('config/<config>/lib_src/ble_stack/<parent>/<path>'
        
    Where:
        <parent> and <config> are passed in as parameters.
        <path> and <fname> are passed in flist
        <folder> is derived from <ftype>
            <ftype> == 'SOURCE' then <folder> = 'src'
            <ftype> == 'HEADER' then <folder> = 'inc'

    Parameters
    ----------
    component   : (localComponent)
                The H3 component parameter to instantiateComponent()
    flist       : list of tuples
                The list contains the paths and filenames
    parent      : string
                The name of the folder containing the path and file
    config      : string
                The name of the MPLAB X IDE project's config folder
    """

    print('ble_load_src:add_ble_lib_files(comp, list, {}, {}, {})'.format(ftype, parent, config))

    if 'SOURCE' == ftype:
        folder = 'src/'
    elif 'HEADER' == ftype:
        folder = 'inc/'
    else:
        folder = ''

    for path, fname in flist:

        print('  parent {}, path {}, fname {}'.format(parent, path, fname))
        fsymbol = component.createFileSymbol(None, None)
        fsymbol.setSourcePath('driver/ble/src/ble_src/{}/{}/{}/{}'.format(parent, folder, path, fname))
        fsymbol.setOutputName(fname)
        fsymbol.setOverwrite(True)
        fsymbol.setDestPath('/lib_src/ble_stack/{}/{}/{}'.format(parent, folder, path))
        fsymbol.setProjectPath('config/{}/lib_src/ble_stack/{}/{}'.format(config, parent, path))
        fsymbol.setType(ftype)
        fsymbol.setEnabled(True)


def add_lib_util_files(component, flist, ftype, parent, config):

    print('ble_load_src:add_lib_util_files(comp, flist[0] {}, {}, {}, {})'.format(repr(flist[0]), ftype, parent, config))

    if 'SOURCE' == ftype:
        folder = 'src/'
    elif 'HEADER' == ftype:
        folder = 'inc/'
    else:
        folder = ''


    for path, fname in flist:

        print('  parent {}, path {}, fname {}'.format(parent, path, fname))
        fsymbol = component.createFileSymbol(None, None)
        fsymbol.setSourcePath('driver\\ble\\src\\{}\\{}\\{}'.format(parent, path, fname))
        fsymbol.setOutputName(fname)
        fsymbol.setOverwrite(True)
        fsymbol.setDestPath('\\lib_src\\{}\\{}'.format(parent, path))
        fsymbol.setProjectPath('config\\{}\\lib_src\\{}\\{}'.format(config, parent, path))
        fsymbol.setType(ftype)
        fsymbol.setEnabled(True)


def addExtraIncludes(component, plist):
    # print('ble_stack_load_src:addExtraIncludes' + ''.join(plist))
    blestack_includes = component.createSettingSymbol("BLE_STACK_SRC_INCLUDES", None)
    blestack_includes.setCategory("C32")
    blestack_includes.setKey("extra-include-directories")
    blestack_includes.setValue(';'.join(plist))
    blestack_includes.setAppend(True, ";")


def addPreprocessorMacros(component, mlist):
    # print('ble_stack_load_src:addPreprocessorMacros' + ''.join(mlist))
    blestack_macros = component.createSettingSymbol("BLE_STACK_SRC_MACROS", None)
    blestack_macros.setCategory("C32")
    blestack_macros.setKey("preprocessor-macros")
    blestack_macros.setValue(';'.join(mlist))
    blestack_macros.setAppend(True, ";")


def setConfigItemC32(component, key, value):
    print('ble_stack_load_src setting C32 config {} to {}'.format(key, value))
    blestack_macros = component.createSettingSymbol("BLE_STACK_C32_CONFIG", None)
    blestack_macros.setCategory("C32")
    blestack_macros.setKey(key)
    blestack_macros.setValue(value)


controller_ble_src = [
    ('', 'bb_clk.c'),
    ('', 'bt_dev.c'),
    ('cmn', 'ble_cmn.c'),
    ('dtm', 'ble_dtm.c'),
    ('hal', 'ble_hal_ch_sel.c'),
    ('hal', 'ble_hal_clk.c'),
    ('hal', 'ble_hal_hop.c'),
    ('hal', 'ble_hal_low_energy.c'),
    ('hal', 'ble_hal_main.c'),
    ('hal', 'ble_hal_packet.c'),
    ('hal', 'ble_hal_palist.c'),
    ('hal', 'ble_hal_privacy.c'),
    ('hal', 'ble_hal_task.c'),
    ('hal', 'ble_hal_test.c'),
    ('hal', 'ble_hal_fal.c'),
    ('isr', 'ble_isr_adv.c'),
    ('isr', 'ble_isr_ini.c'),
    ('isr', 'ble_isr_scn.c'),
    ('isr', 'ble_isr_sync.c'),
    ('lc', 'ble_lc_adv_ext.c'),
    ('lc', 'ble_lc_adv_lega.c'),
    ('lc', 'ble_lc_adv_main.c'),
    ('lc', 'ble_lc_conn.c'),
    ('lc', 'ble_lc_ini_ext.c'),
    ('lc', 'ble_lc_ini_lega.c'),
    ('lc', 'ble_lc_ini_main.c'),
    ('lc', 'ble_lc_main.c'),
    ('lc', 'ble_lc_c.c'),
    ('lc', 'ble_lc_scn_ext.c'),
    ('lc', 'ble_lc_scn_lega.c'),
    ('lc', 'ble_lc_scn_main.c'),
    ('lc', 'ble_lc_p.c'),
    ('lc', 'ble_lc_sync.c'),
    ('ll', 'ble_ll_adv_ext_init.c'),
    ('ll', 'ble_ll_adv_init.c'),
    ('ll', 'ble_ll_conn_api.c'),
    ('ll', 'ble_ll_dtm_init.c'),
    ('ll', 'ble_ll_ini_ext_init.c'),
    ('ll', 'ble_ll_ini_init.c'),
    ('ll', 'ble_ll_main_init.c'),
    ('ll', 'ble_ll_c_init.c'),
    ('ll', 'ble_ll_scn_ext_init.c'),
    ('ll', 'ble_ll_scn_init.c'),
    ('ll', 'ble_ll_p_init.c'),
    ('ll', 'ble_ll_sync_init.c'),
    ('ll', 'll_api.c'),    
    ('lm', 'ble_lm_adv_ext.c'),
    ('lm', 'ble_lm_adv_lega.c'),
    ('lm', 'ble_lm_adv_main.c'),
    ('lm', 'ble_lm_cmn.c'),
    ('lm', 'ble_lm_conn.c'),
    ('lm', 'ble_lm_ini_ext.c'),
    ('lm', 'ble_lm_ini_lega.c'),
    ('lm', 'ble_lm_ini_main.c'),
    ('lm', 'ble_lm_llcp_cmn.c'),
    ('lm', 'ble_lm_llcp_cmd_cmn.c'),
    ('lm', 'ble_lm_llcp_cmd_c.c'),
    ('lm', 'ble_lm_llcp_cmd_p.c'),    
    ('lm', 'ble_lm_llcp_c.c'),
    ('lm', 'ble_lm_llcp_pdu_cmn.c'),
    ('lm', 'ble_lm_llcp_pdu_c.c'),
    ('lm', 'ble_lm_llcp_pdu_p.c'),
    ('lm', 'ble_lm_llcp_p.c'),
    ('lm', 'ble_lm_llcp_utility.c'),
    ('lm', 'ble_lm_main.c'),
    ('lm', 'ble_lm_c.c'),
    ('lm', 'ble_lm_scn_ext.c'),
    ('lm', 'ble_lm_scn_lega.c'),
    ('lm', 'ble_lm_scn_main.c'),
    ('lm', 'ble_lm_p.c'),
    ('lm', 'ble_lm_sync.c'),
    ('phy', 'ble_phy_main.c'),
    ('rc', 'rc.c'),
    ('rc', 'rc_api.c'),
    ]

controller_ble_header = [
    ('common', 'cmn_bb_clk.h'),
    ('common', 'cmn_bt_dev.h'),
    ('common/cmn', 'le_cmn.h'),
    ('common/dtm', 'cmn_ble_dtm.h'),
    ('common/hal', 'cmn_ble_hal_main.h'),
    ('common/isr', 'cmn_ble_isr_adv.h'),
    ('common/isr', 'cmn_ble_isr_ini.h'),
    ('common/isr', 'cmn_ble_isr_pcsg.h'),
    ('common/isr', 'cmn_ble_isr_scn.h'),
    ('common/isr', 'cmn_ble_isr_sync.h'),
    ('common/lc', 'cmn_ble_lc_adv_ext.h'),
    ('common/lc', 'cmn_ble_lc_adv_lega.h'),
    ('common/lc', 'cmn_ble_lc_adv_main.h'),
    ('common/lc', 'cmn_ble_lc_conn.h'),
    ('common/lc', 'cmn_ble_lc_ini_ext.h'),
    ('common/lc', 'cmn_ble_lc_ini_lega.h'),
    ('common/lc', 'cmn_ble_lc_ini_main.h'),
    ('common/lc', 'cmn_ble_lc_main.h'),
    ('common/lc', 'cmn_ble_lc_c.h'),
    ('common/lc', 'cmn_ble_lc_scn_ext.h'),
    ('common/lc', 'cmn_ble_lc_scn_lega.h'),
    ('common/lc', 'cmn_ble_lc_scn_main.h'),
    ('common/lc', 'cmn_ble_lc_p.h'),
    ('common/lc', 'cmn_ble_lc_sync.h'),    
    ('common/ll', 'cmn_ble_ll_adv_ext_init.h'),
    ('common/ll', 'cmn_ble_ll_adv_init.h'),
    ('common/ll', 'cmn_ble_ll_dtm_init.h'),
    ('common/ll', 'cmn_ble_ll_ini_ext_init.h'),
    ('common/ll', 'cmn_ble_ll_ini_init.h'),
    ('common/ll', 'cmn_ble_ll_main_init.h'),
    ('common/ll', 'cmn_ble_ll_c_init.h'),
    ('common/ll', 'cmn_ble_ll_scn_ext_init.h'),
    ('common/ll', 'cmn_ble_ll_scn_init.h'),
    ('common/ll', 'cmn_ble_ll_p_init.h'),
    ('common/ll', 'cmn_ble_ll_sync_init.h'),
    ('common/ll', 'll_api.h'),    
    ('common/lm', 'cmn_ble_lm_adv_ext.h'),
    ('common/lm', 'cmn_ble_lm_adv_lega.h'),
    ('common/lm', 'cmn_ble_lm_adv_main.h'),
    ('common/lm', 'cmn_ble_lm_cmn.h'),
    ('common/lm', 'cmn_ble_lm_conn.h'),
    ('common/lm', 'cmn_ble_lm_ini_ext.h'),
    ('common/lm', 'cmn_ble_lm_ini_lega.h'),
    ('common/lm', 'cmn_ble_lm_ini_main.h'),
    ('common/lm', 'cmn_ble_lm_llcp.h'),
    ('common/lm', 'cmn_ble_lm_main.h'),    
    ('common/lm', 'cmn_ble_lm_c.h'),
    ('common/lm', 'cmn_ble_lm_scn_ext.h'),
    ('common/lm', 'cmn_ble_lm_scn_lega.h'),
    ('common/lm', 'cmn_ble_lm_scn_main.h'),
    ('common/lm', 'cmn_ble_lm_p.h'),
    ('common/lm', 'cmn_ble_lm_sync.h'),
    ('common/phy', 'cmn_ble_phy_main.h'),
    ('common/rc', 'cmn_rc.h'),
    ('internal', 'bb_clk.h'),
    ('internal', 'bt_dev.h'),    
    ('internal/dtm', 'ble_dtm.h'),
    ('internal/hal', 'ble_hal_ch_sel.h'),
    ('internal/hal', 'ble_hal_clk.h'),
    ('internal/hal', 'ble_hal_hop.h'),
    ('internal/hal', 'ble_hal_low_energy.h'),
    ('internal/hal', 'ble_hal_packet.h'),
    ('internal/hal', 'ble_hal_palist.h'),
    ('internal/hal', 'ble_hal_privacy.h'),
    ('internal/hal', 'ble_hal_sys.h'),
    ('internal/hal', 'ble_hal_task.h'),
    ('internal/hal', 'ble_hal_test.h'),
    ('internal/hal', 'ble_hal_fal.h'),    
    ('internal/isr', 'ble_isr_adv.h'),
    ('internal/isr', 'ble_isr_ini.h'),
    ('internal/isr', 'ble_isr_scn.h'),
    ('internal/isr', 'ble_isr_sync.h'),    
    ('internal/lc', 'ble_lc_adv_ext.h'),
    ('internal/lc', 'ble_lc_adv_lega.h'),
    ('internal/lc', 'ble_lc_adv_main.h'),
    ('internal/lc', 'ble_lc_ini_ext.h'),
    ('internal/lc', 'ble_lc_ini_lega.h'),
    ('internal/lc', 'ble_lc_ini_main.h'),
    ('internal/lc', 'ble_lc_main.h'),
    ('internal/lc', 'ble_lc_scn_ext.h'),
    ('internal/lc', 'ble_lc_scn_lega.h'),
    ('internal/lc', 'ble_lc_scn_main.h'),
    ('internal/lc', 'ble_lc_p.h'),
    ('internal/lc', 'ble_lc_sync.h'),    
    ('internal/ll', 'ble_ll_adv_ext_init.h'),
    ('internal/ll', 'ble_ll_adv_init.h'),
    ('internal/ll', 'ble_ll_ini_ext_init.h'),
    ('internal/ll', 'ble_ll_ini_init.h'),
    ('internal/ll', 'ble_ll_main_init.h'),
    ('internal/ll', 'ble_ll_c_init.h'),
    ('internal/ll', 'ble_ll_scn_ext_init.h'),
    ('internal/ll', 'ble_ll_scn_init.h'),
    ('internal/ll', 'ble_ll_p_init.h'),
    ('internal/ll', 'ble_ll_sync_init.h'),
    ('internal/lm', 'ble_lm_adv_ext.h'),
    ('internal/lm', 'ble_lm_adv_lega.h'),
    ('internal/lm', 'ble_lm_adv_main.h'),
    ('internal/lm', 'ble_lm_cmn.h'),
    ('internal/lm', 'ble_lm_ini_ext.h'),
    ('internal/lm', 'ble_lm_ini_lega.h'),
    ('internal/lm', 'ble_lm_ini_main.h'),
    ('internal/lm', 'ble_lm_llcp_cmd.h'),
    ('internal/lm', 'ble_lm_llcp_cmn.h'),
    ('internal/lm', 'ble_lm_llcp_c.h'),
    ('internal/lm', 'ble_lm_llcp_pdu_cmn.h'),
    ('internal/lm', 'ble_lm_llcp_pdu_c.h'),
    ('internal/lm', 'ble_lm_llcp_pdu_p.h'),
    ('internal/lm', 'ble_lm_llcp_p.h'),
    ('internal/lm', 'ble_lm_llcp_utility.h'),
    ('internal/lm', 'ble_lm_main.h'),
    ('internal/lm', 'ble_lm_c.h'),    
    ('internal/lm', 'ble_lm_scn_ext.h'),
    ('internal/lm', 'ble_lm_scn_lega.h'),
    ('internal/lm', 'ble_lm_scn_main.h'),
    ('internal/lm', 'ble_lm_p.h'),
    ('internal/lm', 'ble_lm_sync.h'),    
    ('internal/phy', 'ble_phy_main.h'),
    ('internal/rc', 'rc.h'),
    ]


host_ble_src = [
    ('gap', 'ble_gap_adv.c'), 
    ('gap', 'ble_gap_conn.c'),
    ('gap', 'ble_gap_connc.c'),
    ('gap', 'ble_gap_connp.c'),
    ('gap', 'ble_gap_core.c'),
    ('gap', 'ble_gap_ext_adv.c'), 
    ('gap', 'ble_gap_ext_scan.c'), 
    ('gap', 'ble_gap_main.c'), 
    ('gap', 'ble_gap_scan.c'), 
    ('gap', 'ble_gap_sync.c'),
    ('gap', 'ble_gap_test.c'), 
    ('l2cap', 'ble_l2cap_coc.c'), 
    ('l2cap', 'ble_l2cap_main.c'), 
    ('smp', 'ble_smp_core.c'), 
    ('smp', 'ble_smp_main.c'), 
    ('smp', 'ble_smp_passkey.c'),
    ]


host_ble_header = [
    ('api', 'ble_gap.h'), 
    ('api', 'ble_l2cap.h'), 
    ('api', 'ble_smp.h'), 
    ('internal', 'ble_gap_api.h'), 
    ('internal', 'ble_gap_main.h'), 
    ('internal', 'ble_l2cap_api.h'), 
    ('internal', 'ble_l2cap_main.h'), 
    ('internal', 'ble_smp_api.h'), 
    ('internal', 'ble_smp_main.h'), 
    ]


host_common_src = [
    ('gap', 'gap_main.c'), 
    ('gap', 'gas_svc.c'), 
    ('gatt', 'att_uuid.c'), 
    ('gatt', 'gatt_main.c'), 
    ('gatt', 'gatt_svc.c'),
    ('gatt', 'gattc_disc.c'), 
    ('gatt', 'gattc_ind.c'), 
    ('gatt', 'gattc_main.c'), 
    ('gatt', 'gattc_read.c'), 
    ('gatt', 'gattc_write.c'), 
    ('gatt', 'gatts_ind.c'), 
    ('gatt', 'gatts_main.c'), 
    ('gatt', 'gatts_read.c'), 
    ('gatt', 'gatts_service_mgr.c'), 
    ('gatt', 'gatts_write.c'), 
    ('hci', 'host_hci_core_chimera.c'), 
    ('hci', 'host_hci_le_adv.c'), 
    ('hci', 'host_hci_le_adv_ext.c'), 
    ('hci', 'host_hci_le_conn.c'),
    ('hci', 'host_hci_le_connc.c'),
    ('hci', 'host_hci_le_connp.c'),
    ('hci', 'host_hci_le_main.c'), 
    ('hci', 'host_hci_le_scan.c'), 
    ('hci', 'host_hci_le_scan_ext.c'), 
    ('hci', 'host_hci_le_sync.c'),
    ('hci', 'host_hci_le_test.c'), 
    ('hci', 'host_hci_main.c'), 
    ('l2cap', 'l2cap_main.c'), 
    ('sec', 'sec_aes.c'), 
    ('sec', 'sec_ecc_chimera.c'), 
    ('sec', 'sec_uecc.c'), 
    ('stack', 'bt_host_chimera.c'), 
    ('stack', 'stack_mgr.c'), 
    ('stack', 'stack_queue.c'), 
    ]


host_common_header = [
    ('api', 'att_uuid.h'),
    ('api', 'gap_defs.h'),
    ('api', 'gatt.h'),
    ('api', 'l2cap_defs.h'),
    ('api', 'mba_error_defs.h'),
    ('api', 'stack_mgr.h'),
    ('internal', 'bt_host.h'),
    ('internal', 'bt_host_chimera.h'),
    ('internal', 'gap_api.h'),
    ('internal', 'gap_main.h'),
    ('internal', 'gas_svc.h'),
    ('internal', 'gattc_main.h'),
    ('internal', 'gatts_main.h'),
    ('internal', 'gatt_api.h'),
    ('internal', 'gatt_main.h'),
    ('internal', 'gatt_svc.h'),
    ('internal', 'host_hci_api.h'),
    ('internal', 'host_hci_main.h'),
    ('internal', 'l2cap_api.h'),
    ('internal', 'l2cap_main.h'),
    ('internal', 'sec_aes.h'),
    ('internal', 'sec_ecc.h'),
    ('internal', 'sec_uecc.h'),
    ('internal', 'stack_mgr_api.h'),
    ('internal', 'stack_queue.h'),
    ]

sys_src = [
    ('', 'bt_sys_cm_mem.c'), 
    ('', 'bt_sys_msg.c'), 
    ('', 'bt_sys_task.c'), 
    ('chimera', 'bt_isr.c'), 
    ('chimera', 'bt_sys_aes.c'), 
    ('chimera', 'bt_sys_assert.c'), 
    ('chimera', 'bt_sys_init.c'), 
    ('chimera', 'bt_sys_isr.c'), 
    ('chimera', 'bt_sys_mem.c'), 
    ('chimera', 'bt_sys_rand.c'), 
    ('chimera', 'bt_sys_timer.c'), 
    ('chimera', 'bt_sys_utility.c'), 
    ('chimera', 'reset.c'), 
    ]


sys_header = [
    ('api', 'bt_sys_init.h'), 
    ('internal', 'bt_global.h'), 
    ('internal', 'bt_sys_aes.h'), 
    ('internal', 'bt_sys_db.h'), 
    ('internal', 'bt_sys_isr.h'), 
    ('internal', 'bt_sys_mem.h'), 
    ('internal', 'bt_sys_msg.h'), 
    ('internal', 'bt_sys_queue.h'), 
    ('internal', 'bt_sys_rand.h'), 
    ('internal', 'bt_sys_task.h'), 
    ('internal', 'bt_sys_timer.h'), 
    ('internal', 'bt_sys_utility.h'), 
    ('internal/platform', 'platform.h'),
    ('internal/platform/chimera', 'bb_reg_giant_struct.h'), 
    ('internal/platform/chimera', 'bt_assert.h'), 
    ('internal/platform/chimera', 'bt_isr.h'), 
    ('internal/platform/chimera', 'bt_reg_fields_direct_defs.h'), 
    ('internal/platform/chimera', 'bt_reg_fields_extern.h'), 
    ('internal/platform/chimera', 'bt_reg_struct_defs.h'),
    ('internal/platform/chimera', 'hal_atomic.h'),
    ('internal/platform/chimera', 'platform_chimera.h'),
    ('internal/platform/chimera', 'reset.h'),
    ('internal/type', 'type_32bits.h'),
    ]


utils_header_files = [
    ('', 'compiler.h'),
    ('', 'status_codes.h'),
    ('preprocessor', 'preprocessor.h'),
    ('preprocessor', 'tpaste.h'),
    ('preprocessor', 'stringz.h'),
    ('preprocessor', 'mrepeat.h'),
    ('preprocessor', 'mrecursion.h'),
    ('', 'interrupt.h'),
    ('interrupt', 'interrupt_sam_nvic.h'),
    ('calibration', 'calibration.h'),
    ('calibration', 'info_block.h'),
    #('', 'drv_usart.h'),
    ('', 'clock.h'),
    ('rf', 'rf.h'),
    ]


utils_source_files = [
    # ('calibration', 'calibration.c'),
    # ('calibration', 'info_block.c'),
    # ('rf', 'rf.c'),
    #('usart', 'drv_usart.c'),
    #('usart', 'write.c'),
    ]

ble_stack_includes = [
    '..\\src\\config\\default\\lib_src\\utils',
    '..\\src\\config\\default\\lib_src\\utils\\calibration',
    '..\\src\\config\\default\\lib_src\\utils\\rf',
    '..\\src\\config\\default\\lib_src\\utils\\preprocessor',
    '..\\src\\config\\default\\lib_src\\ble_stack\\controller_ble\\inc', 
    '..\\src\\config\\default\\lib_src\\ble_stack\\host_ble\\inc', 
    '..\\src\\config\\default\\lib_src\\ble_stack\\host_common\\inc', 
    '..\\src\\config\\default\\lib_src\\ble_stack\\sys\\inc\\', 
    '..\\src\\config\\default\\lib_src\\ble_stack\\sys\\inc\\internal\\platform\\chimera', 
    '..\\src\\config\\default\\ble\\',
    ]


ble_stack_macros = [
    'PLATFORM=PLATFORM_CHIMERA',
    '_USE_NON_PUBLIC_',
    '__FPGA__',
    '__Chimera__',
    'DEBUG_BUILD',
    'BT_EMBEDDED_MODE',
    ]


add_ble_lib_files(libBLEStackComponent, controller_ble_src, 'SOURCE', 'controller_ble', configName)
add_ble_lib_files(libBLEStackComponent, controller_ble_header, 'HEADER', 'controller_ble', configName)
add_ble_lib_files(libBLEStackComponent, host_ble_src, 'SOURCE', 'host_ble', configName)
add_ble_lib_files(libBLEStackComponent, host_ble_header, 'HEADER', 'host_ble', configName)
add_ble_lib_files(libBLEStackComponent, host_common_src, 'SOURCE', 'host_common', configName)
add_ble_lib_files(libBLEStackComponent, host_common_header, 'HEADER', 'host_common', configName)
add_ble_lib_files(libBLEStackComponent, sys_src, 'SOURCE', 'sys', configName)
add_ble_lib_files(libBLEStackComponent, sys_header, 'HEADER', 'sys', configName)

#add_lib_util_files(libBLEStackComponent, utils_source_files, 'SOURCE', 'utils', configName)
add_lib_util_files(libBLEStackComponent, utils_header_files, 'HEADER', 'utils', configName)


addExtraIncludes(libBLEStackComponent, ble_stack_includes)
addPreprocessorMacros(libBLEStackComponent, ble_stack_macros)


# The initial H3 ble_stack source compiles with many warnings.  To allow
# the compile to continue, the XC32 setting 'make-warnings-into-errors'
# must be turned off.
setConfigItemC32(libBLEStackComponent, 'make-warnings-into-errors', 'false')

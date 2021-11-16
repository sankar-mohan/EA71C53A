###############################################################################
######################### BLE Stack Configurations ############################
###############################################################################

from binascii import hexlify

gapDevid = 0
gapGain = 3
gapEtsi = True
################################################################################
#### Component ####
################################################################################
def gapGenDefTxPwr(isLimit):
    global gapDevid

    if gapDevid == 451:
        if isLimit:
            return 11
        else:
            return 15
    else:
        return 9

def gapGenMaxTxPwr(isLimit):
    global gapDevid
    global gapGain

    if gapDevid == 451:
        if isLimit:
            return min(12 + gapGain, 11)
        else:
            return 12 + gapGain
    else:
        if isLimit:
            return min(6 + gapGain, 11)
        else:
            return 6 + gapGain

def gapGenMinTxPwr():
    global gapDevid
    global gapGain
        
    if gapDevid == 451:
        return -29 + gapGain
    else:
        return -25 + gapGain

def bleDefaultVisibility(symbol, event):
    symbol.setVisible(event["value"])
    
def bleScmEnable(symbol, event):
    if event["value"] == False:
        symbol.setEnabled(False)
    else:
        if event["id"] == "BOOL_GCM_SCM":
            symbol.setEnabled(True)
        else:
            symbol.setEnabled(symbol.getComponent().getSymbolByID("BOOL_GCM_SCM").getValue())

    
def gcmVisible(symbol, event):
    symbol.setVisible(event["value"])

def appTrpsEnable(symbol, event):
    #print("blestack::appTrpsEnable: symbol.ID={}".format(symbol.ID))

    # generic method to get non-local data, read database
    trp_bool_server = Database.getSymbolValue("profileBLE_TRP", "TRP_BOOL_SERVER")
    #print("blestack::appTrpsEnable: trp_bool_server = {}".format('True' if trp_bool_server else 'False'))

    # event specific method to get data use "value" element of event
    #print("blestack::appTrpsEnable: trp_bool_server = {}".format('True' if event["value"] else 'False'))

    # now change the dependent symbol's value to match the remote value
    symbol.setValue(trp_bool_server)

def appTrpcEnable(symbol, event):
    #print("blestack::appTrpcEnable: symbol.ID={}".format(symbol.ID))

    # generic method to get non-local data, read database
    trp_bool_client = Database.getSymbolValue("profileBLE_TRP", "TRP_BOOL_CLIENT")
    #print("blestack::appTrpcEnable: trp_bool_client = {}".format('True' if trp_bool_client else 'False'))

    # event specific method to get data use "value" element of event
    #print("blestack::appTrpcEnable: trp_bool_client = {}".format('True' if event["value"] else 'False'))

    # now change the dependent symbol's value to match the remote value
    symbol.setValue(trp_bool_client)
    gattClientRole.setValue(trp_bool_client)

def appTrcbpsEnable(symbol, event):
    #print("blestack::appTrcbpcEnable: symbol.ID={}".format(symbol.ID))

    # generic method to get non-local data, read database
    trcp_bool_server = Database.getSymbolValue("profileBLE_TRCP", "TRCP_BOOL_SERVER")
    #print("blestack::appTrcbpcEnable: trcp_bool_server = {}".format('True' if trcp_bool_server else 'False'))

    # event specific method to get data use "value" element of event
    #print("blestack::appTrcbpcEnable: trcp_bool_server = {}".format('True' if event["value"] else 'False'))

    # now change the dependent symbol's value to match the remote value
    symbol.setValue(trcp_bool_server)
    
def bleDdEnable(symbol, event):
    bleDdSourceFile.setEnabled(event["value"])

def gapPrivVisibility(symbol, event):
    symbol.setVisible(event["value"])

def gapAdvVisibility(symbol, event):
    symbol.setVisible(event["value"])
    
def gapExtAdvTxPwr(symbol, event):
    if event["id"] == "GAP_EXT_ADV_ADV_SET_2":
        symbol.setVisible(event["value"])
        return

    global gapEtsi
    symbol.setMax(gapGenMaxTxPwr(gapEtsi and event["value"] < 3))
    symbol.setMin(gapGenMinTxPwr())
        
def gapLegacyAdvVisibility(symbol, event):
    global gapExtAdvEn
    global gapAdvertising
    symbol.setVisible(not gapExtAdvEn.getValue() and gapAdvertising.getValue())

def gapExtAdvEvtPropVisibility(symbol, event):
    symbol.setVisible(not event["value"] < 3)

def gapScanVisibility(symbol, event):
    symbol.setVisible(event["value"])

def gapExtScanPhy1mVisibility(symbol, event):
    global gapScanPhy
    symbol.setVisible(gapScanPhy.getValue() == 0 or gapScanPhy.getValue() == 2)

def gapExtScanPhyCodedVisibility(symbol, event):
    global gapScanPhy
    symbol.setVisible(gapScanPhy.getValue() == 1 or gapScanPhy.getValue() == 2)


def gapLegacyScanVisibility(symbol, event):
    global gapExtScanEn
    global gapScan
    symbol.setVisible(not gapExtScanEn.getValue() and gapScan.getValue())


def gattManuConfirmVisibility(symbol, event):
    symbol.setVisible(event["value"])

def smpSCOnlyVisibility(symbol, event):
    symbol.setVisible(event["value"])

def dmConnVisibility(symbol, event):
    symbol.setVisible(event["value"])

def getValueLen(intValue):

    # convert int to hex and pad to full bytes
    hexStr = '{:X}'.format(intValue)
    hexStrLen = len(hexStr)
    if hexStrLen % 2 == 1:
        # odd number of hex nibbles - add leading 0 to pad value
        hexStr = '0' + hexStr
        hexStrLen += 1

    totalLength = (hexStrLen >> 1)    # length byte + hexStr length
    #print('hexStrlen = {}, totalLength = {}'.format(hexStrLen, totalLength))
    return totalLength

def formatInt2HexBytes(intValue, byteList, chkLen):

    # convert int to hex and pad to full bytes
    hexStr = '{:X}'.format(intValue)
    hexStrLen = len(hexStr)
    if hexStrLen % 2 == 1:
        # odd number of hex nibbles - add leading 0 to pad value
        hexStr = '0' + hexStr
        hexStrLen += 1

    if chkLen != 0 :
        if hexStrLen/2 < chkLen:
            pedLen = chkLen - hexStrLen/2
            for i in range(pedLen):
                hexStr = '0' + '0' + hexStr
                hexStrLen += 2        
    # add length
    #print('hexStrlen = {}'.format(hexStrLen))

    # add hex Str hex bytes as formatted bytes to byteList
    for byte in range(0, hexStrLen, 2):
        byteList.append('0x' + hexStr[byte:byte+2])
        
def formatInt2HexBytesInvert(intValue, byteList, chkLen):

    # convert int to hex and pad to full bytes
    hexStr = '{:X}'.format(intValue)
    hexStrLen = len(hexStr)
    if hexStrLen % 2 == 1:
        # odd number of hex nibbles - add leading 0 to pad value
        hexStr = '0' + hexStr
        hexStrLen += 1

    if chkLen != 0 :
        if hexStrLen/2 < chkLen:
            pedLen = chkLen - hexStrLen/2
            for i in range(pedLen):
                hexStr = '0' + '0' + hexStr
                hexStrLen += 2        
    # add length
    #print('hexStrlen = {}'.format(hexStrLen))

    # add hex Str hex bytes as formatted bytes to byteList
    for byte in range(0, hexStrLen, 2):
        byteList.append('0x' + hexStr[hexStrLen-byte-2:hexStrLen-byte])
        
def gapGenExtAdvData(symbol):
    if -1 == symbol.getID().find("_2"):
        gapAdvDataFlagEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_FLAG_EN")
        gapAdvDataFlag = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_FLAG")
        gapAdvDataLocalNameEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LOCAL_NAME_EN")
        gapAdvDataLocalName = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LOCAL_NAME")
        gapAdvDataSvcDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_DATA_EN")
        gapAdvDataSvcData = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_DATA")
        gapAdvDataSvcUUID = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_UUID")
        gapAdvDataManuSpecDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD_EN")
        gapAdvDataManuSpecData = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD")
        gapAdvDataManuSpecDataCid = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD_CID")
        gapAdvDataUserDefEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_UDD_EN")
        gapAdvDataUserDef = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_UDD")
        gapAdvertisingDataLen = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LEN")
        gapAdvertisingDataValue = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_VALUE")
    else:
        gapAdvDataFlagEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_FLAG_EN_2")
        gapAdvDataFlag = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_FLAG_2")
        gapAdvDataLocalNameEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LOCAL_NAME_EN_2")
        gapAdvDataLocalName = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LOCAL_NAME_2")
        gapAdvDataSvcDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2")
        gapAdvDataSvcData = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_DATA_2")
        gapAdvDataSvcUUID = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_SERVICE_UUID_2")
        gapAdvDataManuSpecDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD_EN_2")
        gapAdvDataManuSpecData = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD_2")
        gapAdvDataManuSpecDataCid = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_MSD_CID_2")
        gapAdvDataUserDefEn = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_UDD_EN_2")
        gapAdvDataUserDef = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_UDD_2")
        gapAdvertisingDataLen = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_LEN_2")
        gapAdvertisingDataValue = symbol.getComponent().getSymbolByID("GAP_EXT_ADV_DATA_VALUE_2")
        
    advDataList = []    # array of formatted hex bytes
    advDataLength   = 0 # total number of bytes of advDataList
    
    if gapAdvDataFlagEn.getValue() == True:
        totalFlagLen=getValueLen(int(gapAdvDataFlag.getSelectedKey(), 16))+1
        advDataList.append('0x{:02X}'.format(totalFlagLen))
        advDataList.append('0x01')
        
        # get the Flag Ad Type value
        formatInt2HexBytes(int(gapAdvDataFlag.getSelectedKey(), 16), advDataList, 0)
        advDataLength+=totalFlagLen+1

    if gapAdvDataLocalNameEn.getValue() == True:
        # get the local name string and convert to ASCII hex
        localNameHex = hexlify(gapAdvDataLocalName.getValue()).upper()

        # add length
        localNameHexLen = (len(localNameHex) >> 1) + 1   # name length in bytes
        advDataLength += localNameHexLen        # length byte + name length

        # add formatted hex length byte to advDataList
        advDataList.append('0x{:02X}'.format(localNameHexLen))

        advDataList.append('0x09')  # some hex key
        advDataLength += 1          # add key byte to length

        # add hex name string as formatted bytes to advDataList
        for byte in range(0, len(localNameHex), 2):
            advDataList.append('0x' + localNameHex[byte:byte+2])

    if gapAdvDataSvcDataEn.getValue() == True:
        totalSvcDataLen=getValueLen(gapAdvDataSvcUUID.getValue())+1
        totalSvcDataLen+=getValueLen(gapAdvDataSvcData.getValue())
        advDataList.append('0x{:02X}'.format(totalSvcDataLen))
        advDataList.append('0x16')

        # get uuid as integer
        formatInt2HexBytesInvert(gapAdvDataSvcUUID.getValue(), advDataList, 2)

        # get svcData as integer
        formatInt2HexBytes(gapAdvDataSvcData.getValue(), advDataList, 0)

        advDataLength+=totalSvcDataLen+1            

    if gapAdvDataManuSpecDataEn.getValue() == True:
        totalMsdLen=getValueLen(gapAdvDataManuSpecDataCid.getValue())+1
        totalMsdLen+=getValueLen(gapAdvDataManuSpecData.getValue())
        advDataList.append('0x{:02X}'.format(totalMsdLen))         
        advDataList.append('0xff')  # some hex key 

        formatInt2HexBytes(gapAdvDataManuSpecDataCid.getValue(), advDataList, 2)
        formatInt2HexBytes(gapAdvDataManuSpecData.getValue(), advDataList, 0)
        
        advDataLength+=totalMsdLen+1

    if gapAdvDataUserDefEn.getValue() == True:
        totalUsdLen=getValueLen(gapAdvDataUserDef.getValue())+1
        formatInt2HexBytes(gapAdvDataUserDef.getValue(), advDataList, 0)
        advDataLength+=totalUsdLen
    

    #for e in advDataList:
    #    print('advDataList: {}'.format(e))

    symbol.setValue(', '.join(advDataList))
    gapAdvertisingDataLen.setValue(advDataLength)
    gapAdvertisingDataValue.setLabel(symbol.getValue())
    #print('gapSetAdvData advData = {}'.format(symbol.getValue()))
    #print('gapSetAdvData advDataLength = {}'.format(advDataLength))
        
def gapSetExtAdvData(symbol, event):
    #print('gapSetAdvData symbol.getType = ' + symbol.getType())
    #print('gapSetAdvData event.id       = ' + event["id"])
    #print('gapSetAdvData event.value    = ' + str(event["value"]))
    
    gapGenExtAdvData(symbol)

def gapGenExtScanRspData(symbol):
    if -1 == symbol.getID().find("_2"):
        gapScanRspDataLocalNameEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN")
        gapScanRspDataLocalName = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME")
        gapScanRspDataSvcDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN")
        gapScanRspDataSvcData = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA")
        gapScanRspDataSvcUUID = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID")
        gapScanRspDataManuSpecDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD_EN")
        gapScanRspDataManuSpecData = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD")
        gapScanRspDataManuSpecDataCid = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD_CID")
        gapScanRspDataUserDefEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_UDD_EN")
        gapScanRspDataUserDef = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_UDD")
        gapScanRspDataLen = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LEN")
        gapScanRspDataValue = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_VALUE")
    else:
        gapScanRspDataLocalNameEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN_2")
        gapScanRspDataLocalName = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_2")
        gapScanRspDataSvcDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2")
        gapScanRspDataSvcData = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_2")
        gapScanRspDataSvcUUID = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID_2")
        gapScanRspDataManuSpecDataEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD_EN_2")
        gapScanRspDataManuSpecData = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD_2")
        gapScanRspDataManuSpecDataCid = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_MSD_CID_2")
        gapScanRspDataUserDefEn = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_UDD_EN_2")
        gapScanRspDataUserDef = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_UDD_2")
        gapScanRspDataLen = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_LEN_2")
        gapScanRspDataValue = symbol.getComponent().getSymbolByID("GAP_EXT_SCAN_RSP_DATA_VALUE_2")
    # build the GAP_SCAN_RSP_DATA string here
    ''' If a GAP data element is enabled, add the formatted hex data.
        The format used is;
            xx  - data length
            xx  - AD type            
            xx..n   - data
    '''
    scanRspDataList = []    # array of formatted hex bytes
    scanRspDataLength   = 0 # total number of bytes of advDataList
    
    if gapScanRspDataLocalNameEn.getValue() == True:
        # get the local name string and convert to ASCII hex
        localNameHex = hexlify(gapScanRspDataLocalName.getValue()).upper()

        # add length
        localNameHexLen = (len(localNameHex) >> 1) + 1   # name length in bytes
        scanRspDataLength += localNameHexLen        # length byte + name length

        # add formatted hex length byte to scanRspDataList
        scanRspDataList.append('0x{:02X}'.format(localNameHexLen))

        scanRspDataList.append('0x09')  # some hex key
        scanRspDataLength += 1          # add key byte to length

        # add hex name string as formatted bytes to advDataList
        for byte in range(0, len(localNameHex), 2):
            scanRspDataList.append('0x' + localNameHex[byte:byte+2])
    
    if gapScanRspDataSvcDataEn.getValue() == True:
        totalSvcDataLen=getValueLen(gapScanRspDataSvcUUID.getValue())+1
        totalSvcDataLen+=getValueLen(gapScanRspDataSvcData.getValue())
        scanRspDataList.append('0x{:02X}'.format(totalSvcDataLen))
        scanRspDataList.append('0x16')

        # get uuid as integer
        formatInt2HexBytesInvert(gapScanRspDataSvcUUID.getValue(), scanRspDataList, 2)

        # get svcData as integer
        formatInt2HexBytes(gapScanRspDataSvcData.getValue(), scanRspDataList, 0)

        scanRspDataLength+=totalSvcDataLen+1            

    if gapScanRspDataManuSpecDataEn.getValue() == True:
        totalMsdLen=getValueLen(gapScanRspDataManuSpecDataCid.getValue())+1
        totalMsdLen+=getValueLen(gapScanRspDataManuSpecData.getValue())
        scanRspDataList.append('0x{:02X}'.format(totalMsdLen))         
        scanRspDataList.append('0xff')  # some hex key 

        formatInt2HexBytes(gapScanRspDataManuSpecDataCid.getValue(), scanRspDataList, 2)
        formatInt2HexBytes(gapScanRspDataManuSpecData.getValue(), scanRspDataList, 0)
        
        scanRspDataLength+=totalMsdLen+1

    if gapScanRspDataUserDefEn.getValue() == True:
        totalUsdLen=getValueLen(gapScanRspDataUserDef.getValue())+1
        formatInt2HexBytes(gapScanRspDataUserDef.getValue(), scanRspDataList, 0)
        scanRspDataLength+=totalUsdLen

    #for e in scanRspDataList:
        #print('scanRspDataList: {}'.format(e))

    symbol.setValue(', '.join(scanRspDataList))
    gapScanRspDataLen.setValue(scanRspDataLength)
    gapScanRspDataValue.setLabel(symbol.getValue())
    #print('gapSetScanRspData advData = {}'.format(symbol.getValue()))
    #print('gapSetScanRspData advDataLength = {}'.format(scanRspDataLength))
        
def gapSetExtScanRspData(symbol, event):
    #print('gapSetScanRspData symbol.getType = ' + symbol.getType())
    #print('gapSetScanRspData event.id       = ' + event["id"])
    #print('gapSetScanRspData event.value    = ' + str(event["value"]))
    
    gapGenExtScanRspData(symbol)


def gapGenAdvData(symbol):
    global gapAdvDataFlagEn 
    global gapAdvDataFlag 
    global gapAdvDataLocalNameEn 
    global gapAdvDataLocalName 
    global gapAdvDataSvcDataEn 
    global gapAdvDataSvcUUID 
    global gapAdvDataSvcData 
    global gapAdvDataManuSpecDataEn 
    global gapAdvDataManuSpecData 
    global gapAdvDataUserDefEn 
    global gapAdvDataUserDef 
    global gapAdvertisingDataValue
    global gapAdvertisingDataLen

    # build the GAP_ADV_DATA string here
    ''' If a GAP data element is enabled, add the formatted hex data.
        The format used is;
            xx  - data length
            xx  - AD type            
            xx..n   - data
    '''
    advDataList = []    # array of formatted hex bytes
    advDataLength   = 0 # total number of bytes of advDataList

    if gapAdvDataFlagEn.getValue() == True:
        totalFlagLen=getValueLen(int(gapAdvDataFlag.getSelectedKey(), 16))+1
        advDataList.append('0x{:02X}'.format(totalFlagLen))
        advDataList.append('0x01')
        
        # get the Flag Ad Type value
        formatInt2HexBytes(int(gapAdvDataFlag.getSelectedKey(), 16), advDataList, 0)
        advDataLength+=totalFlagLen+1

    if gapAdvDataLocalNameEn.getValue() == True:
        # get the local name string and convert to ASCII hex
        localNameHex = hexlify(gapAdvDataLocalName.getValue()).upper()

        # add length
        localNameHexLen = (len(localNameHex) >> 1) + 1   # name length in bytes
        advDataLength += localNameHexLen        # length byte + name length

        # add formatted hex length byte to advDataList
        advDataList.append('0x{:02X}'.format(localNameHexLen))

        advDataList.append('0x09')  # some hex key
        advDataLength += 1          # add key byte to length

        # add hex name string as formatted bytes to advDataList
        for byte in range(0, len(localNameHex), 2):
            advDataList.append('0x' + localNameHex[byte:byte+2])

    if gapAdvDataSvcDataEn.getValue() == True:
        totalSvcDataLen=getValueLen(gapAdvDataSvcUUID.getValue())+1
        totalSvcDataLen+=getValueLen(gapAdvDataSvcData.getValue())
        advDataList.append('0x{:02X}'.format(totalSvcDataLen))
        advDataList.append('0x16')

        # get uuid as integer
        formatInt2HexBytesInvert(gapAdvDataSvcUUID.getValue(), advDataList, 2)

        # get svcData as integer
        formatInt2HexBytes(gapAdvDataSvcData.getValue(), advDataList, 0)

        advDataLength+=totalSvcDataLen+1            

    if gapAdvDataManuSpecDataEn.getValue() == True:
        totalMsdLen=getValueLen(gapAdvDataManuSpecDataCid.getValue())+1
        totalMsdLen+=getValueLen(gapAdvDataManuSpecData.getValue())
        advDataList.append('0x{:02X}'.format(totalMsdLen))         
        advDataList.append('0xff')  # some hex key 

        formatInt2HexBytes(gapAdvDataManuSpecDataCid.getValue(), advDataList, 2)
        formatInt2HexBytes(gapAdvDataManuSpecData.getValue(), advDataList, 0)
        
        advDataLength+=totalMsdLen+1

    if gapAdvDataUserDefEn.getValue() == True:
        totalUsdLen=getValueLen(gapAdvDataUserDef.getValue())+1
        formatInt2HexBytes(gapAdvDataUserDef.getValue(), advDataList, 0)
        advDataLength+=totalUsdLen


    #for e in advDataList:
    #    print('advDataList: {}'.format(e))

    symbol.setValue(', '.join(advDataList))
    gapAdvertisingDataLen.setValue(advDataLength)
    gapAdvertisingDataValue.setLabel(symbol.getValue())
    #print('gapSetAdvData advData = {}'.format(symbol.getValue()))
    #print('gapSetAdvData advDataLength = {}'.format(advDataLength))


def gapSetAdvData(symbol, event):
    #print('gapSetAdvData symbol.getType = ' + symbol.getType())
    #print('gapSetAdvData event.id       = ' + event["id"])
    #print('gapSetAdvData event.value    = ' + str(event["value"]))
    
    gapGenAdvData(symbol)

def gapGenScanRspData(symbol):
    global gapScanRspDataSvcDataEn 
    global gapScanRspDataSvcUUID 
    global gapScanRspDataSvcData 
    global gapScanRspDataManuSpecDataEn 
    global gapScanRspDataManuSpecData 
    global gapScanRspDataUserDefEn 
    global gapScanRspDataUserDef 
    global gapScanRspDataValue
    global gapScanRspDataLen

    # build the GAP_SCAN_RSP_DATA string here
    ''' If a GAP data element is enabled, add the formatted hex data.
        The format used is;
            xx  - data length
            xx  - AD type            
            xx..n   - data
    '''
    scanRspDataList = []    # array of formatted hex bytes
    scanRspDataLength   = 0 # total number of bytes of advDataList

    if symbol.getComponent().getSymbolValue("GAP_SCAN_RSP_DATA_LOCAL_NAME_EN") == True:
        # get the local name string and convert to ASCII hex
        localNameHex = hexlify(symbol.getComponent().getSymbolValue("GAP_SCAN_RSP_DATA_LOCAL_NAME")).upper()

        # add length
        localNameHexLen = (len(localNameHex) >> 1) + 1   # name length in bytes
        scanRspDataLength += localNameHexLen        # length byte + name length

        # add formatted hex length byte to scanRspDataList
        scanRspDataList.append('0x{:02X}'.format(localNameHexLen))

        scanRspDataList.append('0x09')  # some hex key
        scanRspDataLength += 1          # add key byte to length

        # add hex name string as formatted bytes to advDataList
        for byte in range(0, len(localNameHex), 2):
            scanRspDataList.append('0x' + localNameHex[byte:byte+2])
    
    if gapScanRspDataSvcDataEn.getValue() == True:
        totalSvcDataLen=getValueLen(gapScanRspDataSvcUUID.getValue())+1
        totalSvcDataLen+=getValueLen(gapScanRspDataSvcData.getValue())
        scanRspDataList.append('0x{:02X}'.format(totalSvcDataLen))
        scanRspDataList.append('0x16')

        # get uuid as integer
        formatInt2HexBytesInvert(gapScanRspDataSvcUUID.getValue(), scanRspDataList, 2)

        # get svcData as integer
        formatInt2HexBytes(gapScanRspDataSvcData.getValue(), scanRspDataList, 0)

        scanRspDataLength+=totalSvcDataLen+1            

    if gapScanRspDataManuSpecDataEn.getValue() == True:
        totalMsdLen=getValueLen(gapScanRspDataManuSpecDataCid.getValue())+1
        totalMsdLen+=getValueLen(gapScanRspDataManuSpecData.getValue())
        scanRspDataList.append('0x{:02X}'.format(totalMsdLen))         
        scanRspDataList.append('0xff')  # some hex key 

        formatInt2HexBytes(gapScanRspDataManuSpecDataCid.getValue(), scanRspDataList, 2)
        formatInt2HexBytes(gapScanRspDataManuSpecData.getValue(), scanRspDataList, 0)
        
        scanRspDataLength+=totalMsdLen+1

    if gapScanRspDataUserDefEn.getValue() == True:
        totalUsdLen=getValueLen(gapScanRspDataUserDef.getValue())+1
        formatInt2HexBytes(gapScanRspDataUserDef.getValue(), scanRspDataList, 0)
        scanRspDataLength+=totalUsdLen

    #for e in scanRspDataList:
        #print('scanRspDataList: {}'.format(e))

    symbol.setValue(', '.join(scanRspDataList))
    gapScanRspDataLen.setValue(scanRspDataLength)
    gapScanRspDataValue.setLabel(symbol.getValue())
    #print('gapSetScanRspData advData = {}'.format(symbol.getValue()))
    #print('gapSetScanRspData advDataLength = {}'.format(scanRspDataLength))

def gapSetScanRspData(symbol, event):
    #print('gapSetScanRspData symbol.getType = ' + symbol.getType())
    #print('gapSetScanRspData event.id       = ' + event["id"])
    #print('gapSetScanRspData event.value    = ' + str(event["value"]))

    gapGenScanRspData(symbol)

def sleepEnabledChange(symbol, event):
    value = event["value"]
    Log.writeInfoMessage('BLEStack:sleepEnabledChange value={}'.format(str(value)))
    sendRTCSupportMessage(value)

def sendRTCSupportMessage(rtcRequiredValue):
    Database.sendMessage("pic32cx_bz2_devsupport", "RTC_SUPPORT", {"target": "pic32cx_bz2_devsupport",
                                                 "source": "libBLEStack",
                                                 "rtcRequired": rtcRequiredValue})

def handleMessage(messageID, args):
    # Log.writeInfoMessage('BLEStack:handleMessage ID={} argLen={}'.format(messageID, len(args)))
    ''' This message handler is designed to process messages
        sent from the driver/pic32cx-bz/config/device_support.py
        script.
    '''
    if(messageID == 'ANTENNA_GAIN_CHANGE'):
        component = Database.getComponentByID(args['target'])
        customGainEnabled = component.getSymbolByID('USE_CUSTOM_ANT_GAIN')
        customGainValue = component.getSymbolByID('CUSTOM_ANT_GAIN')
        customRegion = component.getSymbolByID('CUSTOM_ANT_REGION')
        Log.writeInfoMessage('{:<17}: Handling - target={}'.format('blestack.py', args['target']))
        for arg in args:
            Log.writeInfoMessage('{:<17}: {}: {}'.format('', arg, args[arg]))
            if('CUSTOM_ANT_ENABLE' == arg):
                customGainEnabled.setValue(args[arg])
            if('CUSTOM_ANT_GAIN' == arg):
                customGainValue.setValue(args[arg])
            if('CUSTOM_ANT_REGION' == arg):
                customRegion.setValue(args[arg])
                
        # update tx power symbols
        global gapGain
        global gapEtsi
        
        if customGainEnabled.getValue() == True:
            gapGain = customGainValue.getValue()
        else:
            gapGain = 3
            
        gapEtsi = customRegion.getValue().find('ETSI') != -1

        gapTxPwr = component.getSymbolByID('GAP_CONN_TX_PWR')
        gapTxPwr.setMax(gapGenMaxTxPwr(False))
        gapTxPwr.setMin(gapGenMinTxPwr())
        
        gapTxPwr = component.getSymbolByID('GAP_ADV_TX_PWR')
        gapTxPwr.setMax(gapGenMaxTxPwr(gapEtsi))
        gapTxPwr.setMin(gapGenMinTxPwr())
        
        global gapExtAdvEvtProp
        global gapExtAdvEvtProp2
        
        gapTxPwr = component.getSymbolByID('GAP_EXT_ADV_TX_POWER')
        gapTxPwr.setMax(gapGenMaxTxPwr(gapEtsi and gapExtAdvEvtProp.getValue() < 3))
        gapTxPwr.setMin(gapGenMinTxPwr())
        
        gapTxPwr = component.getSymbolByID('GAP_EXT_ADV_TX_POWER_2')
        gapTxPwr.setMax(gapGenMaxTxPwr(gapEtsi and gapExtAdvEvtProp2.getValue() < 3))
        gapTxPwr.setMin(gapGenMinTxPwr())


def instantiateComponent(libBLEStackComponent):
    global gapAdvDataFlag 
    global gapAdvDataFlagEn 
    global gapAdvDataLocalNameEn 
    global gapAdvDataLocalName 
    global gapAdvDataSvcDataEn 
    global gapAdvDataSvcUUID 
    global gapAdvDataSvcData 
    global gapAdvDataManuSpecDataEn 
    global gapAdvDataManuSpecDataCid
    global gapAdvDataManuSpecData 
    global gapAdvDataUserDefEn 
    global gapAdvDataUserDef 
    global gapAdvertisingDataValue
    global gapAdvertisingDataLen
    global gapScanRspDataSvcDataEn 
    global gapScanRspDataSvcUUID 
    global gapScanRspDataSvcData 
    global gapScanRspDataManuSpecDataEn 
    global gapScanRspDataManuSpecDataCid
    global gapScanRspDataManuSpecData 
    global gapScanRspDataUserDefEn 
    global gapScanRspDataUserDef 
    global gapScanRspDataValue
    global gapScanRspDataLen    
    global gapExtAdvEn
    global gapAdvertising
    global gapExtScanEn
    global gapScan
    global gapScanPhy
    global gapExtAdvEvtProp
    global gapExtAdvEvtProp2
    
    global gapDevid
    global gapEtsi

    print('BLEStackComponent')
    configName = Variables.get('__CONFIGURATION_NAME')
    processor = Variables.get("__PROCESSOR")

    print('HarmonyCore.ENABLE_APP_FILE = {}'.format(str(Database.getSymbolValue("HarmonyCore", 'ENABLE_APP_FILE'))))
    print('HarmonyCore.ENABLE_OSAL = {}'.format(str(Database.getSymbolValue("HarmonyCore", 'ENABLE_OSAL'))))
    print('FreeRTOS.FREERTOS_USE_QUEUE_SETS = {}'.format(str(Database.getSymbolValue("FreeRTOS", 'FREERTOS_USE_QUEUE_SETS'))))

    print('Config Name: {} processor: {}'.format(configName, processor))

    # inform H3 core that BLE has been loaded
    Database.setSymbolValue("core", "BLE_CLOCK_ENABLE", True)


    if processor == 'WBZ450' or processor == 'PIC32CX1012BZ24032':
        gapDevid = 450
    else:
        gapDevid = 451
        
    if Database.getComponentByID("pic32cx_bz2_devsupport") and Database.getSymbolValue("pic32cx_bz2_devsupport","CUSTOM_ANT_REGION").find('ETSI') == -1:
        gapEtsi = False
    else:
        gapEtsi = True


    ############################################################################
    ### Execute additional H3 Python code
    ############################################################################
    #Load ble stack whole source package
    #execfile(Module.getPath() + "/driver/ble/config/ble_stack_load_src.py")
    
    #Load ble stack header files when adding ble_stack_lib.a
    execfile(Module.getPath() + "/driver/ble/config/ble_stack_load_lib.py")



    ############################################################################
    ### Add ble_stack library files
    ############################################################################

    # Add ble_stack_lib.a library
    ble_stack_lib_a = libBLEStackComponent.createLibrarySymbol(None, None)
    ble_stack_lib_a.setDestPath('ble/lib')
    ble_stack_lib_a.setSourcePath('driver/ble/src/ble_stack_lib.a')
    ble_stack_lib_a.setOutputName('ble_stack.a')

    #################################################################
    ###############  System Initialization Settings   ###############
    #################################################################
    bleInitTemplates = [('sysInitData', 'system_initialize_data.c.ftl', 'core.LIST_SYSTEM_INIT_C_LIBRARY_INITIALIZATION_DATA'),
        ('sysInitstart', 'system_initialize_start.c.ftl', 'core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_START'),
        ('sysInitMiddleware', 'system_initialize_middleware.c.ftl', 'core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE'),
        ('sysTaskMiddlewareDef', 'system_tasks_def.c.ftl', 'core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS'),
        ('sysTaskMiddleware', 'system_tasks.c.ftl', 'core.LIST_SYSTEM_TASKS_C_CALL_LIB_TASKS'),
        ('sysDefZB', 'system_definitions.h.ftl', 'core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES'),
        ]

    n = 0
    bleInit = []
    for i, f, p in bleInitTemplates:
        bleInit.append( libBLEStackComponent.createFileSymbol(i, None))
        bleInit[n].setType('STRING')
        bleInit[n].setOutputName(p)
        bleInit[n].setSourcePath('driver/ble/templates/system/' + f)
        bleInit[n].setMarkup(True)
        print('{} file: {} pos: {}'.format(n, f, p))
        n = n + 1


    #################################################################
    ########  app_ble.c app_ble.h app_ble_handler.c files ###########
    #################################################################
    # Add app_ble.c - generated file
    app_bleSourceFile = libBLEStackComponent.createFileSymbol(None, None)
    app_bleSourceFile.setSourcePath('driver/ble/templates/app_ble.c.ftl')
    app_bleSourceFile.setOutputName('app_ble.c')
    app_bleSourceFile.setOverwrite(True)
    app_bleSourceFile.setDestPath('../../app_ble')
    app_bleSourceFile.setProjectPath('app_ble')
    app_bleSourceFile.setType('SOURCE')
    app_bleSourceFile.setEnabled(True)
    app_bleSourceFile.setMarkup(True)

    # Add app_ble.h - static file
    app_bleHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    app_bleHeaderFile.setSourcePath('driver/ble/src/app_ble.h')
    app_bleHeaderFile.setOutputName('app_ble.h')
    app_bleHeaderFile.setOverwrite(True)
    app_bleHeaderFile.setDestPath('../../app_ble')
    app_bleHeaderFile.setProjectPath('app_ble')
    app_bleHeaderFile.setType('HEADER')
    app_bleHeaderFile.setEnabled(True)

    # Add app_ble_handler.h - static file
    app_bleHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    app_bleHeaderFile.setSourcePath('driver/ble/src/app_ble_handler.h')
    app_bleHeaderFile.setOutputName('app_ble_handler.h')
    app_bleHeaderFile.setOverwrite(True)
    app_bleHeaderFile.setDestPath('../../app_ble')
    app_bleHeaderFile.setProjectPath('app_ble')
    app_bleHeaderFile.setType('HEADER')
    app_bleHeaderFile.setEnabled(True)    

    # Add app_ble.c - generated file
    app_bleHdlSourceFile = libBLEStackComponent.createFileSymbol(None, None)
    app_bleHdlSourceFile.setSourcePath('driver/ble/src/app_ble_handler.c')
    app_bleHdlSourceFile.setOutputName('app_ble_handler.c')
    app_bleHdlSourceFile.setOverwrite(True)
    app_bleHdlSourceFile.setDestPath('../../app_ble')
    app_bleHdlSourceFile.setProjectPath('app_ble')
    app_bleHdlSourceFile.setType('SOURCE')
    app_bleHdlSourceFile.setEnabled(True)
    app_bleHdlSourceFile.setMarkup(True)    

    #################################################################
    ##############  Add BLE static header files       ###############
    #################################################################
    # # Add ble_stack.h
    # HeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    # HeaderFile.setSourcePath('driver/ble/src/ble_lib/ble_stack.h')
    # HeaderFile.setOutputName('ble_stack.h')
    # HeaderFile.setOverwrite(True)
    # HeaderFile.setDestPath('/lib_src/ble')
    # HeaderFile.setProjectPath('config/' + configName + '/lib_tmp/')
    # HeaderFile.setType('HEADER')
    # HeaderFile.setEnabled(True)


    #################################################################
    ##############  Add BLE static source files       ###############
    #################################################################
    # # Add ble_stack.c
    # HeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    # HeaderFile.setSourcePath('driver/ble/src/ble_lib/ble_stack.c')
    # HeaderFile.setOutputName('ble_stack.c')
    # HeaderFile.setOverwrite(True)
    # HeaderFile.setDestPath('/lib_src/ble')
    # HeaderFile.setProjectPath('config/' + configName + '/lib_tmp/')
    # HeaderFile.setType('SOURCE')
    # HeaderFile.setEnabled(True)

    #################################################################
    ##################      BLE Systen Settings       ###############
    #################################################################
    menuBleSys = libBLEStackComponent.createMenuSymbol('BLE_SYS_MENU', None)
    menuBleSys.setLabel('System')
    menuBleSys.setVisible(True)
    menuBleSys.setDescription("BLE System Settings")

    # Sleep Mode setting
    sysSleep = libBLEStackComponent.createBooleanSymbol('BLE_SYS_SLEEP_MODE_EN', menuBleSys)
    sysSleep.setLabel('Sleep Mode Enable')
    sysSleep.setDescription('Enable BLE Sleep Mode')
    sysSleep.setDefaultValue(False)
    sysSleep.setVisible(True)    
    sysSleep.setDependencies(sleepEnabledChange, ["BLE_SYS_SLEEP_MODE_EN"])


    #################################################################
    ##################      GAP Settings              ###############
    #################################################################
    menuGAP = libBLEStackComponent.createMenuSymbol('GAP_MENU', None)
    menuGAP.setLabel('Generic Access Profile (GAP)')
    menuGAP.setVisible(True)
    menuGAP.setDescription("GAP SETTINGS")

    # Device Name
    gapDeviceName = libBLEStackComponent.createStringSymbol("GAP_DEV_NAME", menuGAP)
    gapDeviceName.setLabel("Device Name")
    gapDeviceName.setDefaultValue("Microchip")
    gapDeviceName.setVisible(True)
    gapDeviceName.setReadOnly(False)
    
    # Device Name Write Property
    gapSvc = libBLEStackComponent.createBooleanSymbol('GAP_SVC_DEV_NAME_WRITE', menuGAP)
    gapSvc.setLabel('Enable Device Name Write Property')
    gapSvc.setDescription('Enable to permit remote modify device name')
    gapSvc.setDefaultValue(False)
    gapSvc.setVisible(True)

    # Appearance
    gapSvc = libBLEStackComponent.createHexSymbol('GAP_SVC_APPEARANCE', menuGAP)
    gapSvc.setLabel('Appearance')
    gapSvc.setDescription('Please refer to https://specificationrefs.bluetooth.com/assigned-values/Appearance%20Values.pdf for valid value')
    gapSvc.setDefaultValue(0x0000)
    gapSvc.setMin(0x0000)
    gapSvc.setMax(0xFFFF)
    gapSvc.setVisible(True)
    
    # Peripheral prefered connection parameters
    gapSvc = libBLEStackComponent.createBooleanSymbol('GAP_SVC_PERI_PRE_CP', menuGAP)
    gapSvc.setLabel('Enable Peripheral Prefered Connection Parameters')
    gapSvc.setDefaultValue(False)
    gapSvc.setVisible(True)
    
    # Minimum Connection Interval
    gapSvcPeri = libBLEStackComponent.createIntegerSymbol("GAP_SVC_CP_MIN_CONN_INTERVAL", gapSvc)
    gapSvcPeri.setLabel("Minimum Connection Interval(Unit: 1.25ms)")
    gapSvcPeri.setDescription("Range: 6 to 3200. Value of 0xFFFF indicates no specific minimum.")
    gapSvcPeri.setDefaultValue(32)
    gapSvcPeri.setMin(6)
    gapSvcPeri.setMax(65535)
    gapSvcPeri.setVisible(False)
    gapSvcPeri.setDependencies(bleDefaultVisibility, ["GAP_SVC_PERI_PRE_CP"])
    
    # Maximum Connection Interval
    gapSvcPeri = libBLEStackComponent.createIntegerSymbol("GAP_SVC_CP_MAX_CONN_INTERVAL", gapSvc)
    gapSvcPeri.setLabel("Maximum Connection Interval(Unit: 1.25ms)")
    gapSvcPeri.setDescription("Range: 6 to 3200. Value of 0xFFFF indicates no specific maximum.")
    gapSvcPeri.setDefaultValue(32)
    gapSvcPeri.setMin(6)
    gapSvcPeri.setMax(65535)
    gapSvcPeri.setVisible(False)
    gapSvcPeri.setDependencies(bleDefaultVisibility, ["GAP_SVC_PERI_PRE_CP"])
    
    # Peripheral latency
    gapSvcPeri = libBLEStackComponent.createIntegerSymbol("GAP_SVC_CP_PERI_LATENCY", gapSvc)
    gapSvcPeri.setLabel("Peripheral Latency")
    gapSvcPeri.setDefaultValue(0)
    gapSvcPeri.setMin(0)
    gapSvcPeri.setMax(0x01F3)
    gapSvcPeri.setVisible(False)
    gapSvcPeri.setDependencies(bleDefaultVisibility, ["GAP_SVC_PERI_PRE_CP"])
    
    # Connection Supervision timeout multipilier
    gapSvcPeri = libBLEStackComponent.createIntegerSymbol("GAP_SVC_CP_PERI_SUPERVISION_TIMEOUT", gapSvc)
    gapSvcPeri.setLabel("Connection Supervision Timeout Multiplier(Unit: 10ms)")
    gapSvcPeri.setDescription("Range: 10 to 3200. 65535 indicates no specific value requested.")
    gapSvcPeri.setDefaultValue(600)
    gapSvcPeri.setMin(0xA)
    gapSvcPeri.setMax(0xFFFF)
    gapSvcPeri.setVisible(False)
    gapSvcPeri.setDependencies(bleDefaultVisibility, ["GAP_SVC_PERI_PRE_CP"])


    # Privacy
    gapPrivacy = libBLEStackComponent.createBooleanSymbol('BOOL_GAP_PRIVACY', menuGAP)
    gapPrivacy.setLabel('Enable Privacy')
    gapPrivacy.setDefaultValue(False)
    gapPrivacy.setVisible(True)

    # Privacy Address Type
    gapPriAddrType = libBLEStackComponent.createKeyValueSetSymbol('GAP_PRIV_ADDR_TYPE', gapPrivacy)
    gapPriAddrType.setLabel('Address Type')
    gapPriAddrType.addKey('BLE_GAP_ADDR_TYPE_RANDOM_RESOLVABLE', '0', 'Resolvable Random Address')
    gapPriAddrType.addKey('BLE_GAP_ADDR_TYPE_RANDOM_NON_RESOLVABLE', '1', 'Non-resolvable Random Address')
    gapPriAddrType.setDefaultValue(0)
    gapPriAddrType.setVisible(False)
    gapPriAddrType.setOutputMode('Key')
    gapPriAddrType.setDisplayMode('Description')
    gapPriAddrType.setDescription("Privacy address type")
    gapPriAddrType.setDependencies(gapPrivVisibility, ["BOOL_GAP_PRIVACY"])

    # Privacy Address Timeout
    gapPriAddrTimeout = libBLEStackComponent.createIntegerSymbol('GAP_PRIV_ADDR_TO', gapPrivacy)
    gapPriAddrTimeout.setLabel("Address Timeout(Unit: 1sec)")
    gapPriAddrTimeout.setDefaultValue(900)
    gapPriAddrTimeout.setMax(65535)
    gapPriAddrTimeout.setMin(1)
    gapPriAddrTimeout.setVisible(False)
    gapPriAddrTimeout.setDescription("Privacy address timeout")
    gapPriAddrTimeout.setDependencies(gapPrivVisibility, ["BOOL_GAP_PRIVACY"])

    #####################
    # Advertising
    #####################
    gapAdvertising = libBLEStackComponent.createBooleanSymbol('GAP_ADVERTISING', menuGAP)
    gapAdvertising.setLabel('Advertising')
    gapAdvertising.setDefaultValue(True)
    gapAdvertising.setVisible(True)
    gapAdvertising.setDescription("Advertising Configuration")
    
    #Extended Advertising
    gapExtAdvEn = libBLEStackComponent.createBooleanSymbol('BOOL_GAP_EXT_ADV', gapAdvertising)
    gapExtAdvEn.setLabel('Extended Advertising')
    gapExtAdvEn.setDefaultValue(False)
    gapExtAdvEn.setVisible(True)
    gapExtAdvEn.setDescription("Extended Advertising")
    gapExtAdvEn.setDependencies(gapAdvVisibility, ["GAP_ADVERTISING"])
    
    # Advertising Tx Power
    gapAdvTxPwr = libBLEStackComponent.createIntegerSymbol('GAP_ADV_TX_PWR', gapAdvertising)
    gapAdvTxPwr.setLabel('Advertising Tx Power (dBm)')
    gapAdvTxPwr.setDefaultValue(gapGenDefTxPwr(gapEtsi))
    gapAdvTxPwr.setMax(gapGenMaxTxPwr(gapEtsi))
    gapAdvTxPwr.setMin(gapGenMinTxPwr())
    gapAdvTxPwr.setVisible(True)
    gapAdvTxPwr.setDescription("Advertising Tx Power")
    gapAdvTxPwr.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])
    
    # Advertising Interval Min
    gapAdvIntMin = libBLEStackComponent.createIntegerSymbol("GAP_ADV_INTERVAL_MIN", gapAdvertising)
    gapAdvIntMin.setLabel("Advertising Interval Min(Unit: 0.625ms)")
    gapAdvIntMin.setDefaultValue(32)
    gapAdvIntMin.setMin(1)
    gapAdvIntMin.setMax(65535)
    gapAdvIntMin.setVisible(True)
    gapAdvIntMin.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])

    # Advertising Interval Max
    gapAdvIntMax = libBLEStackComponent.createIntegerSymbol("GAP_ADV_INTERVAL_MAX", gapAdvertising)
    gapAdvIntMax.setLabel("Advertising Interval Max(Unit: 0.625ms)")
    gapAdvIntMax.setDefaultValue(32)
    gapAdvIntMax.setMin(1)
    gapAdvIntMax.setMax(65535)
    gapAdvIntMax.setVisible(True)
    gapAdvIntMax.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])      

    # Advertising Type
    gapAdvType = libBLEStackComponent.createKeyValueSetSymbol('GAP_ADV_TYPE', gapAdvertising)
    gapAdvType.setLabel('Advertising Type')
    gapAdvType.addKey('BLE_GAP_ADV_TYPE_ADV_IND', '0', 'Connectable and scannable undirected advertising')
    gapAdvType.addKey('BLE_GAP_ADV_TYPE_ADV_SCAN_IND', '2', 'Scannable undirected advertising')
    gapAdvType.addKey('BLE_GAP_ADV_TYPE_ADV_NONCONN_IND', '3', 'Non connectable undirected advertising')
    gapAdvType.setDefaultValue(0)
    gapAdvType.setVisible(True)
    gapAdvType.setOutputMode('Key')
    gapAdvType.setDisplayMode('Description')
    gapAdvType.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])    
 
    # Advertising Channel Map
    gapAdvChannelMap = libBLEStackComponent.createKeyValueSetSymbol('GAP_CHANNEL_MAP', gapAdvertising)
    gapAdvChannelMap.setLabel('Advertising Channel Map')
    gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_ALL', '0', 'Channel 37, 38 and 39')
    gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_38', '1', 'Channel 37, and 38')
    gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38|BLE_GAP_ADV_CHANNEL_39', '2', 'Channel 38, and 39')
    gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_39', '3', 'Channel 37, and 39')
    gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37', '4', 'Channel 37 Only')
    gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38', '5', 'Channel 38 Only')
    gapAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_39', '6', 'Channel 39 Only')
    gapAdvChannelMap.setDefaultValue(0)
    gapAdvChannelMap.setVisible(True)
    gapAdvChannelMap.setOutputMode('Key')
    gapAdvChannelMap.setDisplayMode('Description')
    gapAdvChannelMap.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])
                                                
    # Advertising Filter Policy
    gapAdvFiltPolicy = libBLEStackComponent.createKeyValueSetSymbol('GAP_ADV_FILT_POLICY', gapAdvertising)
    gapAdvFiltPolicy.setLabel('Advertising Filter Policy')
    gapAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_DEFAULT', '0', 'Process scan and connection requests from all devices')
    gapAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN', '1', 'Process connection requests from all devices and scan requests only from devices that are in the White List')
    gapAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_CONNECT', '2', 'Process scan requests from all devices and connection requests only from devices that are in the White List')
    gapAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN_CONNECT', '3', 'Process scan and connection requests only from devices in the White List')
    gapAdvFiltPolicy.setDefaultValue(0)
    gapAdvFiltPolicy.setOutputMode('Key')
    gapAdvFiltPolicy.setDisplayMode('Description')
    gapAdvFiltPolicy.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])    

    # Advertising Set
    gapExtAdvAdvSet = libBLEStackComponent.createMenuSymbol("GAP_EXT_ADV_ADV_SET", gapExtAdvEn)
    gapExtAdvAdvSet.setLabel("Advertising Set 1")
    #gapExtAdvAdvSet.setDefaultValue("Advertising Set")
    #gapExtAdvAdvHandle.setMin(0x00)
    #gapExtAdvAdvHandle.setMax(0xEF)
    gapExtAdvAdvSet.setVisible(False)
    gapExtAdvAdvSet.setDependencies(gapAdvVisibility, ["BOOL_GAP_EXT_ADV"])

    # Advertising Handle
    gapExtAdvAdvHandle = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_ADV_SET_HANDLE", gapExtAdvAdvSet)
    gapExtAdvAdvHandle.setLabel("Advertising Handle")
    gapExtAdvAdvHandle.setDefaultValue(0x00)
    gapExtAdvAdvHandle.setMin(0x00)
    gapExtAdvAdvHandle.setMax(0xEF)
    gapExtAdvAdvHandle.setVisible(False)
    gapExtAdvAdvHandle.setDependencies(gapAdvVisibility, ["BOOL_GAP_EXT_ADV"])

    # Advertising Event Properties
    gapExtAdvEvtProp = libBLEStackComponent.createKeyValueSetSymbol("GAP_EXT_ADV_EVT_PROPERTIES", gapExtAdvAdvSet)
    gapExtAdvEvtProp.setLabel("Advertising Event Properties")
    gapExtAdvEvtProp.addKey('BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '0', 'Connectable and scannable undirected advertising')
    gapExtAdvEvtProp.addKey('BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '1', 'Scannable undirected advertising')
    gapExtAdvEvtProp.addKey('BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '2', 'Non-connectable and non-scannable undirected advertising')
    gapExtAdvEvtProp.addKey('BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV', '3', 'Extended connectable undirected advertising')
    gapExtAdvEvtProp.addKey('BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV', '4', 'Extended scannable undirected advertising')
    gapExtAdvEvtProp.addKey('0', '5', 'Extended non-connectable and non-scannable undirected advertising')
    gapExtAdvEvtProp.setDefaultValue(0)
    gapExtAdvEvtProp.setOutputMode('Key')
    gapExtAdvEvtProp.setDisplayMode('Description')
    
    # Advertising Event Properties - Anonymous advertising
    gapExtAdvEvtPropAnon = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_EVT_PROP_ANON', gapExtAdvEvtProp)
    gapExtAdvEvtPropAnon.setLabel('Anonymous advertising')
    gapExtAdvEvtPropAnon.setDefaultValue(False)
    gapExtAdvEvtPropAnon.setVisible(not gapExtAdvEvtProp.getDefaultValue() < 3)
    gapExtAdvEvtPropAnon.setDependencies(gapExtAdvEvtPropVisibility, ["GAP_EXT_ADV_EVT_PROPERTIES"])

    # Advertising Event Properties - Tx Power
    gapExtAdvEvtPropTxPower = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_EVT_PROP_TX_POWER', gapExtAdvEvtProp)
    gapExtAdvEvtPropTxPower.setLabel('Include TxPower in the extended header')
    gapExtAdvEvtPropTxPower.setDefaultValue(False)
    gapExtAdvEvtPropTxPower.setVisible(not gapExtAdvEvtProp.getDefaultValue() < 3)
    gapExtAdvEvtPropTxPower.setDependencies(gapExtAdvEvtPropVisibility, ["GAP_EXT_ADV_EVT_PROPERTIES"])
    
    # Primary Advertising Interval Min
    gapExtAdvIntMin = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_PRI_INTERVAL_MIN", gapExtAdvAdvSet)
    gapExtAdvIntMin.setLabel("Primary Advertising Interval Min(Unit: 0.625ms)")
    gapExtAdvIntMin.setDefaultValue(0x20)
    gapExtAdvIntMin.setMin(0x20)
    gapExtAdvIntMin.setMax(0xFFFFFF)

    # Primary Advertising Interval Max
    gapExtAdvIntMax = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_PRI_INTERVAL_MAX", gapExtAdvAdvSet)
    gapExtAdvIntMax.setLabel("Primary Advertising Interval Max(Unit: 0.625ms)")
    gapExtAdvIntMax.setDefaultValue(0x20)
    gapExtAdvIntMax.setMin(0x20)
    gapExtAdvIntMax.setMax(0xFFFFFF)
    
    # Primary Advertising Channel Map
    gapExtAdvChannelMap = libBLEStackComponent.createKeyValueSetSymbol('GAP_PRI_CHANNEL_MAP', gapExtAdvAdvSet)
    gapExtAdvChannelMap.setLabel('Primary Advertising Channel Map')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_ALL', '0', 'Channel 37, 38 and 39')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_38', '1', 'Channel 37, and 38')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38|BLE_GAP_ADV_CHANNEL_39', '2', 'Channel 38, and 39')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_39', '3', 'Channel 37, and 39')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37', '4', 'Channel 37 Only')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38', '5', 'Channel 38 Only')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_39', '6', 'Channel 39 Only')
    gapExtAdvChannelMap.setDefaultValue(0)
    gapExtAdvChannelMap.setOutputMode('Key')
    gapExtAdvChannelMap.setDisplayMode('Description')
    
    # Advertising Filter Policy
    gapExtAdvFiltPolicy = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_ADV_FILT_POLICY', gapExtAdvAdvSet)
    gapExtAdvFiltPolicy.setLabel('Advertising Filter Policy')
    gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_DEFAULT', '0', 'Process scan and connection requests from all devices')
    gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN', '1', 'Process connection requests from all devices and scan requests only from devices that are in the White List')
    gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_CONNECT', '2', 'Process scan requests from all devices and connection requests only from devices that are in the White List')
    gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN_CONNECT', '3', 'Process scan and connection requests only from devices in the White List')
    gapExtAdvFiltPolicy.setDefaultValue(0)
    gapExtAdvFiltPolicy.setOutputMode('Key')
    gapExtAdvFiltPolicy.setDisplayMode('Description')
    
    # Advertising Tx Power
    gapExtAdvTxPower = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_TX_POWER", gapExtAdvAdvSet)
    gapExtAdvTxPower.setLabel("Advertising Tx Power(Unit: dBm)")
    gapExtAdvTxPower.setDefaultValue(gapGenDefTxPwr(gapEtsi))
    gapExtAdvTxPower.setMin(gapGenMinTxPwr())
    gapExtAdvTxPower.setMax(gapGenMaxTxPwr(gapEtsi and gapExtAdvEvtProp.getValue() < 3))
    gapExtAdvTxPower.setDependencies(gapExtAdvTxPwr, ["GAP_EXT_ADV_EVT_PROPERTIES"])

    # Primary Advertising PHY
    gapExtAdvPhy = libBLEStackComponent.createKeyValueSetSymbol('GAP_PRI_ADV_PHY', gapExtAdvAdvSet)
    gapExtAdvPhy.setLabel('Primary Advertising PHY')
    gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_1M', '0', 'LE 1M')
    gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_CODED', '1', 'LE Coded')
    gapExtAdvPhy.setDefaultValue(0)
    gapExtAdvPhy.setOutputMode('Key')
    gapExtAdvPhy.setDisplayMode('Description')

    # Secondary Advertising Max Skip
    gapExtAdvSecMaxSkip = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_MAX_SKIP", gapExtAdvAdvSet)
    gapExtAdvSecMaxSkip.setLabel("Secondary Advertising Max Skip")
    gapExtAdvSecMaxSkip.setDefaultValue(0)
    gapExtAdvSecMaxSkip.setMin(0)
    gapExtAdvSecMaxSkip.setMax(0xFF)
    
    # Secondary Advertising PHY
    gapExtAdvPhy = libBLEStackComponent.createKeyValueSetSymbol('GAP_SEC_ADV_PHY', gapExtAdvAdvSet)
    gapExtAdvPhy.setLabel('Secondary Advertising PHY')
    gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_1M', '0', 'LE 1M')
    gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_2M', '1', 'LE 2M')
    gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_CODED', '2', 'LE Coded')
    gapExtAdvPhy.setDefaultValue(0)
    gapExtAdvPhy.setOutputMode('Key')
    gapExtAdvPhy.setDisplayMode('Description')

    # Advertising SID
    gapExtAdvSid = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_SID", gapExtAdvAdvSet)
    gapExtAdvSid.setLabel("Advertising SID")
    gapExtAdvSid.setDefaultValue(0)
    gapExtAdvSid.setMin(0)
    gapExtAdvSid.setMax(0x0F)
    
    #Scan Request Notification Enable
    gapExtAdvScanEnable = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_SCAN_ENABLE', gapExtAdvAdvSet)
    gapExtAdvScanEnable.setLabel('Scan Request Notification Enable')
    gapExtAdvScanEnable.setDefaultValue(False)

    #Ext Advertising Data Menu
    menuExtAdvData = libBLEStackComponent.createMenuSymbol('GAP_EXT_ADV_DATA_MENU', gapExtAdvAdvSet)
    menuExtAdvData.setLabel('Advertising Data')
    menuExtAdvData.setDescription("Advertising Data")
    
    #Ext Advertising Data-Flag Bool
    gapExtAdvDataFlagEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_FLAG_EN', menuExtAdvData)
    gapExtAdvDataFlagEn.setLabel('Flag')
    gapExtAdvDataFlagEn.setDefaultValue(True)
    gapExtAdvDataFlagEn.setDescription("Flag")

    #Ext Advertising Data-Flag
    gapExtAdvDataFlag = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_ADV_DATA_FLAG', gapExtAdvDataFlagEn)
    gapExtAdvDataFlag.setLabel(' ')
    gapExtAdvDataFlag.addKey('0x04', '0', 'LE Limited Discoverable Mode')
    gapExtAdvDataFlag.addKey('0x05', '1', 'LE General Discoverable Mode')
    gapExtAdvDataFlag.setDefaultValue(0)
    gapExtAdvDataFlag.setOutputMode('Key')
    gapExtAdvDataFlag.setDisplayMode('Description')
    gapExtAdvDataFlag.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_FLAG_EN"])

    #Ext Advertising Data-Local Name Bool    
    gapExtAdvDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_LOCAL_NAME_EN', menuExtAdvData)
    #print('libBLEStackComponent:gapExtAdvDataLocalNameEn type({})'.format(type(gapExtAdvDataLocalNameEn)))
    gapExtAdvDataLocalNameEn.setLabel('Local Name')
    gapExtAdvDataLocalNameEn.setDefaultValue(True)
    gapExtAdvDataLocalNameEn.setDescription("Local Name")

    #Ext Advertising Data-Local Name
    gapExtAdvDataLocalName = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_LOCAL_NAME", gapExtAdvDataLocalNameEn)
    gapExtAdvDataLocalName.setLabel(" ")
    gapExtAdvDataLocalName.setDefaultValue("pic32cx-bz")
    gapExtAdvDataLocalName.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_LOCAL_NAME_EN"])

    #Ext Advertising Data-Service Data Bool
    gapExtAdvDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_SERVICE_DATA_EN', menuExtAdvData)
    gapExtAdvDataSvcDataEn.setLabel('Service Data')
    gapExtAdvDataSvcDataEn.setDefaultValue(False)
    gapExtAdvDataSvcDataEn.setDescription("Service Data")

    #Ext Advertising Data-Service UUID
    gapExtAdvDataSvcUUID = libBLEStackComponent.createHexSymbol("GAP_EXT_ADV_DATA_SERVICE_UUID", gapExtAdvDataSvcDataEn)
    gapExtAdvDataSvcUUID.setLabel("Service UUID")
    gapExtAdvDataSvcUUID.setDefaultValue(0xFEDA)
    gapExtAdvDataSvcUUID.setMax(0xFFFF)
    gapExtAdvDataSvcUUID.setVisible(False)
    gapExtAdvDataSvcUUID.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN"])

    #Ext Advertising Data- Service Data
    gapExtAdvDataSvcData = libBLEStackComponent.createHexSymbol("GAP_EXT_ADV_DATA_SERVICE_DATA", gapExtAdvDataSvcDataEn)
    gapExtAdvDataSvcData.setLabel("Service Data")
    #gapExtAdvDataSvcData.setDefaultValue(0x0304)
    gapExtAdvDataSvcData.setVisible(False)
    gapExtAdvDataSvcData.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN"])

    #Ext Advertising Data-Manufacture Specific Data Bool
    gapExtAdvDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_MSD_EN', menuExtAdvData)
    gapExtAdvDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
    gapExtAdvDataManuSpecDataEn.setDefaultValue(False)
    gapExtAdvDataManuSpecDataEn.setDescription("Manufacturer Specific Data")

    #Ext Advertising Data-Manufacture Specific Data Company Id
    gapExtAdvDataManuSpecDataCid = libBLEStackComponent.createHexSymbol("GAP_EXT_ADV_DATA_MSD_CID", gapExtAdvDataManuSpecDataEn)
    gapExtAdvDataManuSpecDataCid.setLabel("Company Id")
    #gapExtAdvDataManuSpecDataCid.setDefaultValue(0x00CD)
    gapExtAdvDataManuSpecDataCid.setMax(0xFFFF)
    gapExtAdvDataManuSpecDataCid.setVisible(False)
    gapExtAdvDataManuSpecDataCid.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_MSD_EN"])

    #Ext Advertising Data-Manufacture Specific Data
    gapExtAdvDataManuSpecData = libBLEStackComponent.createHexSymbol("GAP_EXT_ADV_DATA_MSD", gapExtAdvDataManuSpecDataEn)
    gapExtAdvDataManuSpecData.setLabel("Manufacturer Data")
    #gapExtAdvDataManuSpecData.setDefaultValue(0x0405)
    gapExtAdvDataManuSpecData.setVisible(False)
    gapExtAdvDataManuSpecData.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_MSD_EN"])

    #Ext Advertising Data-User Defined Data Bool
    gapExtAdvDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_UDD_EN', menuExtAdvData)
    gapExtAdvDataUserDefEn.setLabel('User Defined Data')
    gapExtAdvDataUserDefEn.setDefaultValue(False)

    #Ext Advertising Data-User Defined Data
    gapExtAdvDataUserDef = libBLEStackComponent.createHexSymbol("GAP_EXT_ADV_DATA_UDD", gapExtAdvDataUserDefEn)
    gapExtAdvDataUserDef.setLabel("User Data")
    #gapExtAdvDataUserDef.setDefaultValue(0x0607)
    gapExtAdvDataUserDef.setVisible(False)
    gapExtAdvDataUserDef.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_UDD_EN"])
    
    # Ext Advertising Data
    gapExtAdvertisingData = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA", gapExtAdvAdvSet)
    gapExtAdvertisingData.setLabel("Advertising Data")
    gapExtAdvertisingData.setDefaultValue("Advertising Data")
    gapExtAdvertisingData.setVisible(False)
    gapExtAdvertisingData.setDependencies(gapSetExtAdvData, ["GAP_EXT_ADV_DATA_FLAG_EN", "GAP_EXT_ADV_DATA_FLAG", 
                                                       "GAP_EXT_ADV_DATA_LOCAL_NAME_EN", "GAP_EXT_ADV_DATA_LOCAL_NAME", 
                                                       "GAP_EXT_ADV_DATA_SERVICE_DATA_EN", 
                                                       "GAP_EXT_ADV_DATA_SERVICE_UUID", 
                                                       "GAP_EXT_ADV_DATA_SERVICE_DATA", 
                                                       "GAP_EXT_ADV_DATA_MSD_EN", "GAP_EXT_ADV_DATA_MSD", "GAP_EXT_ADV_DATA_MSD_CID", 
                                                       "GAP_EXT_ADV_DATA_UDD_EN", "GAP_EXT_ADV_DATA_UDD", ])

    # Ext Advertising Data length
    gapExtAdvertisingDataLen = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_DATA_LEN", gapExtAdvAdvSet)
    gapExtAdvertisingDataLen.setLabel("Advertising Data Len")
    gapExtAdvertisingDataLen.setDefaultValue(0)
    gapExtAdvertisingDataLen.setVisible(False)
    
    gapExtAdvertisingDataValue = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_VALUE", gapExtAdvAdvSet)
    gapExtAdvertisingDataValue.setVisible(False)
    
    gapGenExtAdvData(gapExtAdvertisingData)

    #Ext Scan Response Data
    menuExtScanRspData = libBLEStackComponent.createMenuSymbol('GAP_EXT_SCAN_RSP_DATA_MENU', gapExtAdvAdvSet)
    menuExtScanRspData.setLabel('Scan Response Data')
    menuExtScanRspData.setDescription("Scan Response Data")

    #Ext Scan Response Data-Local Name Bool    
    gapExtScanRspDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN', menuExtScanRspData)
    gapExtScanRspDataLocalNameEn.setLabel('Local Name')
    gapExtScanRspDataLocalNameEn.setDefaultValue(True)
    gapExtScanRspDataLocalNameEn.setDescription("Local Name")

    #Ext Scan Response Data-Local Name
    gapExtScanRspDataLocalName = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME", gapExtScanRspDataLocalNameEn)
    gapExtScanRspDataLocalName.setLabel(" ")
    gapExtScanRspDataLocalName.setDefaultValue("pic32cx-bz")
    gapExtScanRspDataLocalName.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN"])

    #Ext Scan Response Data-Service Data Bool
    gapExtScanRspDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN', menuExtScanRspData)
    gapExtScanRspDataSvcDataEn.setLabel('Service Data')
    gapExtScanRspDataSvcDataEn.setDefaultValue(False)
    gapExtScanRspDataSvcDataEn.setDescription("Service Data")

    #Ext Scan Response Data-Service UUID
    gapExtScanRspDataSvcUUID = libBLEStackComponent.createHexSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID", gapExtScanRspDataSvcDataEn)
    gapExtScanRspDataSvcUUID.setLabel("Service UUID")
    gapExtScanRspDataSvcUUID.setDefaultValue(0xFEDA)
    gapExtScanRspDataSvcUUID.setMax(0xFFFF)
    gapExtScanRspDataSvcUUID.setVisible(False)
    gapExtScanRspDataSvcUUID.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN"])
    
    #Ext Scan Response Data- Service Data
    gapExtScanRspDataSvcData = libBLEStackComponent.createHexSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA", gapExtScanRspDataSvcDataEn)
    gapExtScanRspDataSvcData.setLabel("Service Data")
    #gapExtScanRspDataSvcData.setDefaultValue(0x3344)
    gapExtScanRspDataSvcData.setVisible(False)
    gapExtScanRspDataSvcData.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN"])
    
    #Ext Scan Response Data-Manufacture Specific Data Bool
    gapExtScanRspDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_MSD_EN', menuExtScanRspData)
    gapExtScanRspDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
    gapExtScanRspDataManuSpecDataEn.setDefaultValue(False)
    gapExtScanRspDataManuSpecDataEn.setDescription("Manufacturer Specific Data")
    
    #Ext Scan Response Data-Manufacture Specific Data Company Id
    gapExtScanRspDataManuSpecDataCid = libBLEStackComponent.createHexSymbol("GAP_EXT_SCAN_RSP_DATA_MSD_CID", gapExtScanRspDataManuSpecDataEn)
    gapExtScanRspDataManuSpecDataCid.setLabel("Company Id")
    #gapExtScanRspDataManuSpecDataCid.setDefaultValue(0x00CD)
    gapExtScanRspDataManuSpecDataCid.setVisible(False)
    gapExtScanRspDataManuSpecDataCid.setMax(0xFFFF)
    gapExtScanRspDataManuSpecDataCid.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_MSD_EN"])

    #Ext Scan Response Data-Manufacture Specific Data
    gapExtScanRspDataManuSpecData = libBLEStackComponent.createHexSymbol("GAP_EXT_SCAN_RSP_DATA_MSD", gapExtScanRspDataManuSpecDataEn)
    gapExtScanRspDataManuSpecData.setLabel("Manufacture Data")
    #gapExtScanRspDataManuSpecData.setDefaultValue(0x5566)
    gapExtScanRspDataManuSpecData.setVisible(False)
    gapExtScanRspDataManuSpecData.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_MSD_EN"])
    
    #Ext Scan Response Data-User Defined Data Bool
    gapExtScanRspDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_UDD_EN', menuExtScanRspData)
    gapExtScanRspDataUserDefEn.setLabel('User Defined Data')
    gapExtScanRspDataUserDefEn.setDefaultValue(False)
    
    #Ext Scan Response Data-User Defined Data
    gapExtScanRspDataUserDef = libBLEStackComponent.createHexSymbol("GAP_EXT_SCAN_RSP_DATA_UDD", gapExtScanRspDataUserDefEn)
    gapExtScanRspDataUserDef.setLabel(" ")
    #gapExtScanRspDataUserDef.setDefaultValue(0x7788)
    gapExtScanRspDataUserDef.setVisible(False)
    gapExtScanRspDataUserDef.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_UDD_EN"])

    # Ext Scan Response Data
    gapExtScanRspData = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA", gapExtAdvAdvSet)
    gapExtScanRspData.setLabel("Scan Response Data")
    gapExtScanRspData.setVisible(False)
    gapExtScanRspData.setDependencies(gapSetExtScanRspData, ["GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN","GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME",
                                                       "GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN", 
                                                       "GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID", 
                                                       "GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA", 
                                                       "GAP_EXT_SCAN_RSP_DATA_MSD_EN", "GAP_EXT_SCAN_RSP_DATA_MSD", "GAP_EXT_SCAN_RSP_DATA_MSD_CID",  
                                                       "GAP_EXT_SCAN_RSP_DATA_UDD_EN", "GAP_EXT_SCAN_RSP_DATA_UDD", ])

    # Ext Scan Response Data length
    gapExtScanRspDataLen = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_RSP_DATA_LEN", gapExtAdvAdvSet)
    gapExtScanRspDataLen.setLabel("Scan Response Data Len")
    gapExtScanRspDataLen.setDefaultValue(0)
    gapExtScanRspDataLen.setVisible(False)

    gapExtScanRspDataValue = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_VALUE", gapExtAdvAdvSet)
    gapExtScanRspDataValue.setVisible(False)
    
    gapGenExtScanRspData(gapExtScanRspData)

    # Advertising Set 2
    gapExtAdvAdvSet2 = libBLEStackComponent.createBooleanSymbol("GAP_EXT_ADV_ADV_SET_2", gapExtAdvEn)
    gapExtAdvAdvSet2.setLabel("Advertising Set 2")
    gapExtAdvAdvSet2.setDefaultValue(False)
    gapExtAdvAdvSet2.setVisible(False)
    gapExtAdvAdvSet2.setDependencies(gapAdvVisibility, ["BOOL_GAP_EXT_ADV"])

    # Advertising Handle
    gapExtAdvAdvHandle = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_ADV_SET_HANDLE_2", gapExtAdvAdvSet2)
    gapExtAdvAdvHandle.setLabel("Advertising Handle")
    gapExtAdvAdvHandle.setDefaultValue(0x01)
    gapExtAdvAdvHandle.setMin(0x00)
    gapExtAdvAdvHandle.setMax(0xEF)
    gapExtAdvAdvHandle.setVisible(False)
    gapExtAdvAdvHandle.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    # Advertising Event Properties
    gapExtAdvEvtProp2 = libBLEStackComponent.createKeyValueSetSymbol("GAP_EXT_ADV_EVT_PROPERTIES_2", gapExtAdvAdvSet2)
    gapExtAdvEvtProp2.setLabel("Advertising Event Properties")
    gapExtAdvEvtProp2.addKey('BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '0', 'Connectable and scannable undirected advertising')
    gapExtAdvEvtProp2.addKey('BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV|BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '1', 'Scannable undirected advertising')
    gapExtAdvEvtProp2.addKey('BLE_GAP_EXT_ADV_EVT_PROP_LEGACY_ADV', '2', 'Non-connectable and non-scannable undirected advertising')
    gapExtAdvEvtProp2.addKey('BLE_GAP_EXT_ADV_EVT_PROP_CONNECTABLE_ADV', '3', 'Extended connectable undirected advertising')
    gapExtAdvEvtProp2.addKey('BLE_GAP_EXT_ADV_EVT_PROP_SCANNABLE_ADV', '4', 'Extended scannable undirected advertising')
    gapExtAdvEvtProp2.addKey('0', '5', 'Extended non-connectable and non-scannable undirected advertising')
    gapExtAdvEvtProp2.setDefaultValue(0)
    gapExtAdvEvtProp2.setVisible(False)
    gapExtAdvEvtProp2.setOutputMode('Key')
    gapExtAdvEvtProp2.setDisplayMode('Description')
    gapExtAdvEvtProp2.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])
    
    # Advertising Event Properties - Anonymous advertising
    gapExtAdvEvtPropAnon = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_EVT_PROP_ANON_2', gapExtAdvEvtProp2)
    gapExtAdvEvtPropAnon.setLabel('Anonymous advertising')
    gapExtAdvEvtPropAnon.setDefaultValue(False)
    gapExtAdvEvtPropAnon.setVisible(not gapExtAdvEvtProp2.getDefaultValue() < 3)
    gapExtAdvEvtPropAnon.setDependencies(gapExtAdvEvtPropVisibility, ["GAP_EXT_ADV_EVT_PROPERTIES_2"])

    # Advertising Event Properties - Tx Power
    gapExtAdvEvtPropTxPower = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_EVT_PROP_TX_POWER_2', gapExtAdvEvtProp2)
    gapExtAdvEvtPropTxPower.setLabel('Include TxPower in the extended header')
    gapExtAdvEvtPropTxPower.setDefaultValue(False)
    gapExtAdvEvtPropTxPower.setVisible(not gapExtAdvEvtProp2.getDefaultValue() < 3)
    gapExtAdvEvtPropTxPower.setDependencies(gapExtAdvEvtPropVisibility, ["GAP_EXT_ADV_EVT_PROPERTIES_2"])
    
    # Primary Advertising Interval Min
    gapExtAdvIntMin = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_PRI_INTERVAL_MIN_2", gapExtAdvAdvSet2)
    gapExtAdvIntMin.setLabel("Primary Advertising Interval Min(Unit: 0.625ms)")
    gapExtAdvIntMin.setDefaultValue(0x20)
    gapExtAdvIntMin.setMin(0x20)
    gapExtAdvIntMin.setMax(0xFFFFFF)
    gapExtAdvIntMin.setVisible(False)
    gapExtAdvIntMin.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    # Primary Advertising Interval Max
    gapExtAdvIntMax = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_PRI_INTERVAL_MAX_2", gapExtAdvAdvSet2)
    gapExtAdvIntMax.setLabel("Primary Advertising Interval Max(Unit: 0.625ms)")
    gapExtAdvIntMax.setDefaultValue(0x20)
    gapExtAdvIntMax.setMin(0x20)
    gapExtAdvIntMax.setMax(0xFFFFFF)
    gapExtAdvIntMax.setVisible(False)
    gapExtAdvIntMax.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])
    
    # Primary Advertising Channel Map
    gapExtAdvChannelMap = libBLEStackComponent.createKeyValueSetSymbol('GAP_PRI_CHANNEL_MAP_2', gapExtAdvAdvSet2)
    gapExtAdvChannelMap.setLabel('Primary Advertising Channel Map')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_ALL', '0', 'Channel 37, 38 and 39')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_38', '1', 'Channel 37, and 38')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38|BLE_GAP_ADV_CHANNEL_39', '2', 'Channel 38, and 39')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37|BLE_GAP_ADV_CHANNEL_39', '3', 'Channel 37, and 39')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_37', '4', 'Channel 37 Only')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_38', '5', 'Channel 38 Only')
    gapExtAdvChannelMap.addKey('BLE_GAP_ADV_CHANNEL_39', '6', 'Channel 39 Only')
    gapExtAdvChannelMap.setDefaultValue(0)
    gapExtAdvChannelMap.setVisible(False)
    gapExtAdvChannelMap.setOutputMode('Key')
    gapExtAdvChannelMap.setDisplayMode('Description')
    gapExtAdvChannelMap.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])
    
    # Advertising Filter Policy
    gapExtAdvFiltPolicy = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_ADV_FILT_POLICY_2', gapExtAdvAdvSet2)
    gapExtAdvFiltPolicy.setLabel('Advertising Filter Policy')
    gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_DEFAULT', '0', 'Process scan and connection requests from all devices')
    gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN', '1', 'Process connection requests from all devices and scan requests only from devices that are in the White List')
    gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_CONNECT', '2', 'Process scan requests from all devices and connection requests only from devices that are in the White List')
    gapExtAdvFiltPolicy.addKey('BLE_GAP_ADV_FILTER_SCAN_CONNECT', '3', 'Process scan and connection requests only from devices in the White List')
    gapExtAdvFiltPolicy.setDefaultValue(0)
    gapExtAdvFiltPolicy.setVisible(False)
    gapExtAdvFiltPolicy.setOutputMode('Key')
    gapExtAdvFiltPolicy.setDisplayMode('Description')
    gapExtAdvFiltPolicy.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])    

    # Advertising Tx Power
    gapExtAdvTxPower = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_TX_POWER_2", gapExtAdvAdvSet2)
    gapExtAdvTxPower.setLabel("Advertising Tx Power(Unit: dBm)")
    gapExtAdvTxPower.setDefaultValue(gapGenDefTxPwr(gapEtsi))
    gapExtAdvTxPower.setMin(gapGenMinTxPwr())
    gapExtAdvTxPower.setMax(gapGenMaxTxPwr(gapEtsi and gapExtAdvEvtProp2.getValue() < 3))
    gapExtAdvTxPower.setVisible(False)
    gapExtAdvTxPower.setDependencies(gapExtAdvTxPwr, ["GAP_EXT_ADV_ADV_SET_2", "GAP_EXT_ADV_EVT_PROPERTIES_2"])

    # Primary Advertising PHY
    gapExtAdvPhy = libBLEStackComponent.createKeyValueSetSymbol('GAP_PRI_ADV_PHY_2', gapExtAdvAdvSet2)
    gapExtAdvPhy.setLabel('Primary Advertising PHY')
    gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_1M', '0', 'LE 1M')
    gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_CODED', '1', 'LE Coded')
    gapExtAdvPhy.setDefaultValue(0)
    gapExtAdvPhy.setVisible(False)
    gapExtAdvPhy.setOutputMode('Key')
    gapExtAdvPhy.setDisplayMode('Description')
    gapExtAdvPhy.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    # Secondary Advertising Max Skip
    gapExtAdvSecMaxSkip = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_MAX_SKIP_2", gapExtAdvAdvSet2)
    gapExtAdvSecMaxSkip.setLabel("Secondary Advertising Max Skip")
    gapExtAdvSecMaxSkip.setDefaultValue(0)
    gapExtAdvSecMaxSkip.setMin(0)
    gapExtAdvSecMaxSkip.setMax(0xFF)
    gapExtAdvSecMaxSkip.setVisible(False)
    gapExtAdvSecMaxSkip.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])
    
    # Secondary Advertising PHY
    gapExtAdvPhy = libBLEStackComponent.createKeyValueSetSymbol('GAP_SEC_ADV_PHY_2', gapExtAdvAdvSet2)
    gapExtAdvPhy.setLabel('Secondary Advertising PHY')
    gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_1M', '0', 'LE 1M')
    gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_2M', '1', 'LE 2M')
    gapExtAdvPhy.addKey('BLE_GAP_PHY_TYPE_LE_CODED', '2', 'LE Coded')
    gapExtAdvPhy.setDefaultValue(0)
    gapExtAdvPhy.setVisible(False)
    gapExtAdvPhy.setOutputMode('Key')
    gapExtAdvPhy.setDisplayMode('Description')
    gapExtAdvPhy.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    # Advertising SID
    gapExtAdvSid = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_SID_2", gapExtAdvAdvSet2)
    gapExtAdvSid.setLabel("Advertising SID")
    gapExtAdvSid.setDefaultValue(0)
    gapExtAdvSid.setMin(0)
    gapExtAdvSid.setMax(0x0F)
    gapExtAdvSid.setVisible(False)
    gapExtAdvSid.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])
    
    #Scan Request Notification Enable
    gapExtAdvScanEnable = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_SCAN_ENABLE_2', gapExtAdvAdvSet2)
    gapExtAdvScanEnable.setLabel('Scan Request Notification Enable')
    gapExtAdvScanEnable.setDefaultValue(False)
    gapExtAdvScanEnable.setVisible(False)
    gapExtAdvScanEnable.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    #Ext Advertising Data Menu
    menuExtAdvData = libBLEStackComponent.createMenuSymbol('GAP_EXT_ADV_DATA_MENU_2', gapExtAdvAdvSet2)
    menuExtAdvData.setLabel('Advertising Data')
    menuExtAdvData.setVisible(False)
    menuExtAdvData.setDescription("Advertising Data")
    menuExtAdvData.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])
    
    #Ext Advertising Data-Flag Bool
    gapExtAdvDataFlagEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_FLAG_EN_2', menuExtAdvData)
    gapExtAdvDataFlagEn.setLabel('Flag')
    gapExtAdvDataFlagEn.setDefaultValue(True)
    gapExtAdvDataFlagEn.setVisible(True)
    gapExtAdvDataFlagEn.setDescription("Flag")
    gapExtAdvDataFlagEn.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    #Ext Advertising Data-Flag
    gapExtAdvDataFlag = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_ADV_DATA_FLAG_2', gapExtAdvDataFlagEn)
    gapExtAdvDataFlag.setLabel(' ')
    gapExtAdvDataFlag.addKey('0x04', '0', 'LE Limited Discoverable Mode')
    gapExtAdvDataFlag.addKey('0x05', '1', 'LE General Discoverable Mode')
    gapExtAdvDataFlag.setDefaultValue(0)
    gapExtAdvDataFlag.setVisible(True)
    gapExtAdvDataFlag.setOutputMode('Key')
    gapExtAdvDataFlag.setDisplayMode('Description')
    gapExtAdvDataFlag.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_FLAG_EN_2"])

    #Ext Advertising Data-Local Name Bool    
    gapExtAdvDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_LOCAL_NAME_EN_2', menuExtAdvData)
    #print('libBLEStackComponent:gapExtAdvDataLocalNameEn type({})'.format(type(gapExtAdvDataLocalNameEn)))
    gapExtAdvDataLocalNameEn.setLabel('Local Name')
    gapExtAdvDataLocalNameEn.setDefaultValue(True)
    gapExtAdvDataLocalNameEn.setVisible(True)
    gapExtAdvDataLocalNameEn.setDescription("Local Name")
    gapExtAdvDataLocalNameEn.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    #Ext Advertising Data-Local Name
    gapExtAdvDataLocalName = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_LOCAL_NAME_2", gapExtAdvDataLocalNameEn)
    gapExtAdvDataLocalName.setLabel(" ")
    gapExtAdvDataLocalName.setDefaultValue("pic32cx-bz")
    gapExtAdvDataLocalName.setVisible(True)
    gapExtAdvDataLocalName.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_LOCAL_NAME_EN_2"])

    #Ext Advertising Data-Service Data Bool
    gapExtAdvDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2', menuExtAdvData)
    gapExtAdvDataSvcDataEn.setLabel('Service Data')
    gapExtAdvDataSvcDataEn.setDefaultValue(False)
    gapExtAdvDataSvcDataEn.setVisible(True)
    gapExtAdvDataSvcDataEn.setDescription("Service Data")
    gapExtAdvDataSvcDataEn.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    #Ext Advertising Data-Service UUID
    gapExtAdvDataSvcUUID = libBLEStackComponent.createHexSymbol("GAP_EXT_ADV_DATA_SERVICE_UUID_2", gapExtAdvDataSvcDataEn)
    gapExtAdvDataSvcUUID.setLabel("Service UUID")
    gapExtAdvDataSvcUUID.setDefaultValue(0xFEDA)
    gapExtAdvDataSvcUUID.setMax(0xFFFF)
    gapExtAdvDataSvcUUID.setVisible(False)
    gapExtAdvDataSvcUUID.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2"])

    #Ext Advertising Data- Service Data
    gapExtAdvDataSvcData = libBLEStackComponent.createHexSymbol("GAP_EXT_ADV_DATA_SERVICE_DATA_2", gapExtAdvDataSvcDataEn)
    gapExtAdvDataSvcData.setLabel("Service Data")
    #gapExtAdvDataSvcData.setDefaultValue(0x0304)
    gapExtAdvDataSvcData.setVisible(False)
    gapExtAdvDataSvcData.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2"])

    #Ext Advertising Data-Manufacture Specific Data Bool
    gapExtAdvDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_MSD_EN_2', menuExtAdvData)
    gapExtAdvDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
    gapExtAdvDataManuSpecDataEn.setDefaultValue(False)
    gapExtAdvDataManuSpecDataEn.setVisible(True)
    gapExtAdvDataManuSpecDataEn.setDescription("Manufacturer Specific Data")
    gapExtAdvDataManuSpecDataEn.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    #Ext Advertising Data-Manufacture Specific Data Company Id
    gapExtAdvDataManuSpecDataCid = libBLEStackComponent.createHexSymbol("GAP_EXT_ADV_DATA_MSD_CID_2", gapExtAdvDataManuSpecDataEn)
    gapExtAdvDataManuSpecDataCid.setLabel("Company Id")
    #gapExtAdvDataManuSpecDataCid.setDefaultValue(0x00CD)
    gapExtAdvDataManuSpecDataCid.setMax(0xFFFF)
    gapExtAdvDataManuSpecDataCid.setVisible(False)
    gapExtAdvDataManuSpecDataCid.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_MSD_EN_2"])

    #Ext Advertising Data-Manufacture Specific Data
    gapExtAdvDataManuSpecData = libBLEStackComponent.createHexSymbol("GAP_EXT_ADV_DATA_MSD_2", gapExtAdvDataManuSpecDataEn)
    gapExtAdvDataManuSpecData.setLabel("Manufacturer Data")
    #gapExtAdvDataManuSpecData.setDefaultValue(0x0405)
    gapExtAdvDataManuSpecData.setVisible(False)
    gapExtAdvDataManuSpecData.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_MSD_EN_2"])

    #Ext Advertising Data-User Defined Data Bool
    gapExtAdvDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_ADV_DATA_UDD_EN_2', menuExtAdvData)
    gapExtAdvDataUserDefEn.setLabel('User Defined Data')
    gapExtAdvDataUserDefEn.setDefaultValue(False)
    gapExtAdvDataUserDefEn.setVisible(True)
    gapExtAdvDataUserDefEn.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    #Ext Advertising Data-User Defined Data
    gapExtAdvDataUserDef = libBLEStackComponent.createHexSymbol("GAP_EXT_ADV_DATA_UDD_2", gapExtAdvDataUserDefEn)
    gapExtAdvDataUserDef.setLabel("User Data")
    #gapExtAdvDataUserDef.setDefaultValue(0x0607)
    gapExtAdvDataUserDef.setVisible(False)
    gapExtAdvDataUserDef.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_DATA_UDD_EN_2"])
    
    # Ext Advertising Data
    gapExtAdvertisingData = libBLEStackComponent.createStringSymbol("GAP_EXT_ADV_DATA_2", gapExtAdvAdvSet2)
    gapExtAdvertisingData.setLabel("Advertising Data")
    gapExtAdvertisingData.setDefaultValue("Advertising Data")
    gapExtAdvertisingData.setVisible(False)
    gapExtAdvertisingData.setDependencies(gapSetExtAdvData, ["GAP_EXT_ADV_DATA_FLAG_EN_2", "GAP_EXT_ADV_DATA_FLAG_2", 
                                                       "GAP_EXT_ADV_DATA_LOCAL_NAME_EN_2", "GAP_EXT_ADV_DATA_LOCAL_NAME_2", 
                                                       "GAP_EXT_ADV_DATA_SERVICE_DATA_EN_2", 
                                                       "GAP_EXT_ADV_DATA_SERVICE_UUID_2", 
                                                       "GAP_EXT_ADV_DATA_SERVICE_DATA_2", 
                                                       "GAP_EXT_ADV_DATA_MSD_EN_2", "GAP_EXT_ADV_DATA_MSD_2", "GAP_EXT_ADV_DATA_MSD_CID_2", 
                                                       "GAP_EXT_ADV_DATA_UDD_EN_2", "GAP_EXT_ADV_DATA_UDD_2", ])

    # Ext Advertising Data length
    gapExtAdvertisingDataLen = libBLEStackComponent.createIntegerSymbol("GAP_EXT_ADV_DATA_LEN_2", gapExtAdvAdvSet2)
    gapExtAdvertisingDataLen.setLabel("Advertising Data Len")
    gapExtAdvertisingDataLen.setDefaultValue(0)
    gapExtAdvertisingDataLen.setVisible(False)

    gapExtAdvertisingDataValue = libBLEStackComponent.createCommentSymbol("GAP_EXT_ADV_DATA_VALUE_2", gapExtAdvAdvSet2)
    gapExtAdvertisingDataValue.setVisible(False)
    
    gapGenExtAdvData(gapExtAdvertisingData)

    #Ext Scan Response Data
    menuExtScanRspData = libBLEStackComponent.createMenuSymbol('GAP_EXT_SCAN_RSP_DATA_MENU_2', gapExtAdvAdvSet2)
    menuExtScanRspData.setLabel('Scan Response Data')
    menuExtScanRspData.setVisible(False)
    menuExtScanRspData.setDescription("Scan Response Data")
    menuExtScanRspData.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    #Ext Scan Response Data-Local Name Bool    
    gapExtScanRspDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN_2', menuExtScanRspData)
    gapExtScanRspDataLocalNameEn.setLabel('Local Name')
    gapExtScanRspDataLocalNameEn.setDefaultValue(True)
    gapExtScanRspDataLocalNameEn.setDescription("Local Name")

    #Ext Scan Response Data-Local Name
    gapExtScanRspDataLocalName = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_2", gapExtScanRspDataLocalNameEn)
    gapExtScanRspDataLocalName.setLabel(" ")
    gapExtScanRspDataLocalName.setDefaultValue("pic32cx-bz")
    gapExtScanRspDataLocalName.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN_2"])


    #Ext Scan Response Data-Service Data Bool
    gapExtScanRspDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2', menuExtScanRspData)
    gapExtScanRspDataSvcDataEn.setLabel('Service Data')
    gapExtScanRspDataSvcDataEn.setDefaultValue(False)
    gapExtScanRspDataSvcDataEn.setVisible(True)
    gapExtScanRspDataSvcDataEn.setDescription("Service Data")
    gapExtScanRspDataSvcDataEn.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])

    #Ext Scan Response Data-Service UUID
    gapExtScanRspDataSvcUUID = libBLEStackComponent.createHexSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID_2", gapExtScanRspDataSvcDataEn)
    gapExtScanRspDataSvcUUID.setLabel("Service UUID")
    gapExtScanRspDataSvcUUID.setDefaultValue(0xFEDA)
    gapExtScanRspDataSvcUUID.setMax(0xFFFF)
    gapExtScanRspDataSvcUUID.setVisible(False)
    gapExtScanRspDataSvcUUID.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2"])
    
    #Ext Scan Response Data- Service Data
    gapExtScanRspDataSvcData = libBLEStackComponent.createHexSymbol("GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_2", gapExtScanRspDataSvcDataEn)
    gapExtScanRspDataSvcData.setLabel("Service Data")
    #gapExtScanRspDataSvcData.setDefaultValue(0x3344)
    gapExtScanRspDataSvcData.setVisible(False)
    gapExtScanRspDataSvcData.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2"])
    
    #Ext Scan Response Data-Manufacture Specific Data Bool
    gapExtScanRspDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_MSD_EN_2', menuExtScanRspData)
    gapExtScanRspDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
    gapExtScanRspDataManuSpecDataEn.setDefaultValue(False)
    gapExtScanRspDataManuSpecDataEn.setVisible(True)
    gapExtScanRspDataManuSpecDataEn.setDescription("Manufacturer Specific Data")
    gapExtScanRspDataManuSpecDataEn.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])
    
    #Ext Scan Response Data-Manufacture Specific Data Company Id
    gapExtScanRspDataManuSpecDataCid = libBLEStackComponent.createHexSymbol("GAP_EXT_SCAN_RSP_DATA_MSD_CID_2", gapExtScanRspDataManuSpecDataEn)
    gapExtScanRspDataManuSpecDataCid.setLabel("Company Id")
    #gapExtScanRspDataManuSpecDataCid.setDefaultValue(0x00CD)
    gapExtScanRspDataManuSpecDataCid.setVisible(False)
    gapExtScanRspDataManuSpecDataCid.setMax(0xFFFF)
    gapExtScanRspDataManuSpecDataCid.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_MSD_EN_2"])

    #Ext Scan Response Data-Manufacture Specific Data
    gapExtScanRspDataManuSpecData = libBLEStackComponent.createHexSymbol("GAP_EXT_SCAN_RSP_DATA_MSD_2", gapExtScanRspDataManuSpecDataEn)
    gapExtScanRspDataManuSpecData.setLabel("Manufacture Data")
    #gapExtScanRspDataManuSpecData.setDefaultValue(0x5566)
    gapExtScanRspDataManuSpecData.setVisible(False)
    gapExtScanRspDataManuSpecData.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_MSD_EN_2"])
    
    #Ext Scan Response Data-User Defined Data Bool
    gapExtScanRspDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_EXT_SCAN_RSP_DATA_UDD_EN_2', menuExtScanRspData)
    gapExtScanRspDataUserDefEn.setLabel('User Defined Data')
    gapExtScanRspDataUserDefEn.setDefaultValue(False)
    gapExtScanRspDataUserDefEn.setVisible(True)
    gapExtScanRspDataUserDefEn.setDependencies(gapAdvVisibility, ["GAP_EXT_ADV_ADV_SET_2"])
    
    #Ext Scan Response Data-User Defined Data
    gapExtScanRspDataUserDef = libBLEStackComponent.createHexSymbol("GAP_EXT_SCAN_RSP_DATA_UDD_2", gapExtScanRspDataUserDefEn)
    gapExtScanRspDataUserDef.setLabel(" ")
    #gapExtScanRspDataUserDef.setDefaultValue(0x7788)
    gapExtScanRspDataUserDef.setVisible(False)
    gapExtScanRspDataUserDef.setDependencies(gapAdvVisibility, ["GAP_EXT_SCAN_RSP_DATA_UDD_EN_2"])

    # Ext Scan Response Data
    gapExtScanRspData = libBLEStackComponent.createStringSymbol("GAP_EXT_SCAN_RSP_DATA_2", gapExtAdvAdvSet2)
    gapExtScanRspData.setLabel("Scan Response Data")
    gapExtScanRspData.setVisible(False)
    gapExtScanRspData.setDependencies(gapSetExtScanRspData, ["GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_EN_2","GAP_EXT_SCAN_RSP_DATA_LOCAL_NAME_2",
                                                       "GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_EN_2", 
                                                       "GAP_EXT_SCAN_RSP_DATA_SERVICE_UUID_2", 
                                                       "GAP_EXT_SCAN_RSP_DATA_SERVICE_DATA_2", 
                                                       "GAP_EXT_SCAN_RSP_DATA_MSD_EN_2", "GAP_EXT_SCAN_RSP_DATA_MSD_2", "GAP_EXT_SCAN_RSP_DATA_MSD_CID_2",  
                                                       "GAP_EXT_SCAN_RSP_DATA_UDD_EN_2", "GAP_EXT_SCAN_RSP_DATA_UDD_2", ])

    # Ext Advertising Data length
    gapExtScanRspDataLen = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_RSP_DATA_LEN_2", gapExtAdvAdvSet2)
    gapExtScanRspDataLen.setLabel("Scan Response Data Len")
    gapExtScanRspDataLen.setDefaultValue(0)
    gapExtScanRspDataLen.setVisible(False)

    gapExtScanRspDataValue = libBLEStackComponent.createCommentSymbol("GAP_EXT_SCAN_RSP_DATA_VALUE_2", gapExtAdvAdvSet2)
    gapExtScanRspDataValue.setVisible(False)
    
    gapGenExtScanRspData(gapExtScanRspData)

    #Advertising Data Menu
    menuAdvData = libBLEStackComponent.createMenuSymbol('GAP_ADV_DATA_MENU', gapAdvertising)
    menuAdvData.setLabel('Advertising Data')
    menuAdvData.setVisible(True)
    menuAdvData.setDescription("Advertising Data")
    menuAdvData.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])
    
    #Advertising Data-Flag Bool
    gapAdvDataFlagEn = libBLEStackComponent.createBooleanSymbol('GAP_ADV_DATA_FLAG_EN', menuAdvData)
    gapAdvDataFlagEn.setLabel('Flag')
    gapAdvDataFlagEn.setDefaultValue(True)
    gapAdvDataFlagEn.setVisible(True)
    gapAdvDataFlagEn.setDescription("Flag")
    gapAdvDataFlagEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])

    #Advertising Data-Flag
    gapAdvDataFlag = libBLEStackComponent.createKeyValueSetSymbol('GAP_ADV_DATA_FLAG', gapAdvDataFlagEn)
    gapAdvDataFlag.setLabel(' ')
    gapAdvDataFlag.addKey('0x04', '0', 'LE Limited Discoverable Mode')
    gapAdvDataFlag.addKey('0x05', '1', 'LE General Discoverable Mode')
    gapAdvDataFlag.setDefaultValue(0)
    gapAdvDataFlag.setVisible(True)
    gapAdvDataFlag.setOutputMode('Key')
    gapAdvDataFlag.setDisplayMode('Description')
    gapAdvDataFlag.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])

    #Advertising Data-Local Name Bool    
    gapAdvDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_ADV_DATA_LOCAL_NAME_EN', menuAdvData)
    #print('libBLEStackComponent:gapAdvDataLocalNameEn type({})'.format(type(gapAdvDataLocalNameEn)))
    gapAdvDataLocalNameEn.setLabel('Local Name')
    gapAdvDataLocalNameEn.setDefaultValue(True)
    gapAdvDataLocalNameEn.setVisible(True)
    gapAdvDataLocalNameEn.setDescription("Local Name")
    gapAdvDataLocalNameEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])

    #Advertising Data-Local Name
    gapAdvDataLocalName = libBLEStackComponent.createStringSymbol("GAP_ADV_DATA_LOCAL_NAME", gapAdvDataLocalNameEn)
    gapAdvDataLocalName.setLabel(" ")
    gapAdvDataLocalName.setDefaultValue("pic32cx-bz")
    gapAdvDataLocalName.setVisible(True)
    gapAdvDataLocalName.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])

    #Advertising Data-Service Data Bool
    gapAdvDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_ADV_DATA_SERVICE_DATA_EN', menuAdvData)
    gapAdvDataSvcDataEn.setLabel('Service Data')
    gapAdvDataSvcDataEn.setDefaultValue(False)
    gapAdvDataSvcDataEn.setVisible(True)
    gapAdvDataSvcDataEn.setDescription("Service Data")
    gapAdvDataSvcDataEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])

    # #Advertising Data-Service UUID
    # gapAdvDataSvcUUID = libBLEStackComponent.createStringSymbol("GAP_ADV_DATA_SERVICE_UUID", gapAdvDataSvcDataEn)
    # gapAdvDataSvcUUID.setLabel("Service UUID")
    # gapAdvDataSvcUUID.setDefaultValue("Data in Octets")
    # gapAdvDataSvcUUID.setVisible(False)
    # gapAdvDataSvcUUID.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])

    #Advertising Data-Service UUID
    gapAdvDataSvcUUID = libBLEStackComponent.createHexSymbol("GAP_ADV_DATA_SERVICE_UUID", gapAdvDataSvcDataEn)
    gapAdvDataSvcUUID.setLabel("Service UUID")
    gapAdvDataSvcUUID.setDefaultValue(0xFEDA)
    gapAdvDataSvcUUID.setMax(0xFFFF)
    gapAdvDataSvcUUID.setVisible(False)
    gapAdvDataSvcUUID.setDependencies(gapAdvVisibility, ["GAP_ADV_DATA_SERVICE_DATA_EN"])

    #Advertising Data- Service Data
    gapAdvDataSvcData = libBLEStackComponent.createHexSymbol("GAP_ADV_DATA_SERVICE_DATA", gapAdvDataSvcDataEn)
    gapAdvDataSvcData.setLabel("Service Data")
    #gapAdvDataSvcData.setDefaultValue(0x0304)
    gapAdvDataSvcData.setVisible(False)
    gapAdvDataSvcData.setDependencies(gapAdvVisibility, ["GAP_ADV_DATA_SERVICE_DATA_EN"])

    #Advertising Data-Manufacture Specific Data Bool
    gapAdvDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol('GAP_ADV_DATA_MSD_EN', menuAdvData)
    gapAdvDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
    gapAdvDataManuSpecDataEn.setDefaultValue(False)
    gapAdvDataManuSpecDataEn.setVisible(True)
    gapAdvDataManuSpecDataEn.setDescription("Manufacturer Specific Data")
    gapAdvDataManuSpecDataEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])

    #Advertising Data-Manufacture Specific Data Company Id
    gapAdvDataManuSpecDataCid = libBLEStackComponent.createHexSymbol("GAP_ADV_DATA_MSD_CID", gapAdvDataManuSpecDataEn)
    gapAdvDataManuSpecDataCid.setLabel("Company Id")
    #gapAdvDataManuSpecDataCid.setDefaultValue(0x00CD)
    gapAdvDataManuSpecDataCid.setMax(0xFFFF)
    gapAdvDataManuSpecDataCid.setVisible(False)
    gapAdvDataManuSpecDataCid.setDependencies(gapAdvVisibility, ["GAP_ADV_DATA_MSD_EN"])

    #Advertising Data-Manufacture Specific Data
    gapAdvDataManuSpecData = libBLEStackComponent.createHexSymbol("GAP_ADV_DATA_MSD", gapAdvDataManuSpecDataEn)
    gapAdvDataManuSpecData.setLabel("Manufacturer Data")
    #gapAdvDataManuSpecData.setDefaultValue(0x0405)
    gapAdvDataManuSpecData.setVisible(False)
    gapAdvDataManuSpecData.setDependencies(gapAdvVisibility, ["GAP_ADV_DATA_MSD_EN"])

    #Advertising Data-User Defined Data Bool
    gapAdvDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_ADV_DATA_UDD_EN', menuAdvData)
    gapAdvDataUserDefEn.setLabel('User Defined Data')
    gapAdvDataUserDefEn.setDefaultValue(False)
    gapAdvDataUserDefEn.setVisible(True)
    gapAdvDataUserDefEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])

    #Advertising Data-User Defined Data
    gapAdvDataUserDef = libBLEStackComponent.createHexSymbol("GAP_ADV_DATA_UDD", gapAdvDataUserDefEn)
    gapAdvDataUserDef.setLabel("User Data")
    #gapAdvDataUserDef.setDefaultValue(0x0607)
    gapAdvDataUserDef.setVisible(False)
    gapAdvDataUserDef.setDependencies(gapAdvVisibility, ["GAP_ADV_DATA_UDD_EN"])

    # Advertising Data
    gapAdvertisingData = libBLEStackComponent.createStringSymbol("GAP_ADV_DATA", gapAdvertising)
    gapAdvertisingData.setLabel("Advertising Data")
    gapAdvertisingData.setDefaultValue("Advertising Data")
    gapAdvertisingData.setVisible(False)
    gapAdvertisingData.setDependencies(gapSetAdvData, ["GAP_ADV_DATA_FLAG_EN", "GAP_ADV_DATA_FLAG", 
                                                       "GAP_ADV_DATA_LOCAL_NAME_EN", "GAP_ADV_DATA_LOCAL_NAME", 
                                                       "GAP_ADV_DATA_SERVICE_DATA_EN", 
                                                       "GAP_ADV_DATA_SERVICE_UUID", 
                                                       "GAP_ADV_DATA_SERVICE_DATA", 
                                                       "GAP_ADV_DATA_MSD_EN", "GAP_ADV_DATA_MSD", "GAP_ADV_DATA_MSD_CID", 
                                                       "GAP_ADV_DATA_UDD_EN", "GAP_ADV_DATA_UDD", ])

    # Advertising Data length
    gapAdvertisingDataLen = libBLEStackComponent.createIntegerSymbol("GAP_ADV_DATA_LEN", gapAdvertising)
    gapAdvertisingDataLen.setLabel("Advertising Data Len")
    gapAdvertisingDataLen.setDefaultValue(0)
    gapAdvertisingDataLen.setVisible(False)

    gapAdvertisingDataValue = libBLEStackComponent.createCommentSymbol("GAP_ADV_DATA_VALUE", gapAdvertising)
    gapAdvertisingDataValue.setVisible(False)
    
    gapGenAdvData(gapAdvertisingData)

    #Scan Response Data
    menuScanRspData = libBLEStackComponent.createMenuSymbol('GAP_SCAN_RSP_DATA_MENU', gapAdvertising)
    menuScanRspData.setLabel('Scan Response Data')
    menuScanRspData.setVisible(True)
    menuScanRspData.setDescription("Scan Response Data")
    menuScanRspData.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])

    #Scan Response Data-Local Name Bool    
    gapScanRspDataLocalNameEn = libBLEStackComponent.createBooleanSymbol('GAP_SCAN_RSP_DATA_LOCAL_NAME_EN', menuScanRspData)
    gapScanRspDataLocalNameEn.setLabel('Local Name')
    gapScanRspDataLocalNameEn.setDefaultValue(True)
    gapScanRspDataLocalNameEn.setDescription("Local Name")

    #Scan Response Data-Local Name
    gapScanRspDataLocalName = libBLEStackComponent.createStringSymbol("GAP_SCAN_RSP_DATA_LOCAL_NAME", gapScanRspDataLocalNameEn)
    gapScanRspDataLocalName.setLabel(" ")
    gapScanRspDataLocalName.setDefaultValue("pic32cx-bz")
    gapScanRspDataLocalName.setDependencies(gapLegacyAdvVisibility, ["GAP_SCAN_RSP_DATA_LOCAL_NAME_EN"])

    #Scan Response Data-Service Data Bool
    gapScanRspDataSvcDataEn = libBLEStackComponent.createBooleanSymbol('GAP_SCAN_RSP_DATA_SERVICE_DATA_EN', menuScanRspData)
    gapScanRspDataSvcDataEn.setLabel('Service Data')
    gapScanRspDataSvcDataEn.setDefaultValue(False)
    gapScanRspDataSvcDataEn.setVisible(True)
    gapScanRspDataSvcDataEn.setDescription("Service Data")
    gapScanRspDataSvcDataEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])

    #Scan Response Data-Service UUID
    gapScanRspDataSvcUUID = libBLEStackComponent.createHexSymbol("GAP_SCAN_RSP_DATA_SERVICE_UUID", gapScanRspDataSvcDataEn)
    gapScanRspDataSvcUUID.setLabel("Service UUID")
    gapScanRspDataSvcUUID.setDefaultValue(0xFEDA)
    gapScanRspDataSvcUUID.setMax(0xFFFF)
    gapScanRspDataSvcUUID.setVisible(False)
    gapScanRspDataSvcUUID.setDependencies(gapAdvVisibility, ["GAP_SCAN_RSP_DATA_SERVICE_DATA_EN"])
    
    #Scan Response Data- Service Data
    gapScanRspDataSvcData = libBLEStackComponent.createHexSymbol("GAP_SCAN_RSP_DATA_SERVICE_DATA", gapScanRspDataSvcDataEn)
    gapScanRspDataSvcData.setLabel("Service Data")
    #gapScanRspDataSvcData.setDefaultValue(0x3344)
    gapScanRspDataSvcData.setVisible(False)
    gapScanRspDataSvcData.setDependencies(gapAdvVisibility, ["GAP_SCAN_RSP_DATA_SERVICE_DATA_EN"])
    
    #Scan Response Data-Manufacture Specific Data Bool
    gapScanRspDataManuSpecDataEn = libBLEStackComponent.createBooleanSymbol('GAP_SCAN_RSP_DATA_MSD_EN', menuScanRspData)
    gapScanRspDataManuSpecDataEn.setLabel('Manufacturer Specific Data')
    gapScanRspDataManuSpecDataEn.setDefaultValue(False)
    gapScanRspDataManuSpecDataEn.setVisible(True)
    gapScanRspDataManuSpecDataEn.setDescription("Manufacturer Specific Data")
    gapScanRspDataManuSpecDataEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])
    
    #Scan Response Data-Manufacture Specific Data Company Id
    gapScanRspDataManuSpecDataCid = libBLEStackComponent.createHexSymbol("GAP_SCAN_RSP_DATA_MSD_CID", gapScanRspDataManuSpecDataEn)
    gapScanRspDataManuSpecDataCid.setLabel("Company Id")
    #gapScanRspDataManuSpecDataCid.setDefaultValue(0x00CD)
    gapScanRspDataManuSpecDataCid.setVisible(False)
    gapScanRspDataManuSpecDataCid.setMax(0xFFFF)
    gapScanRspDataManuSpecDataCid.setDependencies(gapAdvVisibility, ["GAP_SCAN_RSP_DATA_MSD_EN"])

    #Scan Response Data-Manufacture Specific Data
    gapScanRspDataManuSpecData = libBLEStackComponent.createHexSymbol("GAP_SCAN_RSP_DATA_MSD", gapScanRspDataManuSpecDataEn)
    gapScanRspDataManuSpecData.setLabel("Manufacture Data")
    #gapScanRspDataManuSpecData.setDefaultValue(0x5566)
    gapScanRspDataManuSpecData.setVisible(False)
    gapScanRspDataManuSpecData.setDependencies(gapAdvVisibility, ["GAP_SCAN_RSP_DATA_MSD_EN"])
    
    #Scan Response Data-User Defined Data Bool
    gapScanRspDataUserDefEn = libBLEStackComponent.createBooleanSymbol('GAP_SCAN_RSP_DATA_UDD_EN', menuScanRspData)
    gapScanRspDataUserDefEn.setLabel('User Defined Data')
    gapScanRspDataUserDefEn.setDefaultValue(False)
    gapScanRspDataUserDefEn.setVisible(True)
    gapScanRspDataUserDefEn.setDependencies(gapLegacyAdvVisibility, ["BOOL_GAP_EXT_ADV","GAP_ADVERTISING"])
    
    #Scan Response Data-User Defined Data
    gapScanRspDataUserDef = libBLEStackComponent.createHexSymbol("GAP_SCAN_RSP_DATA_UDD", gapScanRspDataUserDefEn)
    gapScanRspDataUserDef.setLabel(" ")
    #gapScanRspDataUserDef.setDefaultValue(0x7788)
    gapScanRspDataUserDef.setVisible(False)
    gapScanRspDataUserDef.setDependencies(gapAdvVisibility, ["GAP_SCAN_RSP_DATA_UDD_EN"])
    
    # Scan Response Data
    gapScanRspData = libBLEStackComponent.createStringSymbol("GAP_SCAN_RSP_DATA", gapAdvertising)
    gapScanRspData.setLabel("Scan Response Data")
    gapScanRspData.setVisible(False)
    gapScanRspData.setDependencies(gapSetScanRspData, ["GAP_SCAN_RSP_DATA_LOCAL_NAME_EN","GAP_SCAN_RSP_DATA_LOCAL_NAME",
                                                       "GAP_SCAN_RSP_DATA_SERVICE_DATA_EN", 
                                                       "GAP_SCAN_RSP_DATA_SERVICE_UUID", 
                                                       "GAP_SCAN_RSP_DATA_SERVICE_DATA", 
                                                       "GAP_SCAN_RSP_DATA_MSD_EN", "GAP_SCAN_RSP_DATA_MSD", "GAP_SCAN_RSP_DATA_MSD_CID",  
                                                       "GAP_SCAN_RSP_DATA_UDD_EN", "GAP_SCAN_RSP_DATA_UDD", ])

    # Advertising Data length
    gapScanRspDataLen = libBLEStackComponent.createIntegerSymbol("GAP_SCAN_RSP_DATA_LEN", gapAdvertising)
    gapScanRspDataLen.setLabel("Scan Response Data Len")
    gapScanRspDataLen.setDefaultValue(0)
    gapScanRspDataLen.setVisible(False)

    gapScanRspDataValue = libBLEStackComponent.createCommentSymbol("GAP_SCAN_RSP_DATA_VALUE", gapAdvertising)
    gapScanRspDataValue.setVisible(False)
    
    gapGenScanRspData(gapScanRspData)

    #####################
    # Scan
    #####################
    gapScan = libBLEStackComponent.createBooleanSymbol('GAP_SCAN', menuGAP)
    gapScan.setLabel('Scan')
    gapScan.setDefaultValue(False)
    gapScan.setVisible(True)
    gapScan.setDescription("OBSERVER ROLE CONFIGURATION")

    #Extended Scan
    gapExtScanEn = libBLEStackComponent.createBooleanSymbol('BOOL_GAP_EXT_SCAN', gapScan)
    gapExtScanEn.setLabel('Extended Scan')
    gapExtScanEn.setDefaultValue(False)
    gapExtScanEn.setVisible(False)
    gapExtScanEn.setDescription("Extended Scan")
    gapExtScanEn.setDependencies(gapScanVisibility, ["GAP_SCAN"])
    
    gapExtScanDataLen = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_DATA_LEN", gapExtScanEn)
    gapExtScanDataLen.setLabel("Maximum Data Len")
    gapExtScanDataLen.setDefaultValue(74)
    gapExtScanDataLen.setMin(74)
    gapExtScanDataLen.setMax(1650)
    gapExtScanDataLen.setVisible(False)
    gapExtScanDataLen.setDependencies(gapScanVisibility, ["BOOL_GAP_EXT_SCAN"])

    gapExtScanNum2Pkt = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_NUM_2_PKT", gapExtScanEn)
    gapExtScanNum2Pkt.setLabel("Maximum Num of Secondary Packets")
    gapExtScanNum2Pkt.setDefaultValue(0)
    gapExtScanNum2Pkt.setMin(0)
    gapExtScanNum2Pkt.setMax(16)
    gapExtScanNum2Pkt.setVisible(False)
    gapExtScanNum2Pkt.setDependencies(gapScanVisibility, ["BOOL_GAP_EXT_SCAN"]) 

    #Ext Scan Filter Policy
    gapScanFiltPolicy = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_SCAN_FILT_POLICY', gapExtScanEn)
    gapScanFiltPolicy.setLabel('Scan Filter Policy')
    gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_ALL', '0', 'Accept all advertising and scan response PDUs except directed advertising PDUs not addressed to this device')
    gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_LIST', '1', 'Accept only advertising and scan response PDUs from devices where the advertisers address is in the White List')
    gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_NOT_RESOLVED_DIRECTED', '2', 'Accept all advertising and scan response PDUs except directed advertising PDUs where the identity address corresponding to TargetA does not address this device')
    gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_LIST_NOT_RESOLVED_DIRECTED', '3', 'Accept all advertising and scan response PDUs except: the advertiser identity address is not in the White List and TargetA does not address this device')
    gapScanFiltPolicy.setDefaultValue(0)
    gapScanFiltPolicy.setOutputMode('Key')
    gapScanFiltPolicy.setDisplayMode('Description')
    gapScanFiltPolicy.setVisible(False)
    gapScanFiltPolicy.setDependencies(gapScanVisibility, ["BOOL_GAP_EXT_SCAN"])

    #Ext Scan PHY
    gapScanPhy = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_SCAN_PHY', gapExtScanEn)
    gapScanPhy.setLabel('Scan PHY')
    gapScanPhy.addKey('BLE_GAP_SCAN_PHY_1M', '0', 'Scan advertisements on the LE 1M PHY')
    gapScanPhy.addKey('BLE_GAP_SCAN_PHY_CODED', '1', 'Scan advertisements on the LE Coded PHY')
    gapScanPhy.addKey('BLE_GAP_SCAN_PHY_1M|BLE_GAP_SCAN_PHY_CODED', '2', 'Scan advertisements on the LE 1M PHY and LE Coded PHY')
    gapScanPhy.setDefaultValue(0)
    gapScanPhy.setOutputMode('Key')
    gapScanPhy.setDisplayMode('Description')
    gapScanPhy.setVisible(False)
    gapScanPhy.setDependencies(gapScanVisibility, ["BOOL_GAP_EXT_SCAN"])
    
    # Ext 1M PHY
    menuScanPhy = libBLEStackComponent.createMenuSymbol('GAP_EXT_SCAN_PHY_1M', gapScanPhy)
    menuScanPhy.setLabel('LE 1M PHY')
    menuScanPhy.setDescription("LE 1M PHY")
    menuScanPhy.setVisible(gapScanPhy.getDefaultValue() == 0 or gapScanPhy.getDefaultValue() == 2)
    menuScanPhy.setDependencies(gapExtScanPhy1mVisibility, ["GAP_EXT_SCAN_PHY"])
    
    # Ext Advertising Type 1M
    gapScanType = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_SCAN_TYPE_1M', menuScanPhy)
    gapScanType.setLabel('Scan Type')
    gapScanType.addKey('BLE_GAP_SCAN_TYPE_PASSIVE_SCAN', '0', 'Passive Scanning')
    gapScanType.addKey('BLE_GAP_SCAN_TYPE_ACTIVE_SCAN', '1', 'Active Scanning')
    gapScanType.setDefaultValue(0)
    gapScanType.setOutputMode('Key')
    gapScanType.setDisplayMode('Description')
    
    # Ext Scan Interval 1M
    gapScanInterval = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_INTERVAL_1M", menuScanPhy)
    gapScanInterval.setLabel("Scan Interval(Unit: 0.625ms)")
    gapScanInterval.setDefaultValue(160)
    gapScanInterval.setMin(1)
    gapScanInterval.setMax(65535)

    # Ext Scan Window 1M
    gapScanWindow = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_WINDOW_1M", menuScanPhy)
    gapScanWindow.setLabel("Scan Window(Unit: 0.625ms)")
    gapScanWindow.setDefaultValue(32)
    gapScanWindow.setMin(1)
    gapScanWindow.setMax(65535)

    # Ext Coded PHY
    menuScanPhy = libBLEStackComponent.createMenuSymbol('GAP_EXT_SCAN_PHY_CODED', gapScanPhy)
    menuScanPhy.setLabel('LE Coded PHY')
    menuScanPhy.setDescription("LE Coded PHY")
    menuScanPhy.setVisible(gapScanPhy.getDefaultValue() == 1 or gapScanPhy.getDefaultValue() == 2)
    menuScanPhy.setDependencies(gapExtScanPhyCodedVisibility, ["GAP_EXT_SCAN_PHY"])
    
    # Ext Advertising Type Coded
    gapScanType = libBLEStackComponent.createKeyValueSetSymbol('GAP_EXT_SCAN_TYPE_CODED', menuScanPhy)
    gapScanType.setLabel('Scan Type')
    gapScanType.addKey('BLE_GAP_SCAN_TYPE_PASSIVE_SCAN', '0', 'Passive Scanning')
    gapScanType.addKey('BLE_GAP_SCAN_TYPE_ACTIVE_SCAN', '1', 'Active Scanning')
    gapScanType.setDefaultValue(0)
    gapScanType.setOutputMode('Key')
    gapScanType.setDisplayMode('Description')
    
    # Ext Scan Interval Coded
    gapScanInterval = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_INTERVAL_CODED", menuScanPhy)
    gapScanInterval.setLabel("Scan Interval(Unit: 0.625ms)")
    gapScanInterval.setDefaultValue(160)
    gapScanInterval.setMin(1)
    gapScanInterval.setMax(65535)

    # Ext Scan Window Coded
    gapScanWindow = libBLEStackComponent.createIntegerSymbol("GAP_EXT_SCAN_WINDOW_CODED", menuScanPhy)
    gapScanWindow.setLabel("Scan Window(Unit: 0.625ms)")
    gapScanWindow.setDefaultValue(32)
    gapScanWindow.setMin(1)
    gapScanWindow.setMax(65535)

    
    # Advertising Type
    gapScanType = libBLEStackComponent.createKeyValueSetSymbol('GAP_SCAN_TYPE', gapScan)
    gapScanType.setLabel('Scan Type')
    gapScanType.addKey('BLE_GAP_SCAN_TYPE_PASSIVE_SCAN', '0', 'Passive Scanning')
    gapScanType.addKey('BLE_GAP_SCAN_TYPE_ACTIVE_SCAN', '1', 'Active Scanning')
    gapScanType.setDefaultValue(0)
    gapScanType.setOutputMode('Key')
    gapScanType.setDisplayMode('Description')
    gapScanType.setVisible(False)
    gapScanType.setDependencies(gapLegacyScanVisibility, ["GAP_SCAN","BOOL_GAP_EXT_SCAN"])
    
    # Scan Interval
    gapScanInterval = libBLEStackComponent.createIntegerSymbol("GAP_SCAN_INTERVAL", gapScan)
    gapScanInterval.setLabel("Scan Interval(Unit: 0.625ms)")
    gapScanInterval.setDefaultValue(160)
    gapScanInterval.setMin(1)
    gapScanInterval.setMax(65535)
    gapScanInterval.setVisible(False)
    gapScanInterval.setDependencies(gapLegacyScanVisibility, ["GAP_SCAN","BOOL_GAP_EXT_SCAN"])

    # Scan Window
    gapScanWindow = libBLEStackComponent.createIntegerSymbol("GAP_SCAN_WINDOW", gapScan)
    gapScanWindow.setLabel("Scan Window(Unit: 0.625ms)")
    gapScanWindow.setDefaultValue(32)
    gapScanWindow.setMin(1)
    gapScanWindow.setMax(65535)
    gapScanWindow.setVisible(False)
    gapScanWindow.setDependencies(gapLegacyScanVisibility, ["GAP_SCAN","BOOL_GAP_EXT_SCAN"]) 

    # Scan Filter Policy
    gapScanFiltPolicy = libBLEStackComponent.createKeyValueSetSymbol('GAP_SCAN_FILT_POLICY', gapScan)
    gapScanFiltPolicy.setLabel('Scan Filter Policy')
    gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_ALL', '0', 'Accept all advertising and scan response PDUs except directed advertising PDUs not addressed to this device')
    gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_LIST', '1', 'Accept only advertising and scan response PDUs from devices where the advertisers address is in the White List')
    gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_NOT_RESOLVED_DIRECTED', '2', 'Accept all advertising and scan response PDUs except directed advertising PDUs where the identity address corresponding to TargetA does not address this device')
    gapScanFiltPolicy.addKey('BLE_GAP_SCAN_FP_ACCEPT_LIST_NOT_RESOLVED_DIRECTED', '3', 'Accept all advertising and scan response PDUs except: the advertiser identity address is not in the White List and TargetA does not address this device')
    gapScanFiltPolicy.setDefaultValue(0)
    gapScanFiltPolicy.setOutputMode('Key')
    gapScanFiltPolicy.setDisplayMode('Description')
    gapScanFiltPolicy.setVisible(False)
    gapScanFiltPolicy.setDependencies(gapLegacyScanVisibility, ["GAP_SCAN","BOOL_GAP_EXT_SCAN"])

    #####################
    # Peripheral Role
    #####################
    gapPeripheralRole = libBLEStackComponent.createBooleanSymbol('GAP_PERIPHERAL', menuGAP)
    gapPeripheralRole.setLabel('Peripheral')
    gapPeripheralRole.setDefaultValue(True)
    gapPeripheralRole.setVisible(True)
    gapPeripheralRole.setDescription("PERIPHERAL ROLE CONFIGURATION")
    
    #####################
    # Central Role
    #####################
    gapCentralRole = libBLEStackComponent.createBooleanSymbol('GAP_CENTRAL', menuGAP)
    gapCentralRole.setLabel('Central')
    gapCentralRole.setDefaultValue(False)
    gapCentralRole.setVisible(True)
    gapCentralRole.setDescription("CENTRAL ROLE CONFIGURATION")

    #####################
    # Direct Test Mode
    #####################
    gapDirectTestMode = libBLEStackComponent.createBooleanSymbol('GAP_DIRECT_TEST_MODE', menuGAP)
    gapDirectTestMode.setLabel('Direct Test Mode')
    gapDirectTestMode.setDefaultValue(False)
    gapDirectTestMode.setVisible(True)
    gapDirectTestMode.setDescription("Include Direct Test Mode module")
    
    #####################
    # Connection Tx Power
    #####################
    gapConnTxPwr = libBLEStackComponent.createIntegerSymbol('GAP_CONN_TX_PWR', menuGAP)
    gapConnTxPwr.setLabel('Connection Tx Power (dBm)')
    gapConnTxPwr.setDefaultValue(gapGenDefTxPwr(False))
    gapConnTxPwr.setMax(gapGenMaxTxPwr(False))
    gapConnTxPwr.setMin(gapGenMinTxPwr())
    gapConnTxPwr.setVisible(True)
    gapConnTxPwr.setDescription("Connection Tx Power")
    #################################################################
    ##################         GATT Setting           ###############
    #################################################################
    menuGATT = libBLEStackComponent.createMenuSymbol('GATT_MENU', None)
    menuGATT.setLabel('Generic Attribute Profile (GATT)')
    menuGATT.setVisible(True)
    menuGATT.setDescription("GATT SETTINGS")
    
    global gattServerRole
    gattServerRole = libBLEStackComponent.createMenuSymbol('BLE_MENU_GATT_SERVER', menuGATT)
    gattServerRole.setLabel('Server Role')
    
    gattCachingEnable = libBLEStackComponent.createBooleanSymbol('BLE_BOOL_GATT_CACHING', gattServerRole)
    gattCachingEnable.setLabel('Enable GATT Caching')
    gattCachingEnable.setDefaultValue(False)
    gattCachingEnable.setVisible(True)
    
    gattSvc = libBLEStackComponent.createBooleanSymbol('BLE_BOOL_GATT_SVC_CHANGE', gattServerRole)
    gattSvc.setLabel('Enable GATT Service Change')
    gattSvc.setDefaultValue(False)
    gattSvc.setVisible(True)

    global gattClientRole     
    gattClientRole = libBLEStackComponent.createBooleanSymbol('BLE_BOOL_GATT_CLIENT', menuGATT)
    gattClientRole.setLabel('Enable Client Role')
    gattClientRole.setDefaultValue(False)

    gattManuConfirm = libBLEStackComponent.createBooleanSymbol('BLE_BOOL_GATT_MANU_CONFIRM', gattClientRole)
    gattManuConfirm.setLabel('Manual Handle Confirmation')
    gattManuConfirm.setDefaultValue(False)
    gattManuConfirm.setVisible(False)
    gattManuConfirm.setDependencies(gattManuConfirmVisibility, ["BLE_BOOL_GATT_CLIENT"])
    
    #################################################################
    ##################         SMP Setting           ###############
    #################################################################
    menuSMP = libBLEStackComponent.createMenuSymbol('SMP_MENU', None)
    menuSMP.setLabel('Security Manager (SM)')
    menuSMP.setVisible(True)
    menuSMP.setDescription("SMP SETTINGS")
    
    # IO Capability
    smpIoCapability = libBLEStackComponent.createKeyValueSetSymbol('SMP_IOCAP_TYPE', menuSMP)
    smpIoCapability.setLabel('IO Capability')
    smpIoCapability.addKey('BLE_SMP_IO_DISPLAYONLY', '0', 'Display Only')
    smpIoCapability.addKey('BLE_SMP_IO_DISPLAYYESNO', '1', 'Display YesNo')
    smpIoCapability.addKey('BLE_SMP_IO_KEYBOARDONLY', '2', 'Keyboard Only')
    smpIoCapability.addKey('BLE_SMP_IO_NOINPUTNOOUTPUT', '3', 'NoInputNoOutput')
    smpIoCapability.addKey('BLE_SMP_IO_KEYBOARDDISPLAY', '4', 'Keyboard Display')
    smpIoCapability.setDefaultValue(3)
    smpIoCapability.setOutputMode('Key')
    smpIoCapability.setDisplayMode('Description')

    # Authentication Requirement
    smpAuthReqManu = libBLEStackComponent.createMenuSymbol('SMP_AUTH_REQ', menuSMP)
    smpAuthReqManu.setLabel('Authentication Setting')
    smpAuthReqManu.setVisible(True)
    smpAuthReqManu.setDescription("SMP Authentication Requirement")

    smpAuthReqBond = libBLEStackComponent.createBooleanSymbol('SMP_AUTH_REQ_BOND', smpAuthReqManu)
    smpAuthReqBond.setLabel('Bonding')
    smpAuthReqBond.setDefaultValue(True)
    smpAuthReqBond.setVisible(True)

    smpAuthReqMitm = libBLEStackComponent.createBooleanSymbol('SMP_AUTH_REQ_MITM', smpAuthReqManu)
    smpAuthReqMitm.setLabel('MITM')
    smpAuthReqMitm.setDefaultValue(False)
    smpAuthReqMitm.setVisible(True)

    smpAuthReqSC = libBLEStackComponent.createBooleanSymbol('SMP_AUTH_REQ_SC', smpAuthReqManu)
    smpAuthReqSC.setLabel('Secure Connections')
    smpAuthReqSC.setDefaultValue(True)
    smpAuthReqSC.setVisible(True) 

    smpOob = libBLEStackComponent.createBooleanSymbol('SMP_OOB', menuSMP)
    smpOob.setLabel('OOB Authentication Data Present')
    smpOob.setDefaultValue(False)
    smpOob.setReadOnly(True)
    smpOob.setVisible(True)

    smpSCOnly = libBLEStackComponent.createBooleanSymbol('SMP_SC_ONLY', menuSMP)
    smpSCOnly.setLabel('Secure Connections Only')
    smpSCOnly.setDefaultValue(False)
    smpSCOnly.setReadOnly(False)
    smpSCOnly.setVisible(True)
    smpSCOnly.setDependencies(smpSCOnlyVisibility, ["SMP_AUTH_REQ_SC"])
                                           
    #################################################################
    ##################         L2CAP Setting          ###############
    #################################################################
    menuL2CAP = libBLEStackComponent.createMenuSymbol('L2CAP_MENU', None)
    menuL2CAP.setLabel('Logic Link Control And Adaption Protocol (L2CAP)')
    menuL2CAP.setVisible(True)
    menuL2CAP.setDescription("L2CAP SETTINGS")

    enableL2CAPCreditFlowCtrl = libBLEStackComponent.createBooleanSymbol('BOOL_L2CAP_CREDIT_FLOWCTRL', menuL2CAP)
    enableL2CAPCreditFlowCtrl.setLabel('Credit Based Flow Control')
    enableL2CAPCreditFlowCtrl.setDefaultValue(False)

    #################################################################
    ##################      DM Settings               ###############
    #################################################################
    menuDm = libBLEStackComponent.createMenuSymbol('DM_MENU', None)
    menuDm.setLabel('Device Manager (DM)')
    menuDm.setVisible(True)
    menuDm.setDescription("DM SETTINGS")

    dmSecAutoAccept = libBLEStackComponent.createBooleanSymbol('BOOL_DM_SEC_AUTO_ACCEPT', menuDm)
    dmSecAutoAccept.setLabel('Auto Accept Security Request')
    dmSecAutoAccept.setDefaultValue(True)

    dmConnAudoAccept = libBLEStackComponent.createBooleanSymbol('BOOL_DM_CONN_AUTO_ACCEPT', menuDm)
    dmConnAudoAccept.setLabel('Auto Accept Connect Parameters Update Request')
    dmConnAudoAccept.setDefaultValue(True)

    dmMinConnInterval = libBLEStackComponent.createIntegerSymbol("DM_MIN_CONN_INTERVAL", dmConnAudoAccept)
    dmMinConnInterval.setLabel("Minimum Connection Interval (Unit: 1.25ms)")
    dmMinConnInterval.setDefaultValue(6)
    dmMinConnInterval.setMin(6)
    dmMinConnInterval.setMax(3200)
    dmMinConnInterval.setVisible(True)
    dmMinConnInterval.setDependencies(dmConnVisibility, ["BOOL_DM_CONN_AUTO_ACCEPT"]) 

    dmMaxConnInterval = libBLEStackComponent.createIntegerSymbol("DM_MAX_CONN_INTERVAL", dmConnAudoAccept)
    dmMaxConnInterval.setLabel("Maximum Connection Interval (Unit: 1.25ms)")
    dmMaxConnInterval.setDefaultValue(3200)
    dmMaxConnInterval.setMin(6)
    dmMaxConnInterval.setMax(3200)
    dmMaxConnInterval.setVisible(True)
    dmMaxConnInterval.setDependencies(dmConnVisibility, ["BOOL_DM_CONN_AUTO_ACCEPT"]) 

    dmMinConnLatency = libBLEStackComponent.createIntegerSymbol("DM_MIN_CONN_LATENCY", dmConnAudoAccept)
    dmMinConnLatency.setLabel("Minimum Connection Latency")
    dmMinConnLatency.setDefaultValue(0)
    dmMinConnLatency.setMin(0)
    dmMinConnLatency.setMax(499)
    dmMinConnLatency.setVisible(True)
    dmMinConnLatency.setDependencies(dmConnVisibility, ["BOOL_DM_CONN_AUTO_ACCEPT"])     

    dmMaxConnLatency = libBLEStackComponent.createIntegerSymbol("DM_MAX_CONN_LATENCY", dmConnAudoAccept)
    dmMaxConnLatency.setLabel("Maximum Connection Latency")
    dmMaxConnLatency.setDefaultValue(499)
    dmMaxConnLatency.setMin(0)
    dmMaxConnLatency.setMax(499)
    dmMaxConnLatency.setVisible(True)
    dmMaxConnLatency.setDependencies(dmConnVisibility, ["BOOL_DM_CONN_AUTO_ACCEPT"])     

    #################################################################
    ##################      GCM Settings               ###############
    #################################################################
    menuGcm = libBLEStackComponent.createMenuSymbol('GCM_MENU', None)
    menuGcm.setLabel('GATT Client Manager (GCM)')
    menuGcm.setVisible(False)
    menuGcm.setDescription("GCM SETTINGS")
    menuGcm.setDependencies(gcmVisible, ["BLE_BOOL_GATT_CLIENT"])

    gcmScm = libBLEStackComponent.createBooleanSymbol('BOOL_GCM_SCM', menuGcm)
    gcmScm.setLabel('Service Change Manager')
    gcmScm.setDefaultValue(False)


    #################################################################
    ##################         Profile Setting          #############
    #################################################################    
    global AppTrpServerEnable
    AppTrpServerEnable = libBLEStackComponent.createBooleanSymbol('APP_TRP_SERVER', None)
    AppTrpServerEnable.setLabel("App trps")
    AppTrpServerEnable.setVisible(False)
    AppTrpServerEnable.setDefaultValue(False)
    AppTrpServerEnable.setDependencies(appTrpsEnable, ["profileBLE_TRP.TRP_BOOL_SERVER"])

    global AppTrpClientEnable
    AppTrpClientEnable = libBLEStackComponent.createBooleanSymbol('APP_TRP_CLIENT', None)
    AppTrpClientEnable.setLabel("App trpc")
    AppTrpClientEnable.setVisible(False)
    AppTrpClientEnable.setDefaultValue(False)
    AppTrpClientEnable.setDependencies(appTrpcEnable, ["profileBLE_TRP.TRP_BOOL_CLIENT"])
    
    global AppTrcbpServerEnable
    AppTrcbpServerEnable = libBLEStackComponent.createBooleanSymbol('APP_TRCBP_SERVER', None)
    AppTrcbpServerEnable.setLabel("App trcbps")
    AppTrcbpServerEnable.setVisible(False)
    AppTrcbpServerEnable.setDefaultValue(False)
    AppTrcbpServerEnable.setDependencies(appTrcbpsEnable, ["profileBLE_TRCP.TRCP_BOOL_SERVER"])    
    
    #################################################################
    ##################        Add Source File         ###############
    #################################################################
    # Add mw_assert.h - static file
    bleMwAssertHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleMwAssertHeaderFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_util/mw_assert.h')
    bleMwAssertHeaderFile.setOutputName('mw_assert.h')
    bleMwAssertHeaderFile.setOverwrite(True)
    bleMwAssertHeaderFile.setDestPath('ble/middleware_ble/ble_util')
    bleMwAssertHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_util')
    bleMwAssertHeaderFile.setType('HEADER')
    bleMwAssertHeaderFile.setEnabled(True)          

    # Add byte_stream.h - static file
    bleByteStreamHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleByteStreamHeaderFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_util/byte_stream.h')
    bleByteStreamHeaderFile.setOutputName('byte_stream.h')
    bleByteStreamHeaderFile.setOverwrite(True)
    bleByteStreamHeaderFile.setDestPath('ble/middleware_ble/ble_util')
    bleByteStreamHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_util')
    bleByteStreamHeaderFile.setType('HEADER')
    bleByteStreamHeaderFile.setEnabled(True)

    # Add ble_dm.h file - static file
    bleDmHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmHeaderFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm.h')
    bleDmHeaderFile.setOutputName('ble_dm.h')
    bleDmHeaderFile.setOverwrite(True)
    bleDmHeaderFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmHeaderFile.setType('HEADER')
    bleDmHeaderFile.setEnabled(True)
    bleDmHeaderFile.setMarkup(True)

    # Add ble_dm_aes.h file - static file
    bleDmAesHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmAesHeaderFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm_aes.h')
    bleDmAesHeaderFile.setOutputName('ble_dm_aes.h')
    bleDmAesHeaderFile.setOverwrite(True)
    bleDmAesHeaderFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmAesHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmAesHeaderFile.setType('HEADER')
    bleDmAesHeaderFile.setEnabled(True)
    bleDmAesHeaderFile.setMarkup(True)

    # Add ble_dm_conn.h file - static file
    bleDmConnHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmConnHeaderFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm_conn.h')
    bleDmConnHeaderFile.setOutputName('ble_dm_conn.h')
    bleDmConnHeaderFile.setOverwrite(True)
    bleDmConnHeaderFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmConnHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmConnHeaderFile.setType('HEADER')
    bleDmConnHeaderFile.setEnabled(True)
    bleDmConnHeaderFile.setMarkup(True)
    
    # Add ble_dm_sm.h file - static file
    bleDmSmHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmSmHeaderFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm_sm.h')
    bleDmSmHeaderFile.setOutputName('ble_dm_sm.h')
    bleDmSmHeaderFile.setOverwrite(True)
    bleDmSmHeaderFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmSmHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmSmHeaderFile.setType('HEADER')
    bleDmSmHeaderFile.setEnabled(True)
    bleDmSmHeaderFile.setMarkup(True)     

    # Add ble_dm_info.h file - static file
    bleDmInfoHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmInfoHeaderFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm_info.h')
    bleDmInfoHeaderFile.setOutputName('ble_dm_info.h')
    bleDmInfoHeaderFile.setOverwrite(True)
    bleDmInfoHeaderFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmInfoHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmInfoHeaderFile.setType('HEADER')
    bleDmInfoHeaderFile.setEnabled(True)
    bleDmInfoHeaderFile.setMarkup(True) 
    
    # Add ble_dm_dds.h file - static file
    bleDmDdsHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmDdsHeaderFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm_dds.h')
    bleDmDdsHeaderFile.setOutputName('ble_dm_dds.h')
    bleDmDdsHeaderFile.setOverwrite(True)
    bleDmDdsHeaderFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmDdsHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmDdsHeaderFile.setType('HEADER')
    bleDmDdsHeaderFile.setEnabled(True)
    bleDmDdsHeaderFile.setMarkup(True)

    # Add ble_dm_internal.h file - static file
    bleDmInternalHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmInternalHeaderFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm_internal.h')
    bleDmInternalHeaderFile.setOutputName('ble_dm_internal.h')
    bleDmInternalHeaderFile.setOverwrite(True)
    bleDmInternalHeaderFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmInternalHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmInternalHeaderFile.setType('HEADER')
    bleDmInternalHeaderFile.setEnabled(True)
    bleDmInternalHeaderFile.setMarkup(True)

    # Add ble_dd.h file - static file
    bleDdHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDdHeaderFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_gcm/ble_dd.h')
    bleDdHeaderFile.setOutputName('ble_dd.h')
    bleDdHeaderFile.setOverwrite(True)
    bleDdHeaderFile.setDestPath('ble/middleware_ble/ble_gcm')
    bleDdHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_gcm')
    bleDdHeaderFile.setType('HEADER')
    bleDdHeaderFile.setEnabled(True)
    bleDdHeaderFile.setMarkup(True)
    bleDdHeaderFile.setDependencies(bleDdEnable, ["BLE_BOOL_GATT_CLIENT"])
    
    # Add ble_scm.h file - static file
    bleScmHeaderFile = libBLEStackComponent.createFileSymbol(None, None)
    bleScmHeaderFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_gcm/ble_scm.h')
    bleScmHeaderFile.setOutputName('ble_scm.h')
    bleScmHeaderFile.setOverwrite(True)
    bleScmHeaderFile.setDestPath('ble/middleware_ble/ble_gcm')
    bleScmHeaderFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_gcm')
    bleScmHeaderFile.setType('HEADER')
    bleScmHeaderFile.setEnabled(True)
    bleScmHeaderFile.setMarkup(True)
       
    # Add ble_dm.c file - static file
    bleDmSourceFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmSourceFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm.c')
    bleDmSourceFile.setOutputName('ble_dm.c')
    bleDmSourceFile.setOverwrite(True)
    bleDmSourceFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmSourceFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmSourceFile.setType('SOURCE')
    bleDmSourceFile.setEnabled(True)
    bleDmSourceFile.setMarkup(True)
    
    # Add ble_dm_aes.c file - static file
    bleDmAesSourceFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmAesSourceFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm_aes.c')
    bleDmAesSourceFile.setOutputName('ble_dm_aes.c')
    bleDmAesSourceFile.setOverwrite(True)
    bleDmAesSourceFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmAesSourceFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmAesSourceFile.setType('SOURCE')
    bleDmAesSourceFile.setEnabled(True)
    bleDmAesSourceFile.setMarkup(True)    

    # Add ble_dm_conn.c file - static file
    bleDmConnSourceFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmConnSourceFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm_conn.c')
    bleDmConnSourceFile.setOutputName('ble_dm_conn.c')
    bleDmConnSourceFile.setOverwrite(True)
    bleDmConnSourceFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmConnSourceFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmConnSourceFile.setType('SOURCE')
    bleDmConnSourceFile.setEnabled(True)
    bleDmConnSourceFile.setMarkup(True) 

    # Add ble_dm_sm.c file - static file
    bleDmSmSourceFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmSmSourceFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm_sm.c')
    bleDmSmSourceFile.setOutputName('ble_dm_sm.c')
    bleDmSmSourceFile.setOverwrite(True)
    bleDmSmSourceFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmSmSourceFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmSmSourceFile.setType('SOURCE')
    bleDmSmSourceFile.setEnabled(True)
    bleDmSmSourceFile.setMarkup(True) 

    # Add ble_dm_info.c file - static file
    bleDmInfoSourceFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmInfoSourceFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm_info.c')
    bleDmInfoSourceFile.setOutputName('ble_dm_info.c')
    bleDmInfoSourceFile.setOverwrite(True)
    bleDmInfoSourceFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmInfoSourceFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmInfoSourceFile.setType('SOURCE')
    bleDmInfoSourceFile.setEnabled(True)
    bleDmInfoSourceFile.setMarkup(True) 

    # Add ble_dm_dds.c file - static file
    bleDmDdsSourceFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDmDdsSourceFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_dm/ble_dm_dds.c')
    bleDmDdsSourceFile.setOutputName('ble_dm_dds.c')
    bleDmDdsSourceFile.setOverwrite(True)
    bleDmDdsSourceFile.setDestPath('ble/middleware_ble/ble_dm')
    bleDmDdsSourceFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_dm')
    bleDmDdsSourceFile.setType('SOURCE')
    bleDmDdsSourceFile.setEnabled(True)
    bleDmDdsSourceFile.setMarkup(True)
    
    global bleDdSourceFile
    # Add ble_dd.c file - static file
    bleDdSourceFile = libBLEStackComponent.createFileSymbol(None, None)
    bleDdSourceFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_gcm/ble_dd.c')
    bleDdSourceFile.setOutputName('ble_dd.c')
    bleDdSourceFile.setOverwrite(True)
    bleDdSourceFile.setDestPath('ble/middleware_ble/ble_gcm')
    bleDdSourceFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_gcm')
    bleDdSourceFile.setType('SOURCE')
    bleDdSourceFile.setEnabled(False)
    bleDdSourceFile.setMarkup(True)
    bleDdSourceFile.setDependencies(bleDdEnable, ["BLE_BOOL_GATT_CLIENT"])

    # Add ble_scm.c file - static file
    bleScmSourceFile = libBLEStackComponent.createFileSymbol(None, None)
    bleScmSourceFile.setSourcePath('driver/ble/src/ble_src/middleware_ble/ble_gcm/ble_scm.c')
    bleScmSourceFile.setOutputName('ble_scm.c')
    bleScmSourceFile.setOverwrite(True)
    bleScmSourceFile.setDestPath('ble/middleware_ble/ble_gcm')
    bleScmSourceFile.setProjectPath('config/' + configName + '/ble/middleware_ble/ble_gcm')
    bleScmSourceFile.setType('SOURCE')
    bleScmSourceFile.setEnabled(False)
    bleScmSourceFile.setMarkup(True)
    bleScmSourceFile.setDependencies(bleScmEnable, ["BLE_BOOL_GATT_CLIENT", "BOOL_GCM_SCM"])


    ############################################################################
    ### Custom Antenna Gain Support
    ############################################################################

    # Custom Antenna Gain Enabled
    customAntennaSpecified = libBLEStackComponent.createBooleanSymbol('USE_CUSTOM_ANT_GAIN', None)
    customAntennaSpecified.setVisible(False)
    customAntennaSpecified.setReadOnly(True)
    customAntennaSpecified.setValue(False)

    # Custom Antenna Gain
    customAntennaGain = libBLEStackComponent.createIntegerSymbol('CUSTOM_ANT_GAIN', None)
    customAntennaGain.setVisible(False)
    customAntennaGain.setReadOnly(True)
    customAntennaGain.setValue(3)

    # Custom Antenna Gain Region
    customAntennaRegion = libBLEStackComponent.createStringSymbol('CUSTOM_ANT_REGION', None)
    customAntennaRegion.setVisible(False)
    customAntennaRegion.setReadOnly(True)
    customAntennaRegion.setValue('none')


def finalizeComponent(BLEStackComponent):
    Log.writeInfoMessage('Finalizing: {}'.format(BLEStackComponent.getID()))
    activeComponents = Database.getActiveComponentIDs()
    requiredComponents = ['pic32cx_bz2_devsupport']
    for r in requiredComponents:
        if r not in activeComponents:
            res = Database.activateComponents([r])


def onAttachmentConnected(source, target):

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    connectID = source["id"]
    targetID = target["id"]
    Log.writeInfoMessage('blestack:onAttachmentConnected: source = {} remote = {}'.
            format(localComponent.getID(), targetID))


    if (connectID == "PIC32CX_BZ2_DevSupport_Dependency"):
        bleSleepEnabled = localComponent.getSymbolByID("BLE_SYS_SLEEP_MODE_EN").getValue()
        sendRTCSupportMessage(bleSleepEnabled)


/*******************************************************************************
  BLE OTA Profile Server Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_otaps.c

  Summary:
    This file contains the BLE OTA Profile Server functions for application user.

  Description:
    This file contains the BLE OTA Profile Server functions for application user.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "osal/osal_freertos.h"
#include "bt_sys.h"
#include "mba_error_defs.h"
#include "ble_gap.h"
#include "gatt.h"
#include "ble_util/byte_stream.h"
#include "ble_otas/ble_otas.h"
#include "ble_otaps/ble_otaps.h"


// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************

#define BLE_OTAPS_OP_CODE_RESPONSE            0x01
#define BLE_OTAPS_OP_CODE_UPDATE_REQUEST      0x02
#define BLE_OTAPS_OP_CODE_UPDATE_START        0x03
#define BLE_OTAPS_OP_CODE_UPDATE_COMPLETE     0x04
#define BLE_OTAPS_OP_CODE_RESET_REQUEST       0x05

#define BLE_OTAPS_UPDATE_REQ_SIZE             0x08
#define BLE_OTAPS_UPDATE_START_SIZE           0x01


#define BLE_OTAPS_STATE_CCCD_DISABLED         0x00
#define BLE_OTAPS_STATE_CCCD_ENABLED          0x01
#define BLE_OTAPS_STATE_UPDATE_REQ            0x02
#define BLE_OTAPS_STATE_UPDATE_START          0x03
#define BLE_OTAPS_STATE_COMPLETE              0x04


#define BLE_OTAPS_RESULT_SUCCESS              0x00
#define BLE_OTAPS_RESULT_INVALID_STATE        0x01
#define BLE_OTAPS_RESULT_NOT_SUPPORTED        0x02
#define BLE_OTAPS_RESULT_OPERATION_FAILED     0x03
#define BLE_OTAPS_RESULT_INVALID_PARA         0x04

#define BLE_OTAPS_IMG_BUF_SIZE                0x200

#define BLE_OTAPS_APP_VER_OFFSET              0x21FC
#define BLE_OTAPS_MCU_VER_LEN                 0x04
#define BLE_OTAPS_DSP_VER_SIZE                0x04

#define BLE_OTAPS_FACTORY_CFG_IDX_SIZE        0x1
#define BLE_OTAPS_FACTORY_CFG_ADDR            0x6000

#define BLE_OTAPS_CCCD_CTRL_ON                (1 << 0)
#define BLE_OTAPS_CCCD_DATA_ON                (1 << 1)
// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

static BLE_OTAPS_ProcessRoutine sp_otapsCbRoutine;
static uint8_t s_otapsAppAllowed;
static uint8_t s_otapsState;
static uint8_t * sp_otapsPktBuf;
static uint8_t s_otapsAesIv[16];
static uint8_t s_otapsAesKey[32];
static uint8_t *sp_otapsPlainBuf;
static uint8_t s_otapsIsCbRegistered;
static uint8_t s_otapsCccdStatus;

static uint8_t s_otapsImgType;
#if BLE_OTAPS_ENABLE_DFU
static uint32_t s_otapsDfuExecId;
#endif
static uint32_t s_otapsUpdateOffset;
static uint16_t s_otapsPktBufIdx;
static uint32_t s_otapsImgReceieved;
static uint32_t s_otapsPktTotal;
static struct {
    uint32_t mcuSize;
    uint32_t dspVoiceSize;
    uint32_t facCfgSize;
} s_otapsImgSize;


static uint8_t s_otapsAesCipher[16];


#if BLE_OTAPS_ENABLE_DFU
static uint16_t s_otapsFacCfgCrc;
static uint8_t s_otapsFacCfgIdx;
#endif

static uint16_t      s_otapsConnHandle;
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

static uint8_t ble_otaps_MbaErrToResult(uint16_t mbaErr)
{
    switch (mbaErr)
    {
        case MBA_RES_SUCCESS:
            return BLE_OTAPS_RESULT_SUCCESS;
        case MBA_RES_FAIL:
        case MBA_RES_OOM:
            return BLE_OTAPS_RESULT_OPERATION_FAILED;
        case MBA_RES_INVALID_PARA:
            return BLE_OTAPS_RESULT_INVALID_PARA;
        case MBA_RES_BAD_STATE:
            return BLE_OTAPS_RESULT_INVALID_STATE;
        default:
            return BLE_OTAPS_RESULT_OPERATION_FAILED;
    }
}

static void ble_otaps_FreeBuf()
{
    if (sp_otapsPktBuf)
    {
        OSAL_Free(sp_otapsPktBuf); 
        sp_otapsPktBuf = NULL;
    }

    if (sp_otapsPlainBuf)
    {
        OSAL_Free(sp_otapsPlainBuf);
        sp_otapsPlainBuf = NULL;
    }
}

static void ble_otaps_StopProc()
{
    s_otapsState = BLE_OTAPS_STATE_CCCD_DISABLED;
    
    s_otapsCccdStatus = 0x00;
    
    s_otapsConnHandle = 0x00;
}


static void ble_otaps_ConveyEvent(uint8_t eventId, uint8_t *p_eventField, uint8_t eventFieldLen)
{
    if (sp_otapsCbRoutine)
    {
        BLE_OTAPS_Event_T evtPara;

        evtPara.eventId = eventId;
        memcpy((uint8_t *)&evtPara.eventField, p_eventField, eventFieldLen);
        sp_otapsCbRoutine(&evtPara);
    }
}

static uint16_t ble_otaps_SendResponse(uint16_t connHandle, uint8_t reqOpcode, uint8_t result)
{
    GATTS_HandleValueParams_T hvParams;

    hvParams.charHandle = BLE_OTAS_HDL_CTRL_VAL;
    hvParams.charLength = 0x03;
    hvParams.charValue[0] = BLE_OTAPS_OP_CODE_RESPONSE;
    hvParams.charValue[1] = reqOpcode;
    hvParams.charValue[2] = result;
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;

    if (result != BLE_OTAPS_RESULT_SUCCESS)
    {
        ble_otaps_FreeBuf();
    }

   return GATTS_SendHandleValue(connHandle, &hvParams);
}

static uint16_t ble_otaps_SendDataResponse(uint8_t result)
{
    GATTS_HandleValueParams_T hvParams;

    hvParams.charHandle = BLE_OTAS_HDL_DATA_VAL;
    hvParams.charLength = 0x01;
    hvParams.charValue[0] = result;
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;

    if (result != BLE_OTAPS_RESULT_SUCCESS)
    {
        ble_otaps_FreeBuf();
    }

   return GATTS_SendHandleValue(s_otapsConnHandle, &hvParams);
}


static uint16_t ble_otaps_SendUpdateReqResponse(uint16_t connHandle, uint8_t result)
{
    GATTS_HandleValueParams_T hvParams;

    hvParams.charHandle = BLE_OTAS_HDL_CTRL_VAL;

    hvParams.charLength = 0x03 + 0x02 + BLE_OTAPS_MCU_VER_LEN;
    hvParams.charValue[0] = BLE_OTAPS_OP_CODE_RESPONSE;
    hvParams.charValue[1] = BLE_OTAPS_OP_CODE_UPDATE_REQUEST;
    hvParams.charValue[2] = result;

    if (result == BLE_OTAPS_RESULT_SUCCESS)
    {
        uint32_t ver;
    
        U16_TO_BUF_LE(&hvParams.charValue[3], BLE_OTAPS_IMG_BUF_SIZE);
        BT_SYS_ReadVersion(&ver);
        U32_TO_BUF_LE(&hvParams.charValue[5], ver);

        #if BLE_OTAPS_ENABLE_DFU
        if (DFU_ReadDspVersion(&hvParams.charValue[9]) != MBA_RES_SUCCESS)
            memset(&hvParams.charValue[9], 0x00, BLE_OTAPS_DSP_VER_SIZE);
        #endif
    }
    else
        memset(&hvParams.charValue[3], 0x00, 2 + BLE_OTAPS_MCU_VER_LEN);
    
    hvParams.sendType = ATT_HANDLE_VALUE_NTF;

   return GATTS_SendHandleValue(connHandle, &hvParams);
}

#if BLE_OTAPS_ENABLE_DFU
static uint16_t ble_otaps_AesCfbDecrypt(uint16_t length, uint8_t *p_plainText, uint8_t *p_chiperText)
{
    uint16_t processLen = 0, n = 0;
    uint8_t p_output[16];
    uint16_t res = MBA_RES_SUCCESS;

    while (processLen < length)
    {
        if (n == 0)
        {
            BLE_DM_Aes128Encrypt(s_otapsAesKey, s_otapsAesCipher, p_output);
        }

        p_plainText[processLen] = p_output[15 - n] ^ p_chiperText[processLen];
        s_otapsAesCipher[n] = p_chiperText[processLen + 15 - n - n];
        processLen++;
        n = (n + 1) % 16;
    }

    return res;
}

static uint16_t ble_otaps_AesCbcDecrypt(uint16_t length, uint8_t *p_plainText, uint8_t *p_chiperText)
{
    uint16_t processLen = 0, n = 0;
    uint8_t p_output[16];
    uint16_t res = MBA_RES_SUCCESS;

    while (processLen < length)
    {
        if (n == 0)
        {
            BLE_DM_Aes128Encrypt(s_otapsAesKey, p_chiperText, p_output);
        }

        p_plainText[processLen] = p_output[15 - n] ^ s_otapsAesCipher[processLen];
        s_otapsAesCipher[n] = p_chiperText[processLen + 15 - n - n];
        processLen++;
        n = (n + 1) % 16;
    }

    return res;
}

#endif

static uint16_t ble_otaps_UpdateImage(uint16_t len)
{
    #if BLE_OTAPS_ENABLE_DFU
    uint16_t err;
    uint8_t *p_buf;
    uint32_t cfg_offset;

    /* sp_otapsPktBuf is encrypted image, p_buf is plain text */
    err = ble_otaps_AesCfbDecrypt(len, sp_otapsPlainBuf, sp_otapsPktBuf);

    if (err == MBA_RES_SUCCESS)
    {
        p_buf = sp_otapsPlainBuf;

        switch (s_otapsImgType)
        {
            case BLE_OTAPS_IMG_TYPE_MCU:
                err = DFU_McuImageUpdate(s_otapsDfuExecId, s_otapsUpdateOffset, len, p_buf);
                break;
            case BLE_OTAPS_IMG_TYPE_DSP_VOICE:
                err = DFU_DspVoiceUpdate(s_otapsDfuExecId, s_otapsUpdateOffset, len, p_buf);
                break;
            case BLE_OTAPS_IMG_TYPE_FACTORY_CONFIG:
            {
                cfg_offset = s_otapsUpdateOffset + BLE_OTAPS_FACTORY_CFG_ADDR;
                if (s_otapsUpdateOffset == 0)
                {
                    s_otapsFacCfgIdx = *p_buf;
                    len--;
                    p_buf++;
                    cfg_offset++;
                }

                err = FLASH_Write(cfg_offset, p_buf, len);
            }
            break;
        }

        if (err == MBA_RES_SUCCESS)
        {
            /* read back to sp_otapsPktBuf, sp_otapsPktBuf is decrypted image stored in flash now */
            switch (s_otapsImgType)
            {
                case BLE_OTAPS_IMG_TYPE_MCU:
                    err = DFU_McuImageRead(s_otapsUpdateOffset, len, sp_otapsPktBuf);
                    break;
                case BLE_OTAPS_IMG_TYPE_DSP_VOICE:
                    err = DFU_DspVoiceRead(s_otapsUpdateOffset, len, sp_otapsPktBuf);
                    break;
                case BLE_OTAPS_IMG_TYPE_FACTORY_CONFIG:
                    err = FLASH_Read(sp_otapsPktBuf, cfg_offset, len);
                    break;
            }

            if (err == MBA_RES_SUCCESS)
            {
                uint16_t offset;

                for (offset = 0; offset < len; )
                {
                    uint8_t cmpLen;

                    cmpLen = (len - offset < 0xFF) ? (len - offset) : 0xFF;

                    if (memcmp(p_buf + offset, sp_otapsPktBuf + offset, cmpLen) != 0)
                    {
                        err = MBA_RES_FAIL;
                        break;
                    }

                    offset += cmpLen;
                }
            }
        }
    } 

    return err;
    #else
    return MBA_RES_SUCCESS;
    #endif
}

static void ble_otaps_ProcUpdateRequest(GATT_Event_T *p_event)
{
    if (p_event->eventField.onWrite.writeDataLength - 1 != BLE_OTAPS_UPDATE_REQ_SIZE)
    {
        ble_otaps_SendUpdateReqResponse(p_event->eventField.onWrite.connHandle, BLE_OTAPS_RESULT_NOT_SUPPORTED);
    }
    else
    {
        uint16_t err;
        uint8_t result;

        #if BLE_OTAPS_ENABLE_DFU
        err = DFU_Start((DFU_Info_T *)p_event->eventField.onWrite.writeValue + 1, &s_otapsDfuExecId);
         #else
        err = MBA_RES_SUCCESS;
        #endif
        result = ble_otaps_MbaErrToResult(err);

        if (result == BLE_OTAPS_RESULT_SUCCESS)
        {
            BLE_OTAPS_EvtUpdateReq_T evtUpReq;
            uint8_t * p_value = p_event->eventField.onWrite.writeValue + 1;
            evtUpReq.connHandle = p_event->eventField.onWrite.connHandle;

            STREAM_LE_TO_U32(&evtUpReq.mcuImageSize, &p_value);
            STREAM_LE_TO_U32(&evtUpReq.verMcu, &p_value);
            s_otapsImgSize.mcuSize = evtUpReq.mcuImageSize;
            ble_otaps_ConveyEvent(BLE_OTAPS_EVT_UPDATE_REQ, (uint8_t *)&evtUpReq, sizeof(evtUpReq));
        }
        else
        {
            err = ble_otaps_SendUpdateReqResponse(p_event->eventField.onWrite.connHandle, result);
        }
    }
}

static void ble_otaps_ProcUpdateStart(GATT_Event_T *p_event)
{
    uint16_t err;
    uint8_t result = BLE_OTAPS_RESULT_NOT_SUPPORTED;

    if (p_event->eventField.onWrite.writeDataLength - 1 != BLE_OTAPS_UPDATE_START_SIZE)
    {
        ble_otaps_SendResponse(p_event->eventField.onWrite.connHandle, BLE_OTAPS_OP_CODE_UPDATE_START, BLE_OTAPS_RESULT_NOT_SUPPORTED);
        return;
    }

    s_otapsImgType = p_event->eventField.onWrite.writeValue[1];

    switch (s_otapsImgType)
    {
        case BLE_OTAPS_IMG_TYPE_MCU:
        {
            #if BLE_OTAPS_ENABLE_DFU
            err = DFU_McuImageInit(crc);
            #else
            err = MBA_RES_SUCCESS;
            #endif
            s_otapsPktTotal = s_otapsImgSize.mcuSize;
            result = ble_otaps_MbaErrToResult(err);
        }
        break;

        default:
            break;
    }

    if (result == BLE_OTAPS_RESULT_SUCCESS)
    {
        sp_otapsPktBuf = OSAL_Malloc(BLE_OTAPS_IMG_BUF_SIZE);
        sp_otapsPlainBuf = OSAL_Malloc(BLE_OTAPS_IMG_BUF_SIZE);
        if (sp_otapsPktBuf == NULL || sp_otapsPlainBuf == NULL) 
        {
            result = BLE_OTAPS_RESULT_OPERATION_FAILED;
        }
        else
        {
            BLE_OTAPS_EvtUpdateStart_T evtInit;

            evtInit.imageType = s_otapsImgType;

            s_otapsState = BLE_OTAPS_STATE_UPDATE_START;
            s_otapsUpdateOffset = 0;
            s_otapsPktBufIdx = 0;
            s_otapsImgReceieved = 0;
            memcpy(s_otapsAesCipher, s_otapsAesIv, 16);

            ble_otaps_ConveyEvent(BLE_OTAPS_EVT_UPDATE_START, (uint8_t *)&evtInit , sizeof(evtInit));
        }
    }

    err = ble_otaps_SendResponse(p_event->eventField.onWrite.connHandle, BLE_OTAPS_OP_CODE_UPDATE_START, result);
}


static void ble_otaps_OnCtrlPtWrite(GATT_Event_T *p_event)
{
    switch (p_event->eventField.onWrite.writeValue[0])
    {
        case BLE_OTAPS_OP_CODE_UPDATE_REQUEST:
            ble_otaps_ProcUpdateRequest(p_event);
            break;
        case BLE_OTAPS_OP_CODE_UPDATE_START:
            ble_otaps_ProcUpdateStart(p_event);
            break;
        case BLE_OTAPS_OP_CODE_UPDATE_COMPLETE:
        {
            uint16_t err;
            uint8_t result;
            BLE_OTAPS_EvtComplete_T evtComplete;

            if (s_otapsState == BLE_OTAPS_STATE_COMPLETE)
            {
                ble_otaps_SendResponse(p_event->eventField.onWrite.connHandle, BLE_OTAPS_OP_CODE_UPDATE_COMPLETE, BLE_OTAPS_RESULT_INVALID_STATE);
                return;
            }

            if (s_otapsImgReceieved >= s_otapsPktTotal)
            {
                #if BLE_OTAPS_ENABLE_DFU
                if (s_otapsImgSize.mcuSize || s_otapsImgSize.dspVoiceSize)
                    err = DFU_End(s_otapsDfuExecId);
                else
                    err = MBA_RES_SUCCESS;

                if (err == MBA_RES_SUCCESS && s_otapsImgSize.facCfgSize)
                    err = FLASH_Write(BLE_OTAPS_FACTORY_CFG_ADDR, &s_otapsFacCfgIdx, BLE_OTAPS_FACTORY_CFG_IDX_SIZE);
                #else
                err = MBA_RES_SUCCESS;
                #endif

                evtComplete.operation = BLE_OTAPS_COMP_OP_CONFIRM;
            }
            else
            {
                ble_otaps_FreeBuf();
            
                err = MBA_RES_SUCCESS;

                evtComplete.operation = BLE_OTAPS_COMP_OP_STOP;
                
                result = ble_otaps_MbaErrToResult(err);
                err = ble_otaps_SendResponse(p_event->eventField.onWrite.connHandle, BLE_OTAPS_OP_CODE_UPDATE_COMPLETE, result);
            }

            ble_otaps_ConveyEvent(BLE_OTAPS_EVT_COMPLETE, (uint8_t *)&evtComplete , sizeof(evtComplete));
        }
        break;
        case BLE_OTAPS_OP_CODE_RESET_REQUEST:
            ble_otaps_ConveyEvent(BLE_OTAPS_EVT_RESET_REQ, NULL, 0);
            break;
        default:
            break;
    }
}

static void ble_otaps_PacketHandler(GATT_Event_T *p_event)
{
    uint16_t err;
    uint8_t result;
    uint16_t copyLen, remainLen;
    uint8_t *p_copyData;
    BLE_OTAPS_EvtUpdate_T evtUpdate;

    if (s_otapsState != BLE_OTAPS_STATE_UPDATE_START)
    {
        ble_otaps_SendDataResponse(BLE_OTAPS_RESULT_INVALID_STATE);
        return;
    }

    p_copyData = p_event->eventField.onWrite.writeValue;
    remainLen = p_event->eventField.onWrite.writeDataLength;
    evtUpdate.length = 0;

    while (remainLen + s_otapsPktBufIdx >= BLE_OTAPS_IMG_BUF_SIZE)
    {
        copyLen = BLE_OTAPS_IMG_BUF_SIZE - s_otapsPktBufIdx;

        memcpy(sp_otapsPktBuf + s_otapsPktBufIdx, p_copyData, copyLen);
        s_otapsPktBufIdx = 0;
        p_copyData += copyLen;

        err = ble_otaps_UpdateImage(BLE_OTAPS_IMG_BUF_SIZE);

        result = ble_otaps_MbaErrToResult(err);

        if (result != BLE_OTAPS_RESULT_SUCCESS)
        {
            err = ble_otaps_SendDataResponse(result);
            return;
        }

        remainLen -= copyLen;
        s_otapsUpdateOffset += BLE_OTAPS_IMG_BUF_SIZE;
        evtUpdate.length += BLE_OTAPS_IMG_BUF_SIZE;
    }

    if (remainLen)
    {
        memcpy(sp_otapsPktBuf + s_otapsPktBufIdx, p_copyData, remainLen);
        s_otapsPktBufIdx += remainLen;
    }

    if (p_event->eventField.onWrite.writeDataLength + s_otapsImgReceieved >= s_otapsPktTotal && s_otapsPktBufIdx)
    {
        err = ble_otaps_UpdateImage(s_otapsPktBufIdx);

        result = ble_otaps_MbaErrToResult(err);

        if (result != BLE_OTAPS_RESULT_SUCCESS)
        {
            err = ble_otaps_SendDataResponse(result);
            return;
        }

        evtUpdate.length += s_otapsPktBufIdx;
        s_otapsUpdateOffset += s_otapsPktBufIdx;
    }

    if (evtUpdate.length)
    {
        ble_otaps_ConveyEvent(BLE_OTAPS_EVT_UPDATE, (uint8_t *)&evtUpdate , sizeof(evtUpdate));
        err = ble_otaps_SendDataResponse(result);
    }

    s_otapsImgReceieved += p_event->eventField.onWrite.writeDataLength;
}

static void ble_otaps_Disconnect()
{
    if (s_otapsAppAllowed)
    {
        if (s_otapsState > BLE_OTAPS_STATE_CCCD_DISABLED)
        {
            ble_otaps_StopProc();
            
            ble_otaps_FreeBuf();
        }
    }
}


static void ble_otaps_GapEventProcess(BLE_GAP_Event_T *p_event)
{
    switch(p_event->eventId)
    {
        case BLE_GAP_EVT_DISCONNECTED:
        {
            if (s_otapsConnHandle == p_event->eventField.evtDisconnect.connHandle)
                ble_otaps_Disconnect();
        }
        break;
    }
}

static void ble_otaps_GattEventHandler(GATT_Event_T *p_event)
{
    if (!s_otapsIsCbRegistered)
        return;

    switch (p_event->eventId)
    {
        case GATTS_EVT_WRITE:
        {
            uint8_t errCode = 0;

            if (p_event->eventField.onWrite.attrHandle == BLE_OTAS_HDL_DATA_VAL)
            {
                /* No need to send response. */
                if (p_event->eventField.onWrite.connHandle == s_otapsConnHandle)
                    ble_otaps_PacketHandler(p_event);
                return;
            }

            if ((p_event->eventField.onWrite.attrHandle != BLE_OTAS_HDL_CTRL_CCCD) &&
                (p_event->eventField.onWrite.attrHandle != BLE_OTAS_HDL_CTRL_VAL) &&
                (p_event->eventField.onWrite.attrHandle != BLE_OTAS_HDL_DATA_CCCD))
            {
                /* Only process write operations on SETTING_MANUAL_WRITE_RSP in OTA. */
                return;
            }

            if (p_event->eventField.onWrite.writeType == ATT_PREPARE_WRITE_REQ)
            {
                errCode = ATT_ERRCODE_APPLICATION_ERROR;
            }

            if (!errCode)
            {
                if (p_event->eventField.onWrite.attrHandle == BLE_OTAS_HDL_CTRL_CCCD ||
                    p_event->eventField.onWrite.attrHandle == BLE_OTAS_HDL_DATA_CCCD)
                {
                    if (s_otapsAppAllowed == false)
                    {
                        errCode = ATT_ERRCODE_WRITE_NOT_PERMITTED;
                    }
                    else
                    {
                        uint16_t cccd;

                        BUF_LE_TO_U16(&cccd, p_event->eventField.onWrite.writeValue);

                        if (cccd == NOTIFICATION)
                        {
                            if (p_event->eventField.onWrite.attrHandle == BLE_OTAS_HDL_CTRL_CCCD)
                            {
                                s_otapsCccdStatus |= (BLE_OTAPS_CCCD_CTRL_ON);
                            
                                if (s_otapsCccdStatus & BLE_OTAPS_CCCD_DATA_ON)
                                    s_otapsState = BLE_OTAPS_STATE_CCCD_ENABLED;
                            }
                            else
                            {
                                s_otapsCccdStatus |= (BLE_OTAPS_CCCD_DATA_ON);
                            
                                if (s_otapsCccdStatus & BLE_OTAPS_CCCD_CTRL_ON)
                                    s_otapsState = BLE_OTAPS_STATE_CCCD_ENABLED;
                            }

                            errCode = 0x00;
                        }
                        else if (cccd == 0x0000)
                        {
                            if (s_otapsConnHandle == 0 || p_event->eventField.onWrite.connHandle == s_otapsConnHandle)
                            {
                                if (p_event->eventField.onWrite.attrHandle == BLE_OTAS_HDL_CTRL_CCCD)
                                    s_otapsCccdStatus &= ~(BLE_OTAPS_CCCD_CTRL_ON);
                                else
                                    s_otapsCccdStatus &= ~(BLE_OTAPS_CCCD_DATA_ON);
                            
                                s_otapsState = BLE_OTAPS_STATE_CCCD_DISABLED;
                                errCode = 0x00;
                            }
                            else
                                errCode = ATT_ERRCODE_WRITE_NOT_PERMITTED;
                        }
                        else
                        {
                            errCode = ATT_ERRCODE_APPLICATION_ERROR;
                        }
                    }
                }
                else if (p_event->eventField.onWrite.attrHandle == BLE_OTAS_HDL_CTRL_VAL)
                {
                    if (s_otapsState == BLE_OTAPS_STATE_CCCD_DISABLED
                        || (s_otapsConnHandle != 0 && p_event->eventField.onWrite.connHandle != s_otapsConnHandle)
                        || (s_otapsConnHandle == 0 && p_event->eventField.onWrite.writeValue[0] != BLE_OTAPS_OP_CODE_UPDATE_REQUEST))
                        errCode = ATT_ERRCODE_WRITE_NOT_PERMITTED;
                }
            }

            if (!errCode)
            {
                GATTS_SendWriteRespParams_T *p_respParams;

                p_respParams = (GATTS_SendWriteRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendWriteRespParams_T));
                if (p_respParams != NULL)
                {
                    p_respParams->responseType = ATT_WRITE_RSP;
                    GATTS_SendWriteResponse(p_event->eventField.onWrite.connHandle, p_respParams);
                    OSAL_Free(p_respParams);
                }

                /* It will take long time to validate so we must send write response before calling BLE_OTAPS_OnCtrlPtWrite */
                if (p_event->eventField.onWrite.attrHandle == BLE_OTAS_HDL_CTRL_VAL)
                {
                    ble_otaps_OnCtrlPtWrite(p_event);
                }
            }
            else
            {
                GATTS_SendErrRespParams_T *p_respParams;

                p_respParams = (GATTS_SendErrRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendErrRespParams_T));
                if (p_respParams != NULL)
                {
                    p_respParams->reqOpcode = p_event->eventField.onWrite.writeType;
                    p_respParams->attrHandle = p_event->eventField.onWrite.attrHandle;
                    p_respParams->errorCode = errCode;
                    GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, p_respParams);
                    OSAL_Free(p_respParams);
                }
            }
        }
        break;

        case GATTS_EVT_READ:
        {
            if (p_event->eventField.onRead.attrHandle == BLE_OTAS_HDL_FEATURE_VAL)
            {
                uint8_t errCode = 0;
            
                if (s_otapsState == BLE_OTAPS_STATE_CCCD_DISABLED)
                {
                    errCode = ATT_ERRCODE_WRITE_NOT_PERMITTED;
                }
                else if (p_event->eventField.onRead.readType != ATT_READ_REQ)
                {
                    errCode = ATT_ERRCODE_REQUEST_NOT_SUPPORT;
                }

                if (!errCode)
                {
                    GATTS_SendReadRespParams_T *p_respParams;

                    p_respParams = (GATTS_SendReadRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendReadRespParams_T));
                    if (p_respParams != NULL)
                    {
                        p_respParams->attrLength = 0x01;
                        p_respParams->responseType = ATT_READ_RSP;
                        p_respParams->attrValue[0] = BLE_OTAPS_FEATURE_SUP_IMG_TYPE;
                        GATTS_SendReadResponse(p_event->eventField.onRead.connHandle, p_respParams);
                        OSAL_Free(p_respParams);
                    }
                }
                else
                {
                    GATTS_SendErrRespParams_T *p_respParams;

                    p_respParams = (GATTS_SendErrRespParams_T *)OSAL_Malloc(sizeof(GATTS_SendErrRespParams_T));
                    if (p_respParams != NULL)
                    {
                        p_respParams->reqOpcode = p_event->eventField.onRead.readType;
                        p_respParams->attrHandle = p_event->eventField.onRead.attrHandle;
                        p_respParams->errorCode = errCode;
                        GATTS_SendErrorResponse(p_event->eventField.onWrite.connHandle, p_respParams);
                        OSAL_Free(p_respParams);
                    }
                }
            }
        }
        break;

        default:
            break;
    }
}

uint16_t BLE_OTAPS_Init()
{
    s_otapsIsCbRegistered = 0;

    sp_otapsPktBuf = NULL;
    sp_otapsPlainBuf = NULL;

    s_otapsState = BLE_OTAPS_STATE_CCCD_DISABLED;

    s_otapsAppAllowed = 0x00;

    s_otapsConnHandle = 0x00;

    s_otapsCccdStatus = 0x00;

    return BLE_OTAS_Add();
}

void BLE_OTAPS_EventRegistration(BLE_OTAPS_ProcessRoutine bleOtapsRoutine)
{
    s_otapsIsCbRegistered = 1;

    sp_otapsCbRoutine = bleOtapsRoutine;
}


uint16_t BLE_OTAPS_Enable()
{
    s_otapsAppAllowed = 0x01;

    return MBA_RES_SUCCESS;
}

uint16_t BLE_OTAPS_Disable()
{
    ble_otaps_StopProc();

    ble_otaps_FreeBuf();

    s_otapsAppAllowed = 0x00;

    return MBA_RES_SUCCESS;
}

uint16_t BLE_OTAPS_SetEncrytionInfo(uint8_t * p_iv, uint8_t * p_key)
{
    if (s_otapsState > BLE_OTAPS_STATE_CCCD_ENABLED)
        return MBA_RES_BAD_STATE;

    memcpy(s_otapsAesIv, p_iv, 16);
    memcpy(s_otapsAesKey, p_key, 16);

    return MBA_RES_SUCCESS;
}

uint16_t BLE_OTAPS_UpdateReqResponse(uint16_t connHandle, bool isAllow)
{
    if (isAllow)
    {
        s_otapsState = BLE_OTAPS_STATE_UPDATE_REQ;

        s_otapsConnHandle = connHandle;
    }

    return ble_otaps_SendUpdateReqResponse(connHandle, (isAllow) ? BLE_OTAPS_RESULT_SUCCESS : BLE_OTAPS_RESULT_NOT_SUPPORTED);
}

uint16_t BLE_OTAPS_ConfirmResponse(bool success)
{
    s_otapsState = BLE_OTAPS_STATE_COMPLETE;

    return ble_otaps_SendResponse(s_otapsConnHandle, BLE_OTAPS_OP_CODE_UPDATE_COMPLETE, (success) ? BLE_OTAPS_RESULT_SUCCESS : BLE_OTAPS_RESULT_OPERATION_FAILED);
}


void BLE_OTAPS_BleEventHandler(STACK_Event_T *p_stackEvent)
{
    switch (p_stackEvent->groupId)
    {
        case STACK_GRP_BLE_GAP:
        {
            ble_otaps_GapEventProcess((BLE_GAP_Event_T *)p_stackEvent->p_event);
        }
        break;

        case STACK_GRP_GATT:
        {
            ble_otaps_GattEventHandler((GATT_Event_T *)p_stackEvent->p_event);
        }
        break;

        default:
        break;
    }
}


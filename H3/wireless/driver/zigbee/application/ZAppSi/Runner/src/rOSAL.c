#include <zgb_api.h>
#include <cs_api.h>
#include <nwk_api.h>
#include <configserver.h>
#include <apsCommon.h>
#include <zclAttributes.h>
#include <clusters.h>
#include <pdsDataServer.h>
#include <console.h>
#include <errH.h>
#include <rTypes.h>
#include <rParser.h>
#include "app_task.h"

#include <bdbZllBindings.h>

extern OSAL_QUEUE_HANDLE_TYPE zigbeeRequestQueueHandle;

/**************************************************************************
\brief Create Application queue for zigbee and usart events
***************************************************************************/
void create_app_queue(void)
{
  g_appQueue = xQueueCreate( 64, sizeof(APP_Msg_T) );
}

/**************************************************************************
\brief Zigbee API CALL
***************************************************************************/
inline void ZIGBEE_API_CALL(STACK_API_Request *request)
{
  if ( OSAL_RESULT_TRUE != OSAL_QUEUE_Send(&zigbeeRequestQueueHandle, &request,10))
  {
    while(1);   // ERROR , Should not hang here, handle with assert
  }
}
/**************************************************************************
\brief Post Zigbee API CALLs into queue
***************************************************************************/
#if defined(_RUNNER_OSAL_)
void APP_EvtUpload(void)
{
    APP_Msg_T   appMsg;
    APP_Msg_T   *p_appMsg;

    appMsg.msgId=APP_MSG_ZB_STACK_EVT;

    p_appMsg = &appMsg;
    OSAL_QUEUE_Send(&g_appQueue, p_appMsg, 0);
}

/**************************************************************************//**
\brief ZDO_ResetNetwork Confirmation function

\param[out] conf - Confirmation structure with status
******************************************************************************/
void ZDO_ResetNetworkConf(ZDO_ResetNetworkConf_t *conf)
{
  sysAssert(ZDO_SUCCESS_STATUS == conf->status, ZDOINIT_ZDOINITRESETNETWORKCONF_0);
  APP_EvtUpload();
}
#endif
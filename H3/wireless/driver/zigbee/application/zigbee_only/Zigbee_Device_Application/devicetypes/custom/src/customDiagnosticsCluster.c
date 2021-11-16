
/******************************************************************************
                    Includes section
******************************************************************************/
#include <zclDiagnosticsCluster.h>
#include <haClusters.h>
#include <console.h>
#include <commandManager.h>
#include <wlPdsMemIds.h>
#include <z3Device.h>


/******************************************************************************
                    Definition(s) section
******************************************************************************/


/******************************************************************************
                    Prototypes section
******************************************************************************/
static void customDiagnosticsEventListener(SYS_EventId_t eventId, SYS_EventData_t data);

/******************************************************************************
                    Global variables section
******************************************************************************/
ZCL_DiagnosticsClusterServerAttributes_t DiagnosticsClusterServerAttributes =
{
  ZCL_DEFINE_DIAGNOSTICS_CLUSTER_SERVER_ATTRIBUTES()
};

/******************************************************************************
                    Static variables section
******************************************************************************/
static SYS_EventReceiver_t customEventReceiver = { .func = customDiagnosticsEventListener};
/******************************************************************************
                    Implementation section
******************************************************************************/
/**************************************************************************//**
\brief Initializes diagnostics cluster
******************************************************************************/
void diagnosticsClusterInit(void)
{
  DiagnosticsClusterServerAttributes.averageMACRetryPerAPSMessageSent.value = 0;
  DiagnosticsClusterServerAttributes.lastMessageLQI.value = 0;
  DiagnosticsClusterServerAttributes.lastMessageRSSI.value = 0;
  SYS_SubscribeToEvent(BC_EVENT_APS_DATA_INDICATION, &customEventReceiver);
  SYS_SubscribeToEvent(BC_EVENT_NWK_DATA_CONFIRM, &customEventReceiver);
}

static void customDiagnosticsEventListener(SYS_EventId_t eventId, SYS_EventData_t data)
{
  switch(eventId)
  {
    case BC_EVENT_APS_DATA_INDICATION:
      if(NULL != (APS_DataQualityIndicator_t *)data)
      {
        APS_DataQualityIndicator_t *apsDataQualityIndicator = (APS_DataQualityIndicator_t *)data;
        DiagnosticsClusterServerAttributes.lastMessageLQI.value = apsDataQualityIndicator->linkQuality;
        DiagnosticsClusterServerAttributes.lastMessageRSSI.value = apsDataQualityIndicator->rssi;
      }
      break;

    case BC_EVENT_NWK_DATA_CONFIRM:
      if(NULL != (uint16_t*)data)
      {
        static bool firstPacket = 1;
        if(firstPacket)
        {
          DiagnosticsClusterServerAttributes.averageMACRetryPerAPSMessageSent.value
            = *(uint16_t*)data;
          firstPacket = 0;
        }
        else
        {
           
          DiagnosticsClusterServerAttributes.averageMACRetryPerAPSMessageSent.value
            = (DiagnosticsClusterServerAttributes.averageMACRetryPerAPSMessageSent.value 
              + *(uint16_t*)data) / 2;           
        }
      }
      break;
    default:
      break;
  }
}

// eof customDiagnosticsCluster.c


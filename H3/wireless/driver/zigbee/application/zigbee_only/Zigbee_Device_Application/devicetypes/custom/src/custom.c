#if (APP_Z3_DEVICE_TYPE == APP_DEVICE_TYPE_CUSTOM_DEVICE)

/******************************************************************************
                             Includes section
******************************************************************************/
#include <haClusters.h>
#include <custom/include/customClusters.h>
#include <identifyCluster.h>
#include <custom/include/customBasicCluster.h>
#include <custom/include/customIdentifyCluster.h>
#include <custom/include/customOnOffCluster.h>
#include <custom/include/customLevelControlCluster.h>
#include <custom/include/customColorControlCluster.h>
#include <custom/include/customGroupsCluster.h>
#include <custom/include/customScenesCluster.h>
#include <custom/include/customCommissioningCluster.h>
#include <z3Device.h>
#include <zclSecurityManager.h>
#include <console.h>
#include <wlPdsMemIds.h>
#include <otauService.h>
#include <zclCommandManager.h>
#include <custom/include/custom.h>
#include <scenesCluster.h>
#include <zigbee_device/clusters/include/commissioningCluster.h>
#include <zclOtauManager.h>

/******************************************************************************
                   type(s) section
******************************************************************************/

/*******************************************************************************
                    Static functions section
*******************************************************************************/
static void customFindingBindingFinishedForACluster(Endpoint_t ResponentEp, ClusterId_t id);
#if (ZB_COMMISSIONING_ON_STARTUP == 1)
#ifdef _ZCL_REPORTING_SUPPORT_
static void customConfigureReportingResp(ZCL_Notify_t *ntfy);
#endif
#endif

#ifdef OTAU_CLIENT 
static void customAddOTAUClientCluster(void);
static void configureImageKeyDone(void);
#endif
/******************************************************************************
                    Local variables section
******************************************************************************/
static ZCL_DeviceEndpoint_t customEndpoint =
{
  .simpleDescriptor =
  {
    .endpoint            = APP_ENDPOINT_CUSTOM,
    .AppProfileId        = PROFILE_ID_HOME_AUTOMATION,
    .AppDeviceId         = APP_Z3DEVICE_ID,
    .AppDeviceVersion    = 0x1,
    .AppInClustersCount  = ARRAY_SIZE(customServerClusterIds),
    .AppInClustersList   = customServerClusterIds,
    .AppOutClustersCount = ARRAY_SIZE(customClientClusterIds),
    .AppOutClustersList  = customClientClusterIds,
  },
  .serverCluster = customServerClusters,
  .clientCluster = customClientClusters,
};

static ClusterId_t customClientClusterToBindIds[] =
{
  IDENTIFY_CLUSTER_ID,
  GROUPS_CLUSTER_ID,
};

static ClusterId_t customServerClusterToBindIds[] =
{
  BASIC_CLUSTER_ID,
  IDENTIFY_CLUSTER_ID,
  GROUPS_CLUSTER_ID,
  SCENES_CLUSTER_ID,
  ONOFF_CLUSTER_ID,
  LEVEL_CONTROL_CLUSTER_ID,
  COLOR_CONTROL_CLUSTER_ID
};

static AppBindReq_t customBindReq =
{
  .srcEndpoint       = APP_ENDPOINT_CUSTOM,
  .remoteServersCnt  = ARRAY_SIZE(customClientClusterToBindIds),
  .remoteClientsCnt  = ARRAY_SIZE(customServerClusterToBindIds),
  .groupId           = 0xffff,
  .remoteServers     = customClientClusterToBindIds,
  .remoteClients     = customServerClusterToBindIds,
  .callback          = customFindingBindingFinishedForACluster,
  .startIdentifyingFn= customIdetifyStartIdentifyingCb
};

/******************************************************************************
                    Implementation section
******************************************************************************/
/**************************************************************************//**
\brief Device initialization routine
******************************************************************************/
void appDeviceInit(void)
{

#if APP_ENABLE_CONSOLE == 1
  initConsole();
#endif

  LCD_INIT();


  LCD_PRINT(0, 1, "Custom Device");

  if (!APP_RegisterEndpoint(&customEndpoint, &customBindReq))
    return;

  ZCL_CommandManagerInit();

#ifdef OTAU_CLIENT
  customAddOTAUClientCluster();
#endif //OTAU_CLIENT

#ifdef _ENABLE_PERSISTENT_SERVER_
  if (PDS_IsAbleToRestore(APP_CUSTOM_SCENES_MEM_ID))
    PDS_Restore(APP_CUSTOM_SCENES_MEM_ID);
#endif
  /**CHANGE* - cluster version need to be reinitilized here after PDS Restore same for all devices
    implement a common function to reinitilized */
}

/**************************************************************************//**
\brief Callback about confguring image key on EEPROM
******************************************************************************/
#ifdef OTAU_CLIENT
static void configureImageKeyDone(void)
{
  return;
}
#endif

/**************************************************************************//**
\brief Device common task handler
******************************************************************************/
void appDeviceTaskHandler(void)
{
  switch (appDeviceState) // Actual device state when one joined network
  {
    case DEVICE_INITIAL_STATE:
      {
        appDeviceState = DEVICE_ACTIVE_IDLE_STATE;
      }
#ifdef OTAU_CLIENT
      startOtauClient();
      ZCL_ConfigureOtauImageKey(configureImageKeyDone);
#endif
      break;
    case DEVICE_ACTIVE_IDLE_STATE:
    default:
      break;
  }
}

/*******************************************************************************
\brief callback called on the finishing of binding of one cluster
********************************************************************************/
static void customFindingBindingFinishedForACluster(Endpoint_t ResponentEp, ClusterId_t clusterId)
{
  ZCL_Cluster_t *serverCluster;
  switch(clusterId)
  {
    case ONOFF_CLUSTER_ID:
      serverCluster = ZCL_GetCluster(APP_ENDPOINT_CUSTOM, ONOFF_CLUSTER_ID, ZCL_CLUSTER_SIDE_SERVER);
      if (serverCluster)
#ifdef _ZCL_REPORTING_SUPPORT_
#if (ZB_COMMISSIONING_ON_STARTUP == 1)
        sendConfigureReportingToNotify(APP_ENDPOINT_CUSTOM, 0, ONOFF_CLUSTER_ID,
                                     ZCL_ONOFF_CLUSTER_ONOFF_SERVER_ATTRIBUTE_ID, ONOFF_VAL_MAX_REPORT_PERIOD,
                                     customConfigureReportingResp);
#else
        ZCL_StartReporting();
#endif
#endif
      break;
    case LEVEL_CONTROL_CLUSTER_ID:
       serverCluster = ZCL_GetCluster(APP_ENDPOINT_CUSTOM, LEVEL_CONTROL_CLUSTER_ID, ZCL_CLUSTER_SIDE_SERVER);
       if (serverCluster)
#if (ZB_COMMISSIONING_ON_STARTUP == 1)
#ifdef _ZCL_REPORTING_SUPPORT_
         sendConfigureReportingToNotify(APP_ENDPOINT_CUSTOM, 0,
                                      LEVEL_CONTROL_CLUSTER_ID,ZCL_LEVEL_CONTROL_CLUSTER_CURRENT_LEVEL_ATTRIBUTE_ID,
                                      LEVEL_CONTROL_VAL_MAX_REPORT_PERIOD, customConfigureReportingResp);
#endif
#else
       ZCL_StartReporting();
#endif

      break;
  }

}

#if (ZB_COMMISSIONING_ON_STARTUP == 1)
#ifdef _ZCL_REPORTING_SUPPORT_
/**************************************************************************//**
\brief Indication of configure reporting response

\param[in] ntfy - pointer to response
******************************************************************************/
static void customConfigureReportingResp(ZCL_Notify_t *ntfy)
{
  ZCL_StartReporting();
  (void)ntfy;
}
#endif
#endif
/**************************************************************************//**
\brief Stops identifying on endpoints
******************************************************************************/
void appIdentifyStart(uint16_t identifyTime, bool colorEffect, uint16_t enhancedHue)
{
  customIdentifyStart(identifyTime, colorEffect, enhancedHue);
}

/**************************************************************************//**
\brief Stops identifying on endpoints
******************************************************************************/
void appIdentifyStop(void)
{
  customIdentifyStop();
}

#ifdef OTAU_CLIENT 
/**************************************************************************//**
\brief Adds OTAU client cluster to list of clients clusters of custom devices
******************************************************************************/
static void customAddOTAUClientCluster(void)
{
  customClientClusters[CUSTOM_CLIENT_CLUSTERS_COUNT - 1U] = ZCL_GetOtauClientCluster();
}
#endif // OTAU_CLIENT

#endif // APP_DEVICE_TYPE_CUSTOM_DEVICE
// eof custom.c
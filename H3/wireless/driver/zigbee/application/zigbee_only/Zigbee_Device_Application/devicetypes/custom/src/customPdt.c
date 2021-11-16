#if (APP_Z3_DEVICE_TYPE == APP_DEVICE_TYPE_CUSTOM_DEVICE)

/******************************************************************************
                    Includes section
******************************************************************************/
#include <haClusters.h>
#include <wlPdsMemIds.h>
#include <scenes.h>
#include <light/include/lightOnOffCluster.h>
#include <light/include/lightLevelControlCluster.h>
#include <light/include/lightColorControlCluster.h>
#include <light/include/lightScenesCluster.h>
/******************************************************************************
                    PDT definitions
******************************************************************************/
#ifdef _ENABLE_PERSISTENT_SERVER_
/* Dimmable Light application data file descriptors.
   Shall be placed in the PDS_FF code segment. */
PDS_DECLARE_FILE(APP_LIGHT_SCENES_MEM_ID,        MAX_SCENES_AMOUNT * sizeof(Scene_t),                        lightSceneTable,                                           NO_FILE_MARKS);
PDS_DECLARE_FILE(APP_LIGHT_ONOFF_MEM_ID,         sizeof(lightOnOffClusterServerAttributes),               &lightOnOffClusterServerAttributes,               NO_FILE_MARKS);
#if (APP_Z3_DEVICE_TYPE >= APP_DEVICE_TYPE_DIMMABLE_LIGHT)
PDS_DECLARE_FILE(APP_LIGHT_LEVEL_CONTROL_MEM_ID, sizeof(lightLevelControlClusterServerAttributes), &lightLevelControlClusterServerAttributes, NO_FILE_MARKS);
#endif
#if (APP_Z3_DEVICE_TYPE >= APP_DEVICE_TYPE_COLOR_LIGHT)
PDS_DECLARE_FILE(APP_LIGHT_COLOR_CONTROL_MEM_ID, sizeof(lightColorControlClusterServerAttributes), &lightColorControlClusterServerAttributes, NO_FILE_MARKS);
#endif


/* Dimmable Light application data file identifiers list.
   Will be placed in flash. */
PROGMEM_DECLARE(PDS_MemId_t appDlMemoryIdsTable[]) =
{
  APP_LIGHT_SCENES_MEM_ID,
  APP_LIGHT_ONOFF_MEM_ID

#if (APP_Z3_DEVICE_TYPE >= APP_DEVICE_TYPE_DIMMABLE_LIGHT)
  ,APP_LIGHT_LEVEL_CONTROL_MEM_ID
#endif

#if (APP_Z3_DEVICE_TYPE >= APP_DEVICE_TYPE_COLOR_LIGHT)
 ,APP_LIGHT_COLOR_CONTROL_MEM_ID
#endif
};

/* Dimmable Light application directory descriptor.
   Shall be placed in the PDS_FD code segment. */
PDS_DECLARE_DIR(PDS_DirDescr_t appEsiMemoryDirDescr) =
{
  .list       = appDlMemoryIdsTable,
  .filesCount = ARRAY_SIZE(appDlMemoryIdsTable),
  .memoryId   = Z3DEVICE_APP_MEMORY_MEM_ID
};

#endif // _ENABLE_PERSISTENT_SERVER_
#endif // APP_DEVICE_TYPE_DIMMABLE_LIGHT

// eof lightPdt.c

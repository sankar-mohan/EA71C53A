#ifndef _CUSTOMSCENESCLUSTER_H
#define _CUSTOMSCENESCLUSTER_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zclZllScenesCluster.h>
#include <zcl/clusters/include/scenes.h>

/******************************************************************************
                    Externals
******************************************************************************/
extern ZCL_SceneClusterServerAttributes_t customScenesClusterServerAttributes;
extern ZCL_SceneClusterClientAttributes_t customScenesClusterClientAttributes;
extern PROGMEM_DECLARE(ZCL_ScenesClusterCommands_t        customScenesCommands);
extern PROGMEM_DECLARE(ZCL_ScenesClusterCommands_t        customScenesClientCommands);


extern Scene_t customSceneTable[];

/******************************************************************************
                    Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Initializes Scenes cluster
******************************************************************************/
void customScenesClusterInit(void);

/**************************************************************************//**
\brief Invalidates scene
******************************************************************************/
void customScenesInvalidate(void);

///**************************************************************************//**
//\brief Recalls scene from scene table
//
//\param[in] recallScene - the pointer to Recall Scene request
//******************************************************************************/
//void recallScene(ZCL_RecallScene_t *recallScene);
//
///**************************************************************************//**
//\brief Stores scene to scene table
//
//\param[in] storeScene - the pointer to Store Scene request
//
//\returns status of scene storing
//******************************************************************************/
//ZCL_Status_t storeScene(ZCL_StoreScene_t *storeScene);
#endif // _CUSTOMSCENESCLUSTER_H

// eof customScenesCluster.h


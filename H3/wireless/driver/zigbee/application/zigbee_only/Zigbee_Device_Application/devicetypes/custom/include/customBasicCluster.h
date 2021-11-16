
#ifndef _CUSTOMBASICCLUSTER_H
#define _CUSTOMBASICCLUSTER_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zigbee_device/clusters/include/haClusters.h>
#include <zcl/include/zclZllBasicCluster.h>
#include <zcl/include/zclCommandManager.h>

/******************************************************************************
                    Externals
******************************************************************************/
extern ZCL_BasicClusterServerAttributes_t customBasicClusterServerAttributes;
extern PROGMEM_DECLARE(ZCL_BasicClusterServerCommands_t customBasicClusterServerCommands);
extern ZCL_BasicClusterClientAttributes_t customBasicClusterClientAttributes;

/******************************************************************************
                    Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Initializes Basic cluster
******************************************************************************/
void customBasicClusterInit(void);


#endif // _CUSTOMBASICCLUSTER_H

// eof customBasicCluster.h

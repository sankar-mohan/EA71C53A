
#ifndef _CUSTOMGROUPSCLUSTER_H
#define _CUSTOMGROUPSCLUSTER_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zclZllGroupsCluster.h>

/******************************************************************************
                    Externals
******************************************************************************/
extern ZCL_GroupsClusterServerAttributes_t customGroupsClusterServerAttributes;
extern ZCL_GroupsClusterClientAttributes_t customGroupsClusterClientAttributes;
extern PROGMEM_DECLARE(ZCL_GroupsClusterCommands_t         customGroupsCommands);

/******************************************************************************
                    Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Initializes Groups cluster
******************************************************************************/
void customGroupsClusterInit(void);


#endif // _CUSTOMGROUPSCLUSTER_H

// eof customGroupsCluster.h

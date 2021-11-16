
#ifndef _CUSTOMCOMMISSIONINGCLUSTER_H
#define _CUSTOMCOMMISSIONINGCLUSTER_H

/******************************************************************************
                    Includes
******************************************************************************/
#include <zcl/include/zcl.h>
#include <zcl/include/zclZllCommissioningCluster.h>

/******************************************************************************
                    Externals
******************************************************************************/
extern ZCL_CommissioningClusterCommands_t customCommissioningClusterCommands;
extern ZCL_CommissioningClusterServerAttributes_t customCommissioningClusterServerAttributes;
extern ZCL_CommissioningClusterClientAttributes_t customCommissioningClusterClientAttributes;

/******************************************************************************
                    Prototypes
******************************************************************************/
void customCommissioningClusterInit(void);

#endif // _CUSTOMCOMMISSIONINGCLUSTER_H

// eof customCommissioningCluster.h


#ifndef _THFANCONTROLCLUSTER_H
#define _THFANCONTROLCLUSTER_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zclFanControlCluster.h>
#include <zigbee_device/clusters/include/haClusters.h>

/******************************************************************************
                    Externals
******************************************************************************/
extern ZCL_FanControlClusterServerAttributes_t customFanControlClusterServerAttributes;
extern ZCL_FanControlClusterClientAttributes_t customFanControlClusterClientAttributes;

/******************************************************************************
                    Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Initializes Fan Control cluster

\param[in]- None
\param[out] - None
******************************************************************************/
void customFanControlClusterInit(void);

/**************************************************************************//**
\brief Occupancy notification from Occupancy (client/server) cluster

\param[in] occupied - 0 - Not occupied, 1- occupied
\param[out] - None
******************************************************************************/
void customFanControlOccupancyNotify(bool occupied);

#endif // _THFANCONTROLCLUSTER_H

// eof thFanControlCluster.h
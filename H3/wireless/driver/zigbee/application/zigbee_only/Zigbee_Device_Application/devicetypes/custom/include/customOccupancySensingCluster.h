#ifndef _CUSTOMOCCUPANCYSENSINGCLUSTER_H
#define _CUSTOMOCCUPANCYSENSINGCLUSTER_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zcloccupancysensingcluster.h>
#include <zcl/include/zclZllGroupsCluster.h>
#include <zcl/include/zclZllIdentifyCluster.h>

/******************************************************************************
                    Definition(s) section
******************************************************************************/
#define OCCUPANCY_SENSING_VAL_MIN_REPORT_PERIOD 10
#define OCCUPANCY_SENSING_VAL_MAX_REPORT_PERIOD 20

/******************************************************************************
                    Externals
******************************************************************************/
extern ZCL_OccupancySensingClusterServerAttributes_t customOccupancySensingClusterServerAttributes;
extern ZCL_OccupancySensingClusterClientAttributes_t customOccupancySensingClusterClientAttributes;
extern ZCL_GroupsClusterClientAttributes_t  customGroupsClusterClientAttributes;

/******************************************************************************
                    Prototypes section
******************************************************************************/

/**************************************************************************//**
\brief Device initialization routine
******************************************************************************/
void customDeviceInit(void);

/**************************************************************************//**
\brief Initializes Occupancy Sensing cluster
******************************************************************************/
void customOccupancySensingClusterInit(void);

/**************************************************************************//**
\brief Toggles occupancy
******************************************************************************/
void occupancySensingToggleOccupancy(void);

#endif // _CUSTOMOCCUPANCYSENSINGCLUSTER_H

// eof customOccupancySensingCluster.h

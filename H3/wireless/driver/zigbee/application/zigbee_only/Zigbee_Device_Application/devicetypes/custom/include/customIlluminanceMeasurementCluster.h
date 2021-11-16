#ifndef _CUSTOMILLUMINANCEMEASUREMENTCLUSTER_H
#define _CUSTOMILLUMINANCEMEASUREMENTCLUSTER_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zclIlluminanceMeasurementCluster.h>
#include <zcl/include/zclZllGroupsCluster.h>

/******************************************************************************
                    Definition(s) section
******************************************************************************/
#define ILLUMINANCE_MEASUREMENT_VAL_MIN_REPORT_PERIOD 15
#define ILLUMINANCE_MEASUREMENT_VAL_MAX_REPORT_PERIOD 30

/* Some default values for attributes */
#define APP_ILLUMINANCE_MEASUREMENT_MEASURED_VALUE_ATTRIBUTE_VALUE       0x00ff
#define APP_ILLUMINANCE_MEASUREMENT_MIN_MEASURED_VALUE_ATTRIBUTE_VALUE   0x0002 //range 0x0002 – 0xfffd
#define APP_ILLUMINANCE_MEASUREMENT_MAX_MEASURED_VALUE_ATTRIBUTE_VALUE   0xfffe //range 0x0001 – 0xfffe
#define APP_ILLUMINANCE_MEASUREMENT_TOLERANCE_ATTRIBUTE_VALUE            0x0700 //0x0000 – 0x0800   //Optional
#define APP_ILLUMINANCE_MEASUREMENT_LIGHT_SENSOR_TYPE_VALUE              0x7f   //0x00 – 0xff       //Optional

/******************************************************************************
                    Externals
******************************************************************************/
extern ZCL_IlluminanceMeasurementClusterServerAttributes_t customIlluminanceMeasurementClusterServerAttributes;
extern ZCL_IlluminanceMeasurementClusterClientAttributes_t customIlluminanceMeasurementClusterClientAttributes;
extern ZCL_GroupsClusterClientAttributes_t  customGroupsClusterClientAttributes;

/******************************************************************************
                    Prototypes section
******************************************************************************/

/**************************************************************************//**
\brief Device initialization routine
******************************************************************************/
void customDeviceInit(void);

/**************************************************************************//**
\brief Initializes illuminance Measurement cluster
******************************************************************************/
void customIlluminanceMeasurementClusterInit(void);

/**************************************************************************//**
\brief Update the measured Value
******************************************************************************/
 void illuminanceMeasurementUpdateMeasuredValue(void);

/**************************************************************************//**
\brief Update the tolerance value
******************************************************************************/
 void illuminanceMeasurementUpdateTolerance(void);

#endif // _CUSTOMILLUMINANCEMEASUREMENTCLUSTER_H

// eof customilluminanceMeasureCluster.h

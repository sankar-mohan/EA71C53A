
#ifndef _CUSTOMHUMIDITYMEASUREMENTCLUSTER_H
#define _CUSTOMHUMIDITYMEASUREMENTCLUSTER_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zclHumidityMeasurementCluster.h>
#include <zcl/include/zclZllGroupsCluster.h>

/******************************************************************************
                    Definition(s) section
******************************************************************************/
#define HUMIDITY_MEASUREMENT_VAL_MIN_REPORT_PERIOD 18
#define HUMIDITY_MEASUREMENT_VAL_MAX_REPORT_PERIOD 27

/* Some default values for attributes */
#define APP_HUMIDITY_MEASUREMENT_MEASURED_VALUE_ATTRIBUTE_VALUE       0x1000
#define APP_HUMIDITY_MEASUREMENT_MIN_MEASURED_VALUE_ATTRIBUTE_VALUE   0x0000 //range 0x0 – 0x270f
#define APP_HUMIDITY_MEASUREMENT_MAX_MEASURED_VALUE_ATTRIBUTE_VALUE   0x2710 //range 0x954e – 0x2710
#define APP_HUMIDITY_MEASUREMENT_TOLERANCE_ATTRIBUTE_VALUE            0x0100 //0x0000 – 0x0800

#define APP_HUMIDITY_MEASUREMENT_MEASURED_VALUE_PERIODIC_CHANGE       1000 //coresponding to 10%  
/******************************************************************************
                    Externals
******************************************************************************/
extern ZCL_HumidityMeasurementClusterServerAttributes_t customHumidityMeasurementClusterServerAttributes;
extern ZCL_HumidityMeasurementClusterClientAttributes_t customHumidityMeasurementClusterClientAttributes;
extern ZCL_GroupsClusterClientAttributes_t  customGroupsClusterClientAttributes;

/******************************************************************************
                    Prototypes section
******************************************************************************/

/**************************************************************************//**
\brief Device initialization routine
******************************************************************************/
void customDeviceInit(void);

/**************************************************************************//**
\brief Initializes Humidity Measurement cluster
******************************************************************************/
void customHumidityMeasurementClusterInit(void);

/**************************************************************************//**
\brief Update the measured Value
******************************************************************************/
void humidityMeasurementUpdateMeasuredValue(void);

#endif // _CUSTOMHUMIDITYMEASUREMENTCLUSTER_H

// eof customHumidityMeasurementCluster.h

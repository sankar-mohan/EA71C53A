#ifndef _CUSTOMTEMPERATUREMEASUREMENTCLUSTER_H
#define _CUSTOMTEMPERATUREMEASUREMENTCLUSTER_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zclTemperatureMeasurementCluster.h>
#include <zcl/include/zclZllGroupsCluster.h>


/******************************************************************************
                    Definition(s) section
******************************************************************************/
#define TEMPERATURE_MEASUREMENT_VAL_MIN_REPORT_PERIOD 5
#define TEMPERATURE_MEASUREMENT_VAL_MAX_REPORT_PERIOD 15

/* Some default values for attributes */
#define APP_TEMPERATURE_MEASUREMENT_MEASURED_VALUE_ATTRIBUTE_VALUE       0x5000
#define APP_TEMPERATURE_MEASUREMENT_MIN_MEASURED_VALUE_ATTRIBUTE_VALUE   0x954d //range 0x954d � 0x7ffe
#define APP_TEMPERATURE_MEASUREMENT_MAX_MEASURED_VALUE_ATTRIBUTE_VALUE   0x7fff //range 0x954e � 0x7fff
#define APP_TEMPERATURE_MEASUREMENT_TOLERANCE_ATTRIBUTE_VALUE            0x0100 //0x0000 � 0x0800

#define APP_TEMPERATURE_MEASUREMENT_MEASURED_VALUE_PERIODIC_CHANGE       1000 //corresponding to 10 degree celsius
/******************************************************************************
                    Externals
******************************************************************************/
extern ZCL_TemperatureMeasurementClusterServerAttributes_t customTemperatureMeasurementClusterServerAttributes;
extern ZCL_TemperatureMeasurementClusterClientAttributes_t customTemperatureMeasurementClusterClientAttributes;
extern ZCL_GroupsClusterClientAttributes_t  customGroupsClusterClientAttributes;

/******************************************************************************
                    Prototypes section
******************************************************************************/

/**************************************************************************//**
\brief Device initialization routine
******************************************************************************/
void customDeviceInit(void);

/**************************************************************************//**
\brief Initializes Temperature Measurement cluster
******************************************************************************/
void customTemperatureMeasurementClusterInit(void);

/**************************************************************************//**
\brief Update the measured Value
******************************************************************************/
void customTempeartureMeasurementUpdateMeasuredValue(void);

#endif // _CUSTOMTEMPERATUREMEASUREMENTCLUSTER_H

// eof tsTemperatureMeasurementCluster.h

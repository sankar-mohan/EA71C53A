
#ifndef _CUSTOMTHERMOSTATUICONFCLUSTER_H
#define _CUSTOMTHERMOSTATUICONFCLUSTER_H
/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zclThermostatUiConfCluster.h>
/******************************************************************************
                    External variables section
******************************************************************************/
extern ZCL_ThermostatUiConfClusterServerAttributes_t customThermostatUiConfClusterServerAttributes;
extern ZCL_ThermostatUiConfClusterClientAttributes_t customThermostatUiConfClusterClientAttributes;
/******************************************************************************
                    Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Initializes Thermostat Ui Conf cluster
******************************************************************************/
void customThermostatUiConfClusterInit(void);
int8_t customTranslateZclCelciusTemptoFahr(int16_t temperature);

#endif // _CUSTOMTHERMOSTATUICONFCLUSTER_H

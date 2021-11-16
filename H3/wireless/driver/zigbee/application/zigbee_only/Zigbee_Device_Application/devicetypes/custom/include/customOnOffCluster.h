#ifndef _CUSTOMONOFFCLUSTER_H
#define _CUSTOMONOFFCLUSTER_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zclZllOnOffCluster.h>
#include <zigbee_device/clusters/include/onOffCluster.h>

/******************************************************************************
                    Definition(s) section
******************************************************************************/
#define ONOFF_VAL_MIN_REPORT_PERIOD 30
#define ONOFF_VAL_MAX_REPORT_PERIOD 60

/******************************************************************************
                    Externals
******************************************************************************/
extern ZCL_OnOffClusterServerAttributes_t customOnOffClusterServerAttributes;
extern ZCL_OnOffClusterClientAttributes_t customOnOffClusterClientAttributes;
extern PROGMEM_DECLARE(ZCL_OnOffClusterCommands_t         customOnOffCommands);

/******************************************************************************
                    Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Initializes On/Off cluster
******************************************************************************/
void customOnOffClusterInit(void);

/**************************************************************************//**
\brief Set Extension Field Set
******************************************************************************/
void setOnOff(bool onOff);

/**************************************************************************//**
\brief gets on/off state

\return on/off state
******************************************************************************/
bool onOffState(void);

/**************************************************************************//**
\brief Set Extension Field Set
******************************************************************************/
void onOffClusterSetExtensionField(bool onOff);

/**************************************************************************//**
\brief Set Global Scene Control attribute to 'true'
******************************************************************************/
void onOffClusterSetGlobalSceneControl(void);

/**************************************************************************//**
\brief Update device ON state as per StartUpOnOff attribute value
******************************************************************************/
void customUpdateStartUpOnOffState(void);

#endif // _CUSTOMONOFFCLUSTER_H

// eof customOnOffCluster.h


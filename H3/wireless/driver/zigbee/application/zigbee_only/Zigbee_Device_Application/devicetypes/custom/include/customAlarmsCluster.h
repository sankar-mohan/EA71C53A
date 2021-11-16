#ifndef _CUSTOMALARMSCLUSTER_H
#define _CUSTOMALARMSCLUSTER_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zclAlarmsCluster.h>

/******************************************************************************
                    Defines section
******************************************************************************/
#define ALARM_TABLE_SIZE 5

/******************************************************************************
                    External variables section
******************************************************************************/
extern ZCL_AlarmsClusterServerAttributes_t customAlarmsClusterServerAttributes;
extern ZCL_AlarmsClusterClientAttributes_t customAlarmsClusterClientAttributes;
extern PROGMEM_DECLARE (ZCL_AlarmsClusterServerCommands_t   customAlarmsClusterServerCommands);
extern PROGMEM_DECLARE (ZCL_AlarmsClusterClientCommands_t   customAlarmsClusterClientCommands);

/******************************************************************************
                    Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Initializes Identify cluster
******************************************************************************/
void customAlarmsClusterInit(void);

/**************************************************************************//**
\brief alarm notification raised by other clusters

\param[in] alarmCode - alarm code;
\param[in] clusterId - cluster identifier;
******************************************************************************/
void ZCL_AlarmNotification(ZCL_Alarm_t *alarm, Endpoint_t srcEndPoint, bool masked);

/**************************************************************************//**
\brief Add alarm entry

\param[in] alarmCode - alarm code;
\param[in] clusterId - cluster identifier;
******************************************************************************/
void addAlarmEntry(uint8_t alarmCode, ClusterId_t clusterId);

/**************************************************************************//**
\brief Removes the corresponding entry from the alarm log

\param[in] alarmCode - alarm code;
\param[in] clusterId - cluster identifier;
******************************************************************************/
void removeAlarmID(uint8_t alarmCode, ClusterId_t clusterId);

/**************************************************************************//**
\brief Sends Reset alarm command unicastly

\param[in] mode - address mode;
\param[in] addr - short address of destination node;
\param[in] ep   - destination endpoint;
\param[in] srcEp   - source endpoint;
\param[in] alarmCode - alarm code to be reset;
\param[in] clsuetrId   - cluster identifier;
******************************************************************************/
void alarmsSendResetAlarm(APS_AddrMode_t mode, ShortAddr_t addr, Endpoint_t ep, Endpoint_t srcEp, uint8_t alarmCode, ClusterId_t clusterId);

/**************************************************************************//**
\brief Sends Reset all alarms command unicastly

\param[in] mode - address mode;
\param[in] addr - short address of destination node;
\param[in] ep   - destination endpoint;
\param[in] srcEp- source endpoint;
******************************************************************************/
void alarmsSendResetAllAlarms(APS_AddrMode_t mode, ShortAddr_t addr, Endpoint_t ep, Endpoint_t srcEp);

/**************************************************************************//**
\brief Sends Get Alarm command unicastly

\param[in] mode - address mode;
\param[in] addr - short address of destination node;
\param[in] ep   - destination endpoint;
\param[in] srcEp- source endpoint;
******************************************************************************/
void alarmsSendGetAlarm(APS_AddrMode_t mode, ShortAddr_t addr, Endpoint_t ep, Endpoint_t srcEp);

/**************************************************************************//**
\brief Sends Get Alarm command unicastly

\param[in] mode - address mode;
\param[in] addr - short address of destination node;
\param[in] ep   - destination endpoint;
\param[in] srcEp- source endpoint;
******************************************************************************/
void alarmsSendResetAlarmLog(APS_AddrMode_t mode, ShortAddr_t addr, Endpoint_t ep, Endpoint_t srcEp);

#endif // _CUSTOMALARMSCLUSTER_H

// eof customAlarmsCluster.h

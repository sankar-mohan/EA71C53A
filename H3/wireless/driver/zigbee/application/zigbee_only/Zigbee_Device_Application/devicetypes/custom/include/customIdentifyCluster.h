
#ifndef _CUSTOMIDENTIFYCLUSTER_H
#define _CUSTOMIDENTIFYCLUSTER_H

/******************************************************************************
                    Defines
******************************************************************************/
#define IDENTIFY_NON_COLOR_EFFECT false
#define IDENTIFY_COLOR_EFFECT     true
/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zclZllIdentifyCluster.h>

/******************************************************************************
                    Externals
******************************************************************************/
extern ZCL_IdentifyClusterServerAttributes_t customIdentifyClusterServerAttributes;
extern ZCL_IdentifyClusterClientAttributes_t customIdentifyClusterClientAttributes;
extern PROGMEM_DECLARE(ZCL_IdentifyClusterCommands_t   customIdentifyCommands);

/******************************************************************************
                    Prototypes section
******************************************************************************/
/******************************************************************************
\brief initialization of  identify cluster
******************************************************************************/
void customIdentifyClusterInit(void);

/**************************************************************************//**
\brief Checks if identification process is in progress

\returns true if it is, false otherwise
******************************************************************************/
bool customIdentifyIsIdentifying(void);

/**************************************************************************//**
\brief Makes device to start identify itself

\param[in] time - identifying time in seconds
******************************************************************************/
void customIdetifyStartIdentifyingCb(uint16_t time, void (*cb)(void));

/**************************************************************************//**
\brief Stops identifying on endpoints
******************************************************************************/
void customIdentifyStop(void);

/**************************************************************************//**
\brief Shows identification effect in way specified.

\param[in] identifyTime - identifying period in seconds.
\param[in] colorEffect - flag to set for color effects.
\param[in] enhancedHue - enhanced hue to move for color effects.
******************************************************************************/
void customIdentifyStart(uint16_t identifyTime, bool colorEffect, uint16_t enhancedHue);

#endif // _CUSTOMIDENTIFYCLUSTER_H

// eof customIdentifyCluster.h


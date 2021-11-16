#ifndef _CUSTOMTIMECLUSTER_H
#define _CUSTOMTIMECLUSTER_H
/******************************************************************************
                    Includes section
******************************************************************************/
#include <zcl/include/zclTimeCluster.h>
/******************************************************************************
                    Defines section
******************************************************************************/
#define TIME_SERVER_MASTER           true
#define TIME_SERVER_MASTER_ZONE_DST  true
/******************************************************************************
                    Types section
******************************************************************************/
typedef struct PACK
{
  uint8_t date;
  uint8_t month;
  uint16_t year;
  uint8_t hour;
  uint8_t minute;
  uint8_t seconds;
}dateTime_t;

typedef enum
{
  MONTH_JANUARY = 1,
  MONTH_FEBRUARY,
  MONTH_MARCH,
  MONTH_APRIL,
  MONTH_MAY,
  MONTH_JUNE,
  MONTH_JULY,
  MONTH_AUGUST,
  MONTH_SEPTEMBER,
  MONTH_OCTOBER,
  MONTH_NOVEMBER,
  MONTH_DECEMBER 
}month_t; 
/******************************************************************************
                    External variables section
******************************************************************************/
extern ZCL_TimeClusterServerAttributes_t customTimeClusterServerAttributes;

/******************************************************************************
                    Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Initializes Time cluster
******************************************************************************/
void customTimeClusterInit(void);

/**************************************************************************//**
\brief set the UTCTime
\param date and time
******************************************************************************/
void timeSetTimeAttribute(dateTime_t setDateTime);

/**************************************************************************//**
\brief to read the UTCTime
\param none
******************************************************************************/
uint32_t timeReadTimeAttribute(void);

/**************************************************************************//**
\brief find UTCTime - the no of seconds elapsed since  
                      0hrs 0mins 0secs of 1st January,2000
\param pointer to dateTime structure
******************************************************************************/
uint32_t timeCalculateUTCTime(dateTime_t* dateTime);

/**************************************************************************//**
\brief set the time zone and DST attributes
\param timeZone, dstStart, dstEnd and dstShift in seconds
******************************************************************************/
void timeSetTimezoneAndDSTAttributes(int32_t timeZone, uint32_t dstStart, uint32_t dstEnd, int32_t dstShift);

/**************************************************************************//**
\brief set the time status attribute bits
\param master - true if the server is the master
       synchronized - true if the time is synchronized with the network
       masterZoneDST - true if the server is the master for timeZone and DST
       Superseding - true if the server shall be choosen as best among other servers
******************************************************************************/
void timeSetTimeStatus(bool master, bool synchronized, bool masterZoneDst, bool superseding);

#endif // _CUSTOMTIMECLUSTER_H
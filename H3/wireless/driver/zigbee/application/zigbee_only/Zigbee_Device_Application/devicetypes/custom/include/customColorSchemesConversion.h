
#ifndef _CUSTOMCOLORSCHEMESCONVERTION_H
#define _CUSTOMCOLORSCHEMESCONVERTION_H

/******************************************************************************
                    Includes section
******************************************************************************/
#include <systemenvironment/include/sysTypes.h>

/******************************************************************************
                    Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Converts color temperature to appropriate XY coordinates

\param[in] temperature - color temperature (attribute value);
\param[out] x          - x coordinate (attribute value);
\param[out] y          - y coordinate (attribute value)
******************************************************************************/
void customConvertColorToXY(uint16_t temperature, uint16_t *x, uint16_t *y);

/**************************************************************************//**
\brief Converts XY coordinates to appropriate color temperature

\param[in] x - x coordinate (attribute value);
\param[in] y - y coordinate (attribute value)

\returns color temperature (attribute value)
******************************************************************************/
uint16_t customConvertXYToColor(uint16_t x, uint16_t y);

#endif // _CUSTOMCOLORSCHEMESCONVERTION_H

// eof customColorSchemesConversion.h


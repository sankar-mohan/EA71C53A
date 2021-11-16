#ifndef POWER_H
#define POWER_H

#include <xc.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    PMU_MODE_MLDO = 1,
    PMU_MODE_BUCK_PWM,   // 2
    PMU_MODE_BUCK_PSM   // 3
} PMU_Mode_T;
 
// *****************************************************************************
/**
*@brief This routine will set the power mode of the system.
*
*@param PMU_Mode_T    PMU_MODE_MLDO         // 1
*                     PMU_MODE_BUCK_PWM     // 2
*                     PMU_MODE_BUCK_PSM     // 3
*
*@retval uint8_t      0 is success
*                     1 is fail which means no BDADDR IB
*/  
uint8_t PMU_Set_Mode(PMU_Mode_T mode);

#ifdef __cplusplus
}
#endif

#endif /* POWER_H_INCLUDED */

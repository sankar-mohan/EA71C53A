/*******************************************************************************
  HAL Sleep Source File

  Company:
    Microchip Technology Inc.

  File Name:
    halSleep.c

  Summary:
    This file contains the Implementation of HAL sleep modes.

  Description:
    This file contains the Implementation of HAL sleep modes.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END


/******************************************************************************
 *   WARNING: CHANGING THIS FILE MAY AFFECT CORE FUNCTIONALITY OF THE STACK.  *
 *   EXPERT USERS SHOULD PROCEED WITH CAUTION.                                *
 ******************************************************************************/
/******************************************************************************
                   Includes section
******************************************************************************/
#include <hal/cortexm4/pic32cx/include/halRfCtrl.h>
#include <hal/cortexm4/pic32cx/include/halAppClock.h>
#include <hal/cortexm4/pic32cx/include/halSleep.h>
#include <hal/cortexm4/pic32cx/include/halSleepTimerClock.h>
#include <systemenvironment/include/sysEvents.h>
#include <systemenvironment/include/sysIdleHandler.h>
#include <mac_phy/mac_hwd_phy/RF231_RF212/PHY/include/at86rf233.h>
#include <mac_phy/mac_hwd_phy/RF231_RF212/PHY/include/phyDeepSleep.h>
#ifdef _PIC32CX_
#include <mac_phy/mac_hwd_phy/RF231_RF212/PHY/include/phyPic32cx.h>
#else
#include <mac_phy/mac_hwd_phy/RF231_RF212/PHY/include/phyRfSpiProto.h>
#endif

#define Sleep_Check(x)
#define XTAL_TURN_OFF 1
uint32_t g_refoReg[6];

#ifndef _PIC32CX_
/******************************************************************************
                   Define(s) section
******************************************************************************/

/******************************************************************************
                   Prototypes section
******************************************************************************/
void halSwitchToActiveClock(void);


/**************************************************************************//**
\brief  Turn on pin 9 (DIG1) and pin 10 (DIG2) to enable antenna select.
******************************************************************************/
void phyAntennaSwitcherOn(void);

/**************************************************************************//**
\brief  Turn off pin 9 (DIG1) and pin 10 (DIG2) to disable antenna select.
******************************************************************************/
void phyAntennaSwitcherOff(void);

/******************************************************************************
                   External global variables section
******************************************************************************/
extern HAL_SleepControl_t halSleepControl;

/******************************************************************************
                   Global variables section
******************************************************************************/
static uint64_t halTimeStartOfSleep = 0ull;
static bool mcuSleep = false;
static bool trxSleep = false;


/******************************************************************************
                   Implementations section
******************************************************************************/
/**************************************************************************//**
\brief Switch on system power.

\param[in]
  wakeupSource - wake up source
******************************************************************************/
void halPowerOn(const uint8_t wakeupSource)
{
  halSleepControl.wakeupStation = HAL_ACTIVE_MODE;
  halSleepControl.wakeupSource = wakeupSource;

  if (trxSleep)
  {
    HAL_ClearRfSlpTr();
  }

  //halPostTask(HAL_WAKEUP);
}

/*******************************************************************************
 Put the cpu in sleep state
*******************************************************************************/
void sleep(void)
{
  __DSB();
  __WFI();
}

/*******************************************************************************
 set/prepare sleep mode
*******************************************************************************/
void halSetSleepMode(HALSleepModes_t sleepMode)
{
  switch(sleepMode)
  {
    case SLEEPMODE_IDLE_0:
    case SLEEPMODE_IDLE_1:
    case SLEEPMODE_IDLE_2:
    {
      // todo SCB_SCR_s.sleepdeep = 0;//make sure it is not standby
      // todo PM_SLEEP_s.idle = (uint8_t)sleepMode;
    }
    break;
    case SLEEPMODE_STANDBY:
      {
        // todo SCB_SCR_s.sleepdeep = 1; //enable standby
      }
     break;
  }
}

/*******************************************************************************
 \brief prepare for sleep
*******************************************************************************/
void halPrepareForSleep(void)
{
  /* Disable USB, enabled by default */
  // todo PM_APBBMASK_s.usb = 0;
  // todo PM_AHBMASK_s.usb = 0;

  /* Disable BOD33 */
  // todo SYSCTRL_BOD33_s.enable = 0;
}

/*******************************************************************************
 \brief Restoring the state atter wakeup
*******************************************************************************/
void halRestoreAfterWakeup(void)
{
  /* enable USB, a sit is enabled by default */
  // todo PM_APBBMASK_s.usb = 1;
  // todo PM_AHBMASK_s.usb = 1;

  /* Enable BOD33 */
  // todo SYSCTRL_BOD33_s.enable = 1;
}

/******************************************************************************
 \brief Shutdowns system.
  NOTES:
  the application should be sure the poweroff will not be
  interrupted after the execution of the sleep().
******************************************************************************/
void halPowerOff(void)
{
  HAL_SleepMode_t sleepMode = HAL_SLEEP_MODE_POWER_DOWN;

  if (HAL_ACTIVE_MODE == halSleepControl.wakeupStation)
    return;  // it is a too late to sleep.

  // stop application timer clock
  halStopAppClock(); // will be shutdown
  if (0ul == halTimeStartOfSleep)
  { // start of sleep procedure
    // save time of stopping of the application timer
    halTimeStartOfSleep = halGetTimeOfSleepTimer();
  }

  if (HAL_SLEEP_TIMER_IS_STARTED == halSleepControl.sleepTimerState)
    sleepMode = HAL_SLEEP_MODE_POWER_SAVE;

  if (HAL_SLEEP_STATE_CONTINUE_SLEEP != halSleepControl.sleepState)
  {
    HAL_DisableRxTxSwitcher();

    SYS_PostEvent((SYS_EventId_t)HAL_EVENT_FALLING_ASLEEP, (SYS_EventData_t)&sleepMode);
  }
  else
    SYS_PostEvent((SYS_EventId_t)HAL_EVENT_CONTINUING_SLEEP, (SYS_EventData_t)&sleepMode);

#ifdef CLKM
  SYS_PostEvent((SYS_EventId_t)BC_EVENT_PHY_INIT, INITIALIZING_SLEEP_STATE);
#endif

  if (halRadioInTrxOff())
  {
    if (BSP_IsAntDiversityActionsRequired())
      // disable antenna diversity switcher
      phyAntennaSwitcherOff();
    /* Put the transceiver in deep sleep if trx state is TRX_OFF */
    HAL_SetRfSlpTr();
    trxSleep = true;
  }

  halSleepControl.sleepState = HAL_SLEEP_STATE_CONTINUE_SLEEP;
#if defined(_SLEEP_WHEN_IDLE_)
  if (SYS_IsSleepWhenIdleEnabled())
  {
    halPrepareForSleep();
    __enable_irq();
    mcuSleep = true;

    if (SYS_taskFlag || (!trxSleep))
    {
		// During device power off procedure some source has posted a task
		// so the power on procedure shall be called
		if (HAL_ACTIVE_MODE != halSleepControl.wakeupStation)
			halPowerOn(HAL_SLEEP_TIMER_IS_WAKEUP_SOURCE);
		// post task for task manager
		if (HAL_SLEEP_TIMER_IS_STARTED == halSleepControl.sleepTimerState)
		{
			halInterruptSleepClock();
		}
	
	return;
    }

    halSleepControl.sleepMode = sleepMode;
    if (HAL_SLEEP_MODE_IDLE == sleepMode)
    {
      halSetSleepMode(SLEEPMODE_IDLE_0);
      sleep();
    }
    else if (HAL_SLEEP_MODE_POWER_SAVE == sleepMode)
    { // sleep timer is started
      halSetSleepMode(SLEEPMODE_STANDBY);
      sleep();
    }
    else
    {
      halStopSleepTimerClock();//sleep timer is also stopped
      halSetSleepMode(SLEEPMODE_STANDBY);
      sleep();
      halStartSleepTimerClock(); 
    }
  }
#endif /* _SLEEP_WHEN_IDLE_ */
}

/********************** ********************************************************
  Prepares system for power-save, power-down.
  Power-down the mode is possible only when internal RC is used
  Parameters:
  none.
  Returns:
  0 if there is a possibililty to power-down system.
******************************************************************************/
int HAL_Sleep(void)
{
  halSleepControl.wakeupStation = HAL_SLEEP_MODE;  // the reset of sign of entry to the sleep mode.  
  //halPostTask(HAL_SLEEP);
  return 0;
}

/******************************************************************************
                   Implementations section
******************************************************************************/
/******************************************************************************
 Handler for task manager. It is executed when system has waked up.
******************************************************************************/
void halWakeupHandler(void)
{
  uint64_t timeEndOfSleep;
  HAL_SleepControl_t wakeupSource;

  if (mcuSleep)
  {
#ifndef _PIC32CX_
    halSwitchToActiveClock();
#endif
    halRestoreAfterWakeup();

    timeEndOfSleep = halGetTimeOfSleepTimer();

    timeEndOfSleep -= halTimeStartOfSleep;  // time of sleep
    halTimeStartOfSleep = 0ul;
    // adjust application timer interval
    halAdjustSleepInterval((uint32_t)timeEndOfSleep);

    // start application timer clock
    halStartAppClock();
	mcuSleep = false;
  }

  if (trxSleep)
  {
    // wakeup the transceiver by setting SLPTR to '0'
    HAL_ClearRfSlpTr();

    // Wait for radio to wake up.
    halWaitRadio();
    trxSleep = false;
    if (BSP_IsAntDiversityActionsRequired())
      // enable antenna diversity switcher
      phyAntennaSwitcherOn();
  }

#ifdef CLKM
  SYS_PostEvent((SYS_EventId_t)BC_EVENT_PHY_INIT, INITIALIZING_WAKE_UP_STATE);
#endif

  halSleepControl.sleepState = HAL_SLEEP_STATE_BEGIN;

  memcpy(&wakeupSource, &halSleepControl, sizeof(HAL_SleepControl_t));

  SYS_PostEvent((SYS_EventId_t)HAL_EVENT_WAKING_UP, (SYS_EventData_t)&wakeupSource);

  HAL_EnableRxTxSwitcher();
  halSleepControl.sleepMode = HAL_SLEEP_MODE_NONE;
  if (HAL_SLEEP_TIMER_IS_WAKEUP_SOURCE == halSleepControl.wakeupSource)
  {
    if (halSleepControl.callback)
      halSleepControl.callback(halSleepControl.startPoll);
  }
}

/*******************************************************************************
  Makes MCU enter Sleep mode.
*******************************************************************************/
void HAL_IdleMode(void)
{

}
/*******************************************************************************
  Shuts down the following Peripherials to save power
*******************************************************************************/
void HAL_DisablePeripherials(void)
{
  // todo PM_APBAMASK_s.wdt = 0;
  // todo PM_AHBMASK_s.usb = 0;
  // todo PM_APBBMASK_s.usb = 0;
  // todo SYSCTRL_XOSC_s.enable = 0;
  // todo PM_APBCMASK_s.sercom0 = 0;
  // todo PM_APBCMASK_s.sercom1 = 0;
  // todo PM_APBCMASK_s.sercom2 = 0;
  // todo PM_APBCMASK_s.sercom3 = 0;
  // todo PM_APBCMASK_s.sercom4 = 0;
}
/*******************************************************************************
  Enables USART1
*******************************************************************************/
void HAL_EnableUSART1(void)
{
  // todo PM_APBCMASK_s.sercom0 = 1;
}
/*******************************************************************************
  Disables Pull up
*******************************************************************************/
void HAL_DisablePUD(void)
{
}
/*******************************************************************************
  Enables Pull up
*******************************************************************************/
void HAL_EnablePUD(void)
{   
}
/*******************************************************************************
  Enables Timer/counter 2
*******************************************************************************/
void HAL_EnableSleepTimer(void)
{
}
/*******************************************************************************
  Enables Transceiver clock
*******************************************************************************/
void HAL_EnableTransceiver(void)
{
  // todo PM_APBCMASK_s.rfctrl = 1;
}

#endif //#infdef _PIC32CX_

/*******************************************************************************
  To Enter into MCU Sleep 
*******************************************************************************/
void HAL_Pic32cxSleep()
{
  uint32_t reg32;
  uint16_t reg16;
  // uint8_t  reg8;
  // uint32_t current_time;
  // uint32_t target_time;

  // Step 1 : For connected sleep case, if SOSC clock is available on the module, set 32K CLK source to SOSC CLK in CRU and set MLPCLK_MOD to 1 to divide 32.768kHz clock to 32kHz clock to save power. 
  // Otherwise, set 32K CLK source to POSC CLK in CRU to ensure accuracy of 32K CLK
  // For unconnected sleep case, set 32K CLK source to LPRC CLK in CRU to save power since accuracy requirement does not need to be met
  // step 1 has moved to CLK_Initialize()

    
  // Step 6 : Disable bt_zb_dbg bus toggling, can be moved to system initialization procedure after boot up
  reg32 = BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG1;
  reg32 &= (~0x800);
  reg32 |= 0x1;
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG1 = reg32;   

  // Step 14 : Disable PCHE Cache, which is proposed by SOC team for low power optimization
  PCHE_REGS->PCHE_CHECON = 0xf;
  
  // Step 15 : Set PB1 CLK to SYS_CLK/5, which is proposed by SOC team for low power optimization
  CRU_REGS->CRU_PB1DIV = 0x8804;

  // Step 16 : set REFOx registers to 0, combining step 17 to de-assert external PLL request 
  //since we don't know how and when plib_clock will be changed. so we will backup all 6 sets REFOx registers.
  //Ensure the Reference Clock Out Module is enabled, REFO1 Clock is selected by CFG_CFGPCLKGEN1
  if (CFG_REGS->CFG_PMD2 & CFG_PMD2_REFO1MD_Msk)
  {
    while (CFG_REGS->CFG_CFGCON0 & CFG_CFGCON0_PMDLOCK_Msk);
    CFG_REGS->CFG_PMD2 &= ~CFG_PMD2_REFO1MD_Msk;
  }

  g_refoReg[0] = CRU_REGS->CRU_REFO1CON & 0xFFFFFEFF;   ////Bit 8 is read only
  g_refoReg[1] = CRU_REGS->CRU_REFO2CON & 0xFFFFFEFF;
  g_refoReg[2] = CRU_REGS->CRU_REFO3CON & 0xFFFFFEFF;
  g_refoReg[3] = CRU_REGS->CRU_REFO4CON & 0xFFFFFEFF;
  g_refoReg[4] = CRU_REGS->CRU_REFO5CON & 0xFFFFFEFF;
  g_refoReg[5] = CRU_REGS->CRU_REFO6CON & 0xFFFFFEFF;
  
  if (CRU_REGS->CRU_REFO1CON & CRU_REFO1CON_ON_Msk)
  {
    while (!(CRU_REGS->CRU_REFO1CON & CRU_REFO1CON_ACTIVE_Msk));
    CRU_REGS->CRU_REFO1CONCLR = CRU_REFO1CON_ON_Msk;
  }

  while (CRU_REGS->CRU_REFO1CON & CRU_REFO1CON_ACTIVE_Msk);
  CRU_REGS->CRU_REFO1CONCLR = 0xFFFFFEFF;   //Bit 8 is read only


  // Step 17 : Change SYS CLK source in CRU from SPLL1 CLK to POSC CLK 
  CRU_REGS->CRU_OSCCON &= (~0xf01); 
  CRU_REGS->CRU_OSCCON |= 0x200;

  CRU_REGS->CRU_OSCCONSET = CRU_OSCCON_OSWEN_Msk;      /* request oscillator switch to occur */

  while(CRU_REGS->CRU_OSCCON & CRU_OSCCON_OSWEN_Msk);    /* wait for indication of successful clock change before proceeding */
  

  // Step 18 : Set subsys_clk_src_sel to 0 to change subsys clock source from PLL CLK to XTAL CLK
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG1 &= (~0x30);
  
  // Step 19 : Set PLL_PWD to 1 to disable CLKGEN Clock enable in RF
  BLE_REGS->BLE_DPLL_RG2 |= 0x2;

  // Step 20 : Set die_BENXOANA_ovrd_en to 1 to switch LDO_PLL to CLDO
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG0 |= 0x800;

  // Step 21 : Set EN_RFLDO_ovrd_en to 1
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG0 |= 0x8000;

  // Step 22 : Combining step 21, de-assert external PLL power request to disable CLKGEN LDO enable in RF
  CRU_REGS->CRU_SPLLCON |= 0x08;  //Force the SPLL lock signal to be asserted
  
  // Step 23 : set KEEP_ACLB_CLOCKS_ON to 1 to enable ACLB clocks for SPI access
 // BLE_REGS->BLE_RFPWRMGMT |= 0x40;
  PHY_BleRfPwrMgmt(0x40 , false);

  // Step 24 : set spi_ldo_en to 0 via ACLB config register to turn off MBS in RF
  reg16 = aclb_spi_read(0x5D);  //reg16 = RF_Read_RegWRet(0x5D);
  reg16 &= (~0x400);
  aclb_spi_write(0x5D,reg16);  //RF_Write_Reg(0x5D, reg16);
  
  // Step 25 : set KEEP_ACLB_CLOCKS_ON to 0 to enable ACLB clocks for SPI access
  // BLE_REGS->BLE_RFPWRMGMT &= (~0x40);
  PHY_BleRfPwrMgmt(0x40 , false);
  
  // Step 26 : set bt_en_main_clk to 0 to disable BT main clock
  //BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG0 &= (~0x00100000); 
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG0 &= (~0x00100010); //Disable both zigbee  and BLE 
   
  // Step 27 : change CLK source in CRU from POSC CLK to FRC CLK
  CRU_REGS->CRU_OSCCON &= (~0xf01); 
  CRU_REGS->CRU_OSCCONSET = CRU_OSCCON_OSWEN_Msk;  /* request oscillator switch to occur */
  while(CRU_REGS->CRU_OSCCON & CRU_OSCCON_OSWEN_Msk);    /* wait for indication of successful clock change before proceeding */

  // Step 27-1 : //CHMRSW-732- If XTAL clock is OFF when bt_zb_subsys enters into sleep mode,
  //set subsys_clk_src_sel to 1 via subsys config register (SUBSYS_CNTRL_REG1_ADDR[4]) to select PLL CLK as SRC clock
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG1 |= 0x10;

  // Step 28 : If XTAL clock is OFF when bt_zb_subsys enters into sleep mode, set BXTLEN to 0 via BT config register to disable XTAL
#ifdef XTAL_TURN_OFF
  x_asm_delay(4); // add 2us delay
 // BLE_REGS->BLE_RFPWRMGMT &= (~0x02);
  PHY_BleRfPwrMgmt(0x02 , false);
#endif   

  
}

/*******************************************************************************
  TO Exit from MCU Sleep
*******************************************************************************/
void HAL_Pic32cxWakeup()
{
  uint32_t reg32;
  uint16_t reg16;
//  uint8_t  reg8;

  // Step 1 : Wait for the wake up event from either self-generated timer interrupt OR from NMI. 
  //      In template code, clki match signal would be disabled after bt_zb_subsys NMI is set, so wait for clki match signal de-assertion here   
//  reg32 = BLE_REGS->BLE_PERIODICAL_CLK_SIGNAL_GENERATOR_PCSG;
//  while ((reg32 & 0x04) != 0x0)
//  {
//    reg32  = BLE_REGS->BLE_PERIODICAL_CLK_SIGNAL_GENERATOR_PCSG;
//  }

  while (RCON_REGS->RCON_RCON & 0x08);  

  // Step 2 : If XTAL clock is off when bt_zb_subsys enters into low power mode, wait for xtal_ready_out_sync
#ifdef XTAL_TURN_OFF
  //Enable bit 7 to creates one clk_lp_cycle wide pulse on ZBT Subsystem.external_NMI0 pin
  CFG_REGS->CFG_CFGCON1 |= CFG_CFGCON1_ZBTWKSYS_Msk; //Wake-up BT Subsystem

  reg32 = BTZBSYS_REGS->BTZBSYS_SUBSYS_STATUS_REG1;
  while ((reg32 & 0x01) != 0x01)
  {
    reg32 = BTZBSYS_REGS->BTZBSYS_SUBSYS_STATUS_REG1;
  }  
#endif
  
  //If XTAL clock is OFF when bt_zb_subsys enters into sleep mode, 
  //set subsys_clk_src_sel to 0 via subsys config register (SUBSYS_CNTRL_REG1_ADDR[4]) to select XTAL CLK as SRC clock
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG1 &= (~0x10);

  // Step 3 : Change CLK source in CRU from FRC CLK to POSC CLK
  CRU_REGS->CRU_OSCCON &= (~0xf01);
  CRU_REGS->CRU_OSCCON |= 0x200;
  CRU_REGS->CRU_OSCCONSET = CRU_OSCCON_OSWEN_Msk;  /* request oscillator switch to occur */
  while(CRU_REGS->CRU_OSCCON & CRU_OSCCON_OSWEN_Msk);    /* wait for indication of successful clock change before proceeding */   
 
  // Step 4 : set bt_en_main_clk to 1 to enable BT main clock
  //BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG0 |= 0x00100000;
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG0 |= 0x00100010; //zigbee and BLE
  
  // Step 5 : set KEEP_ACLB_CLOCKS_ON to 1 via BT config register 
  // BLE_REGS->BLE_RFPWRMGMT |= 0x40;
  PHY_BleRfPwrMgmt(0x40 , true);
  
  // Step 6 : Set spi_ldo_en to 1 via ACLB config register to turn on MBS in RF
  reg16 = aclb_spi_read(0x5D);
  //reg16 &= (~0x400);
  reg16 |= 0x400;
  //RF_Write_Reg(0x5D, reg16);
  aclb_spi_write(0x5D,reg16);

  // Step 7 : Wait for MBS settling time. Settling time value is 35us
   //Y Delay(4);
  x_asm_delay(4);
  
  // Step 8 : Set KEEP_ACLB_CLOCKS_ON to 0 via BT config register 
 // BLE_REGS->BLE_RFPWRMGMT &= (~0x40);
  PHY_BleRfPwrMgmt(0x40 , false);

  // Step 9 : Assert external PLL power request
  CRU_REGS->CRU_SPLLCON &= ~(0x08);
     
  // Step 10 : Combining step 9, set EN_RFLDO_ovrd_en to 0 to enable CLKGEN LDO enable in RF 
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG0 &= (~0x8000);

  // Step 11 : Wait for LDO settling time. Settling time value is 10us
  //Delay(1);
  x_asm_delay(1);

 
  // Step 12 : Set die_BENXOANA_ovrd_en to 0 to switch CLDO to LDO_PLL 
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG0 &= (~0x800);
  
  // Step 13 : Set PLL_PWD to 0 to enable CLKGEN CLK enable in RF 
  BLE_REGS->BLE_DPLL_RG2 &= (~0x2);
  
  // Step 14 : Poll pll_lock_out_sync to wait until pll_lock_out_sync is set
  reg32 = BTZBSYS_REGS->BTZBSYS_SUBSYS_STATUS_REG1;
  while ((reg32 & 0x03) != 0x03)
  {
    reg32 = BTZBSYS_REGS->BTZBSYS_SUBSYS_STATUS_REG1;
  }

  // Step 15 : Restore REFOx registers programming for run mode
  CRU_REGS->CRU_REFO1CON = CRU_REFO1CON_ROSEL_SPLL1;
  
  /* Enable oscillator (ON bit) */
  CRU_REGS->CRU_REFO1CONSET = CRU_REFO1CON_ON_Msk;

  // Step 16 : Set PB1 CLK to SYS_CLK to restore its clock rate for run mode 
  CRU_REGS->CRU_PB1DIV = 0x8800; 

  // Step 17 : Restore PCHE Cache programming for run mode
  PCHE_REGS->PCHE_CHECON = 0x07000011;

  // Step 18 : Change CLK source in CRU from POSC CLK to SPLL1 CLK
  CRU_REGS->CRU_OSCCON &= (~0xf01);
  CRU_REGS->CRU_OSCCON |= 0x100;
  CRU_REGS->CRU_OSCCONSET = CRU_OSCCON_OSWEN_Msk;  /* request oscillator switch to occur */
  while(CRU_REGS->CRU_OSCCON & CRU_OSCCON_OSWEN_Msk);    /* wait for indication of successful clock change before proceeding */
  
  // Step 19 : Set subsys_clk_src_sel to 1 to change subsys clock source from XTAL CLK to PLL CLK
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG1 &= (~0x30);
  BTZBSYS_REGS->BTZBSYS_SUBSYS_CNTRL_REG1 |= 0x10;

  // remove sleep enable
  //not match to word! word should have CRU_OSCCON
  CRU_REGS->CRU_OSCCON &= (~0x10); 
}

/*******************************************************************************
  To Execute Sleep
*******************************************************************************/
void HAL_ExecuteSleep()
{
    // Step 29 : set deep sleep enable to 0
  DSCON_REGS->DSCON_DSCON &= ~(0x8000);
  //set sleep enable to 1
  CRU_REGS->CRU_OSCCON |= 0x10;	

  //Execute wait for Interrupt instruction
  __asm volatile( "wfi" );
}

/*******************************************************************************
  Enables Transceiver clock
*******************************************************************************/
void HAL_EnableTransceiver(void)
{
  // todo
}
// eof sleep.c

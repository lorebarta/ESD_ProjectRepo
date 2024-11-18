//#############################################################################
//
// FILE:   lpm_ex2_idlewake_watchdog.c
//
// TITLE:  Idle entry and Exit example.
//
//! \addtogroup driver_example_list
//! <h1>Low Power Modes: Device Idle Mode and Wakeup using Watchdog</h1>
//!
//!  This example puts the device into IDLE mode and then wakes up the device
//!  from IDLE using watchdog timer.
//!
//!  The device wakes up from the IDLE mode when the watchdog timer overflows,
//!  triggering an interrupt.
//!  A pre scalar is set for the watchdog timer to change the counter overflow
//!  time.
//!
//!  GPIO1 is pulled high before entering the IDLE mode and is pulled low when
//!  in the wakeup ISR.
//!
//! \b External \b Connections \n
//!  - On device wakeup, the GPIO1 will be low and LED1 will start blinking
//!
//
//#############################################################################
//
//
// $Copyright:
// Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Function Prototypes
//
void enterLPM();
__interrupt void wakeupISR(void);

//
// To avoid flash accesses, the function enterLPM and the ISR wakeupISR should
// be run from RAM.
// Note that the function SysCtl_enterIdleMode is defined inline. But this
// example is built with opt_level=off which does not inline functions. To avoid
// flash accesses, include SysCtl_enterIdleMode in .TI.ramfunc section (This is
// not required if optimization level 0 or higher is used)
//
#pragma CODE_SECTION(enterLPM, ".TI.ramfunc");
#pragma CODE_SECTION(wakeupISR, ".TI.ramfunc");
#pragma CODE_SECTION(SysCtl_enterIdleMode, ".TI.ramfunc");

//USER'S DEFINES
#define TIMER_ACTIVATION 2000

#define LEN 8
#define ID 0x01
#define OBJID 1

#define SUPPLY_MODE_LEN 1
#define SUPPLY_MODE_ID 0X02
#define SUPPLY_MODE_OBJID 2

#define FIRST_MSG_LEN 1
#define FIRST_MSG_ID 0X03
#define FIRST_MSG_OBJID 3

//VARIABLES
uint16_t ADC_enable;
uint16_t VINGD;
uint32_t timer;
uint16_t firstReconnection;
uint16_t firstConnection;
uint16_t isLPM;
uint16_t PT1000;  //
uint16_t HALL;  //Hall S
uint16_t NTC;  //NTC S
uint16_t SOC;  //SOC
uint16_t msg[LEN];
uint16_t CANTX_flag;
uint16_t firstMsg[1];
uint16_t supplyModeMsg[1];
uint16_t CHG;
uint16_t BUTTON;

//User's defined functions
void ADC_conversion(uint16_t* a, uint16_t* b, uint16_t* c, uint16_t* d);
void ADC2CAN(uint16_t res, uint16_t* firstHalf, uint16_t* secondHalf);
//
// Main
//
void main(void)
{
	BUTTON = 0;
	CHG = 0;
    isLPM = 0;
    ADC_enable = 0;
    VINGD = 1;
    timer = 0;
    firstReconnection = 0;
    firstConnection = 1;
    PT1000 = 0;
    HALL = 0;
    NTC = 0;
    SOC = 0;
    CANTX_flag = 1;
    //CAN standard message init
    msg[0] = 0x00;
    msg[1] = 0x00;
    msg[2] = 0x00;
    msg[3] = 0x00;
    msg[4] = 0x00;
    msg[5] = 0x00;
    msg[6] = 0x00;
    msg[7] = 0x00;
    //CAN first transmission init
    firstMsg[0] = 0xFF;
    //CAN log message
    supplyModeMsg[0] = 0x00;
    //
    // Configure PLL, disable WD, enable peripheral clocks.
    //
    Device_init();
    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    Board_init();

    //LED1
    GPIO_setPinConfig(DEVICE_GPIO_CFG_LED1);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);
    //LED2
    GPIO_writePin(DEVICE_GPIO_PIN_LED2,1);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED2,GPIO_PIN_TYPE_STD);
    //GPIO_setQualificationMode(DEVICE_GPIO_CFG_LED2, GPIO_QUAL_SYNC);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED2, GPIO_DIR_MODE_OUT);

    //VINGD
    GPIO_setPinConfig(15);
    GPIO_setPadConfig(15, GPIO_PIN_TYPE_PULLUP);
    GPIO_setDirectionMode(15, GPIO_DIR_MODE_IN);
    //CHG
    GPIO_setPinConfig(56);
    GPIO_setPadConfig(56, GPIO_PIN_TYPE_PULLUP);
    GPIO_setDirectionMode(56, GPIO_DIR_MODE_IN);
    //BUTTON
    GPIO_setPinConfig(58);
    GPIO_setPadConfig(58, GPIO_PIN_TYPE_PULLUP);
    GPIO_setDirectionMode(58, GPIO_DIR_MODE_IN);
    //LED
    GPIO_writePin(DEVICE_GPIO_PIN_LED2,0);
    GPIO_writePin(DEVICE_GPIO_PIN_LED1,1);
    //
    // Disable global interrupts.
    //
    DINT;
    //
    // Initialize interrupt controller and vector table.
    //
    Interrupt_initModule();
    Interrupt_initVectorTable();
    IER = 0x0000;
    IFR = 0x0000;
    //
    // Re-map watchdog wake interrupt signal to call the ISR function in this
    // example
    //
    Interrupt_register(INT_WAKE, &wakeupISR);
    //
    // Set the watchdog to generate an interrupt signal instead of a
    // reset signal
    //
    SysCtl_setWatchdogMode(SYSCTL_WD_MODE_INTERRUPT);
    SysCtl_setWatchdogPredivider(SYSCTL_WD_PREDIV_64);
    //SysCtl_setWatchdogPrescaler(SYSCTL_WD_PRESCALE_2);
    //
    // Enable the wake interrupt in the PIE: Group 1 interrupt 8.
    //
    Interrupt_enable(INT_WAKE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    //
    // Enable global interrupts.
    //
    EINT;
    //
    // Reset the watchdog counter
    //
    SysCtl_serviceWatchdog();

    //
    // Enable the watchdog
    //
    SysCtl_enableWatchdog();
    //
    //CAN configuration
    //
    CAN_setBitRate(can0_BASE, DEVICE_SYSCLK_FREQ, 1000000,20);
    CAN_startModule(can0_BASE);
    CAN_enableController(can0_BASE);
    //
    //Infinite Loop
    //
    while(1)
    {
        VINGD = GPIO_readPin(15);
        CHG = GPIO_readPin(56);
        BUTTON = GPIO_readPin(58);
        if(ADC_enable == 1){
            ADC_conversion(&PT1000, &HALL, &NTC, &SOC);
            //segmentation for CAN
            ADC2CAN(PT1000, &msg[0],&msg[1]); //
            ADC2CAN(HALL, &msg[2],&msg[3]); //Hall
            msg[4] = 0x00;
            msg[5] = 0x00;
            msg[6] = 0x00;
            msg[7] = 0x00;
            //digital sensor
            if(BUTTON == 1)
            	msg[4] = 0x01;
            else
            	msg[4] = 0x00;
            //CHG & SOC
            //Not in charge
            if(CHG == 1)
        	   msg[5] = 0x10;
            //In charge
            else if (CHG == 0)
        	   msg[5] = 0x00;
            //Low volt
            if(SOC < 1000)
        	   msg[5] = msg[5] + 0x01;
            //Normal volt
            else
            	msg[5] = msg[5] + 0x00;
            //NTC
            if(NTC > 3800)
            	msg[6] = 0x01;
            //Errors in ADC
            if(PT1000 < 100)
            	msg[7] = 0x10;
            else
            	msg[7] = 0x00;
            if(HALL < 100)
            	msg[7] = msg[7] + 0x01;
            else
            	msg[7] = msg[7] + 0x00;

            CAN_setupMessageObject(can0_BASE, OBJID, ID,
                                               CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX, 0,
                                               CAN_MSG_OBJ_TX_INT_ENABLE, LEN);
            CAN_sendMessage(CANA_BASE, 0x01,LEN,msg);
            ADC_enable = 0;
            if(0 == VINGD){
                //Auxiliary battery ON
                timer++;
                firstReconnection = 1;
                if(firstConnection==1){
                	supplyModeMsg[0] = 0xFF;
                	CAN_setupMessageObject(can0_BASE, SUPPLY_MODE_OBJID, SUPPLY_MODE_ID,
                	                                               CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX, 0,
                	                                               CAN_MSG_OBJ_TX_INT_ENABLE, SUPPLY_MODE_LEN);
                	CAN_sendMessage(CANA_BASE,SUPPLY_MODE_ID, SUPPLY_MODE_LEN,supplyModeMsg);
                    //LOG
                    firstConnection = 0;
                }
                if(timer > TIMER_ACTIVATION){
                    enterLPM();
                    GPIO_writePin(DEVICE_GPIO_PIN_LED2,1);
                }

            }
            else{
                //Main Supply ON
                if(firstReconnection == 1){
                	supplyModeMsg[0] = 0x00;
                	CAN_setupMessageObject(can0_BASE, SUPPLY_MODE_OBJID, SUPPLY_MODE_ID, CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX, 0,
                	                	                                               CAN_MSG_OBJ_TX_INT_ENABLE, SUPPLY_MODE_LEN);
                	CAN_sendMessage(CANA_BASE,SUPPLY_MODE_ID, SUPPLY_MODE_LEN,supplyModeMsg);
                    //LOG
                    firstReconnection = 0;
                    GPIO_writePin(DEVICE_GPIO_PIN_LED2,0);
                }
                timer = 0;
                firstConnection = 1;
            }

        }
        else{
        	if(CANTX_flag == 1){
        		CAN_setupMessageObject(can0_BASE, FIRST_MSG_OBJID, FIRST_MSG_ID, CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX, 0,
        		                	        		                                   CAN_MSG_OBJ_TX_INT_ENABLE, FIRST_MSG_LEN);
        		CAN_sendMessage(CANA_BASE, FIRST_MSG_ID, FIRST_MSG_LEN, firstMsg);
        		CANTX_flag = 0;
        	}
        }
    }
}

//
// enterLPM() - This will power down the flash and enter LPM.
//
void enterLPM()
{
    isLPM = 1;
    //
    // Power down the flash bank and pump. Ensure there are no subsequent
    // flash accesses.
    //
    Flash_powerDown(FLASH0CTRL_BASE);

    //
    // Enter idle mode.
    //
    SysCtl_enterIdleMode();
}

//
// Wakeup ISR - The interrupt service routine called when the watchdog
//              triggers the wake interrupt signal
//
__interrupt void
wakeupISR(void)
{
    ADC_enable = 1;
    //
    // Wake the flash from low power mode.
    //
    Flash_wakeFromLPM(FLASH0CTRL_BASE);

    //
    // Set GPIO1 low.
    //
    GPIO_writePin(1, 0);


    //
    // Acknowledge the interrupt
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

void ADC_conversion(uint16_t* a, uint16_t* b, uint16_t* c, uint16_t* d){
    GPIO_writePin(0U, 1U); // Set pin
    GPIO_writePin(0U, 0U); // Clear pin
    while(ADC_getInterruptStatus(ADC0_BASE, ADC_INT_NUMBER1) == 0U);
    ADC_clearInterruptStatus(ADC0_BASE, ADC_INT_NUMBER1);
    *a = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
    *b = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);
    *c = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER2);
    *d = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER3);
}

void ADC2CAN(uint16_t res, uint16_t* firstHalf, uint16_t* secondHalf){
    *firstHalf = (res & 0xFF00) >> 8;
    *secondHalf = res & 0x00FF;
}

//
// End of File
//

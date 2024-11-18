/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BOARD_H
#define BOARD_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//

#include "driverlib.h"
#include "device.h"

//*****************************************************************************
//
// PinMux Configurations
//
//*****************************************************************************

//
// ANALOG -> myANALOGPinMux0 Pinmux
//

//
// CANA -> can0 Pinmux
//
//
// CANA_RX - GPIO Settings
//
#define GPIO_PIN_CANA_RX 5
#define can0_CANRX_GPIO 5
#define can0_CANRX_PIN_CONFIG GPIO_5_CANA_RX
//
// CANA_TX - GPIO Settings
//
#define GPIO_PIN_CANA_TX 4
#define can0_CANTX_GPIO 4
#define can0_CANTX_PIN_CONFIG GPIO_4_CANA_TX
//
// GPIO0 - GPIO Settings
//
#define myGPIO0_GPIO_PIN_CONFIG GPIO_0_GPIO0

//*****************************************************************************
//
// ADC Configurations
//
//*****************************************************************************
#define ADC0_BASE ADCA_BASE
#define ADC0_RESULT_BASE ADCARESULT_BASE
#define ADC0_SOC0 ADC_SOC_NUMBER0
#define ADC0_FORCE_SOC0 ADC_FORCE_SOC0
#define ADC0_SAMPLE_WINDOW_SOC0 75
#define ADC0_TRIGGER_SOURCE_SOC0 ADC_TRIGGER_GPIO
#define ADC0_CHANNEL_SOC0 ADC_CH_ADCIN2
#define ADC0_SOC1 ADC_SOC_NUMBER1
#define ADC0_FORCE_SOC1 ADC_FORCE_SOC1
#define ADC0_SAMPLE_WINDOW_SOC1 75
#define ADC0_TRIGGER_SOURCE_SOC1 ADC_TRIGGER_GPIO
#define ADC0_CHANNEL_SOC1 ADC_CH_ADCIN3
#define ADC0_SOC2 ADC_SOC_NUMBER2
#define ADC0_FORCE_SOC2 ADC_FORCE_SOC2
#define ADC0_SAMPLE_WINDOW_SOC2 75
#define ADC0_TRIGGER_SOURCE_SOC2 ADC_TRIGGER_GPIO
#define ADC0_CHANNEL_SOC2 ADC_CH_ADCIN14
#define ADC0_SOC3 ADC_SOC_NUMBER3
#define ADC0_FORCE_SOC3 ADC_FORCE_SOC3
#define ADC0_SAMPLE_WINDOW_SOC3 75
#define ADC0_TRIGGER_SOURCE_SOC3 ADC_TRIGGER_GPIO
#define ADC0_CHANNEL_SOC3 ADC_CH_ADCIN11
void ADC0_init();


//*****************************************************************************
//
// ASYSCTL Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// CAN Configurations
//
//*****************************************************************************
#define can0_BASE CANA_BASE

void can0_init();


//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define myGPIO0 0
void myGPIO0_init();

//*****************************************************************************
//
// INPUTXBAR Configurations
//
//*****************************************************************************
#define myINPUTXBARINPUT0_SOURCE 0
#define myINPUTXBARINPUT0_INPUT XBAR_INPUT5
void myINPUTXBARINPUT0_init();

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	ADC_init();
void	ASYSCTL_init();
void	CAN_init();
void	GPIO_init();
void	INPUTXBAR_init();
void	PinMux_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of BOARD_H definition

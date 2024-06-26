/*******************************************************************************
  Input Capture (ICAP1) Peripheral Library (PLIB)

  Company:
    Microchip Technology Inc.

  File Name:
    plib_icap1.c

  Summary:
    ICAP1 Source File

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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
#include "plib_icap1.h"
#include "interrupts.h"

volatile static ICAP_OBJECT icap1Obj;
// *****************************************************************************

// *****************************************************************************
// Section: ICAP1 Implementation
// *****************************************************************************
// *****************************************************************************


void ICAP1_Initialize (void)
{
    /*Setup IC1CON    */
    /*ICM     = 3        */
    /*ICI     = 0        */
    /*ICTMR = 0*/
    /*C32     = 0        */
    /*FEDGE = 0        */
    /*SIDL     = false    */

    IC1CON = 0x3;


        IEC0SET = _IEC0_IC1IE_MASK;
}


void ICAP1_Enable (void)
{
    IC1CONSET = _IC1CON_ON_MASK;
}


void ICAP1_Disable (void)
{
    IC1CONCLR = _IC1CON_ON_MASK;
}

uint16_t ICAP1_CaptureBufferRead (void)
{
    return (uint16_t)IC1BUF;
}



void ICAP1_CallbackRegister(ICAP_CALLBACK callback, uintptr_t context)
{
    icap1Obj.callback = callback;
    icap1Obj.context = context;
}

void __attribute__((used)) INPUT_CAPTURE_1_InterruptHandler(void)
{
    uintptr_t context = icap1Obj.context; 
    if( (icap1Obj.callback != NULL))
    {
        icap1Obj.callback(context);
    }
    IFS0CLR = _IFS0_IC1IF_MASK;    //Clear IRQ flag

}


bool ICAP1_ErrorStatusGet (void)
{
    bool status = false;
    status = (((IC1CON >> ICAP_STATUS_OVERFLOW) & 0x1U) != 0U);
    return status;
}

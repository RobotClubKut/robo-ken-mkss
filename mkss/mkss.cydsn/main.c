/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>
#include "PS2_Controller.h"
#include "ICS3_5.h"

int main()
{
    int16 a;
    PS2Controller psData;
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_Servo_Start();
    PS2_Start();
    UART_Debug_Start();
    CyDelay(1000);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    for(;;)
    {
        /* Place your application code here. */
        psData = PS2_Controller_get();
        Pos_Set(0,a);
        Pos_Set(1,a);
        Pos_Set(2,a);
        Pos_Set(3,a);
        Pos_Set(4,a);
        CyDelay(1000);
        a+=10;
    }
}

/* [] END OF FILE */

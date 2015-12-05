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

uint8 g_timerFlag;
CY_ISR(timer_isr)
{    
    g_timerFlag = 1;    
}

void kamae(void);
void karimen(void);

int main()
{
    uint8 circleFlag = 1, flag = 1;
    int16 kakudo=0;
//    int16 value,a;
    char buffer[100];
    PS2Controller psData;
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_mkss_StartEx(timer_isr);
    UART_Servo_Start();
    PS2_Start();
    //UART_Debug_Start();
    CyDelay(1000);
    while(!PS2_Analog_Flag());
    while(!PS2_Controller_get().START);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    for(;;)
    {
        /* Place your application code here. */
        if(g_timerFlag == 1){
            
            psData = PS2_Controller_get();
            
            if(psData.CIRCLE)
            {
                if(circleFlag)
                {
                    if(flag){
                        flag = 0;
                    }
                    else
                    {
                        flag = 1;
                    }
                    circleFlag = 0;
                }
            }
            else
            {
                circleFlag = 1;
            }
            if(flag){
                Pos_Set(4,0); 
            }
            else{
                Pos_Set(4,30);
            }
                
            //sprintf(buffer,"%d\n", (int)Free(0));
            //UART_Debug_PutString(buffer);
            g_timerFlag = 0;
        }
    }
}

void kamae(void){
    Pos_Set(0,0);
    Pos_Set(1,30);
    Pos_Set(2,-20);
    Pos_Set(3,-10);
    Pos_Set(4,0);
}

void karimen(void){
    Pos_Set(0,0);
    Pos_Set(1,-30);
    Pos_Set(2,-20);
    Pos_Set(3,-10);
    Pos_Set(4,-30);
}

/* [] END OF FILE */

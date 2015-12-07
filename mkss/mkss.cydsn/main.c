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
CY_ISR(add_isr)
{    
    g_timerFlag = 1;    
}

void kamae(void);
void karimen(void);
void move(uint8 ID);

int main()
{
    uint8 circleFlag = 1, status = 1;
    int16 kakudo=0,count=0;
    char buffer[100];
    PS2Controller psData;
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_mkss_StartEx(add_isr);
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
        /*---10msごとにフラグ---*/
        if(g_timerFlag == 1)
        {
            ////実験            
            count++;
            Pos_Set(4,count*count*0.2);
            if(count==20){
                return 0;
            }
            /////
            psData = PS2_Controller_get();
            
            /*---コントローラー処理---*/
            if(psData.CIRCLE)
            {
                if(circleFlag)
                {
                    if(status){
                        status = !status;
                    }
                    circleFlag = 0;
                }
            }
            else
            {
                circleFlag = 1;
            }
            
            /*---状態変化---*/
            if(status){
                Pos_Set(4,0);
                //status=Pos_Set(4,0);みたいな感じにしたい
            }
            else{
                move(4);
            }
                
            /*---デバッグ---*/
            //sprintf(buffer,"%d\n", (int)Free(0));
            //UART_Debug_PutString(buffer);
            g_timerFlag = 0;
        }
    }
}
void move (uint8 ID){            
    static uint16 count = 0;
    static uint16 step = 0;
    if(step==0){
        Pos_Set(ID,count*0.2);
        if(count*0.1==55){
            step++;
        }
    }
    else if(step==1){
        Pos_Set(ID,60);
        count=0;
    }
    count++;
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

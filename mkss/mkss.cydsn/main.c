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

#define MODE_TYUDAN 0
#define MODE_MEN    1
#define MODE_JODAN  2

uint8 g_timerFlag;
CY_ISR(clock_isr)
{    
    g_timerFlag = 1;
}

void jodan(void);
void tyudan(void);
void karimen(void);
void move(uint8 ID);
void antei(void);
void karimen2(void);

int main()
{
    uint8 menflag = 0;
    uint8 triangleflg = 1;
    uint8 circleFlag = 1, status = 0;
    uint8 kensei = 1;
    int16 kakudo=0,count=0;
    char buffer[100];
    PS2Controller psData;

    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_mkss_StartEx(clock_isr);
    UART_Servo_Start();
    PS2_Start();
    UART_Debug_Start();
    CyDelay(1000);
    while(!PS2_Analog_Flag());
    while(!PS2_Controller_get().START);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    for(;;)
    {
        /* Place your application code here. */
        /*---10msごとにフラグ---*/
        
            //sprintf(buffer,"ID0:%d ID1:%d ID:2%d ID3:%d ID4:%d\n", (int)Free(0), (int)Free(1), (int)Free(2), (int)Free(3), (int)Free(4));
            //UART_Debug_PutString(buffer);

        //tyudan();
        if(g_timerFlag == 1)
        {
            ////実験
//
//            count++;
//            Pos_Set(4,count*count*0.2);
//            if(count==20){
//                return 0;
//            } 
            /////
            psData = PS2_Controller_get();
            
            /*---コントローラー処理---*/
            if(psData.CIRCLE)
            {
                if(circleFlag)
                {
                    if(status){
                        status = MODE_TYUDAN;
                    }
                    else
                    {
                        status = MODE_JODAN;
                    }
                    circleFlag = 0;
                }
            }
            else
            {
                circleFlag = 1;
            }
            
            if(psData.TRIANGLE)
            {
                if(triangleflg)
                {
                    if(status==MODE_TYUDAN)
                    {
                        status = MODE_MEN;
                    }
                    triangleflg = 0;
                }
            }
            else
            {
                triangleflg = 1;
            }
            
            /*---状態変化---*/            
            if(status==MODE_TYUDAN)
            {
                tyudan();
                menflag=0;
            }
            else if(status==MODE_MEN){
                if(menflag==0){
                    karimen();
                    menflag=1;
                    CyDelay(30);
                }
                else{
                    karimen2();
                    CyDelay(300);
                    jodan();
                    CyDelay(600);
                    status=MODE_TYUDAN;
                }
                //move(4);
            }
            else if(status==MODE_JODAN)
            {
                jodan();
            }
            
            /*---デバッグ---*/
            //sprintf(buffer,"%d %d %d %d %d\n", (int)Free(0), (int)Free(1), (int)Free(2), (int)Free(3), (int)Free(4));
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

void jodan(void){
    Pos_Set(0,-70);
    Pos_Set(1,5);
    Pos_Set(2,-45);
    Pos_Set(3,5);
    Pos_Set(4,0);
}

void karimen(void){
    Pos_Set(0,-70);
    Pos_Set(1,-45);
    Pos_Set(2,-90);
    Pos_Set(3,5);
    Pos_Set(4,45);
}
void karimen2(void){
    Pos_Set(0,-70);
    Pos_Set(1,-80);
    Pos_Set(2,0);
    Pos_Set(3,5);
    Pos_Set(4,-15);
}

void antei(void){
    Pos_Set(0,-70);
    Pos_Set(1,-10);
    Pos_Set(2,4);
    Pos_Set(3,0);
    Pos_Set(4,-35);
}
void tyudan(void){
    Pos_Set(0,-70);
    Pos_Set(1,5);
    Pos_Set(2,-110);
    Pos_Set(3,5);
    Pos_Set(4,0);
}


/* [] END OF FILE */

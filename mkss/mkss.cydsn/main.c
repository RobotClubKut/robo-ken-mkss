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

#define MODE_TYUDAN     0
#define MODE_MEN        1
#define MODE_JODAN      2
#define MODE_KAIHI      3
#define MODE_NOBI       4
#define MODE_KAITEN     5
#define MODE_JODAN2     6
#define MODE_KAIHI2     7
#define MODE_NOBI2      8
#define MODE_KAITEN2    9 
#define MODE_DOU        10
#define MODE_MEN2       11
#define MODE_MEN3       12
#define MODE_DOU2       13
#define MODE_DOU3       14
#define MODE_LEFIT_MEN  15
#define MODE_RIGHT_MEN  16
#define MODE_SAGE       17
#define MODE_AGE        18
#define MODE_NIGE       19  
#define MODE_FREE       20


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
void dou(void);
void dou2(void);
void dou3(void);
void men(void);


int main()
{
    uint8 menflag = 0;
    uint8 kaihiFlag = 0;
    uint8 triangleflg = 1;
    uint8 circleFlag = 1;
    uint8 status = 0;
    uint8 squareFlag = 1;
    uint8 crossFlag = 1;
    uint8 l1Flag = 1;
    uint8 l2Flag = 1;
    uint8 l3Flag = 1;
    uint8 r1Flag = 1;
    uint8 r2Flag = 1;
    uint8 upFlag = 1;
    uint8 downFlag = 1;
    uint8 selectFlag = 1;
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
            //sprintf(buffer,"ID0:%d ID1:%d ID2:%d ID3:%d ID4:%d\n", (int)Free(0), (int)Free(1), (int)Free(2), (int)Free(3), (int)Free(4));
            //UART_Debug_PutString(buffer);
        //Pos_Set(0,-70);
        //antei();
        
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
            if(psData.L2)//上段と中段
            {
                if(l2Flag)
                {
                    if(status==MODE_JODAN){
                        status = MODE_TYUDAN;
                    }
                    else if(status==MODE_TYUDAN)
                    {
                        status = MODE_JODAN;
                    }
                    else if(status==MODE_JODAN2)//上段2から中段へ
                    {
                        status = MODE_TYUDAN;
                    }
                    l2Flag = 0;
                }
            }
            else
            {
                l2Flag = 1;
            }
            
            if(psData.TRIANGLE)//面のラストの動き
            {
                if(triangleflg)
                {
                    if((status==MODE_MEN)||(status==MODE_MEN2))
                    {
                        status = MODE_MEN3;
                    }
                    else if(status==MODE_AGE)
                    {
                        status = MODE_MEN3;
                    }
                    triangleflg = 0;
                }
            }
            else
            {
                triangleflg = 1;
            }
            if(psData.L1)//回避と回転
            {
                if(l1Flag)
                {
                    if(status==MODE_JODAN)
                    {
                        status = MODE_KAIHI;
                    }
                    else if(status==MODE_KAIHI)
                    {
                        status = MODE_JODAN;
                    }
                    else if(status==MODE_JODAN2)
                    {
                        status = MODE_KAIHI2;
                    }
                    l1Flag = 0;
                }
            }
            else
            {
                l1Flag = 1;
            }
            
            if(psData.R1)//面の最後の動き&伸びからの回転
            {
                if(r1Flag)
                {
                    if((status==MODE_TYUDAN)||(status==MODE_JODAN))//面（ガチ）
                    {
                        status = MODE_MEN;
                    }
                    else if(status==MODE_NOBI)
                    {
                        status = MODE_KAITEN;
                    }
                    else if(status==MODE_NOBI2)
                    {
                        status = MODE_KAITEN2;
                    }
     
                    r1Flag = 0;
                }
            }
            else//離したら戻る
            {
                r1Flag = 1;
                if(status==MODE_MEN)
                {
                    status=MODE_TYUDAN;
                }
                if(status==MODE_MEN2)
                {
                    status=MODE_JODAN;
                }
            }
            if(psData.R2)//回転して体を起こす
            {
                if(r2Flag)
                {
                    if(status==MODE_TYUDAN)//胴
                    {
                        status = MODE_DOU;
                    }
                    else if(status==MODE_KAITEN)
                    {
                        status = MODE_JODAN2;
                    }
                    r2Flag = 0;
                }
            }
            else
            {
                r2Flag = 1;
                if(status==MODE_DOU)
                {
                    status=MODE_TYUDAN;
                }
            }

            if(psData.SQUARE)//胴と左右面
            {
                if(squareFlag)
                {
                    if(status==MODE_DOU)//胴を振り切る
                    {
                        status = MODE_DOU2;
                    }
                    else if(status==MODE_TYUDAN)
                    {
                        status = MODE_DOU3;
                    }
                    else if(status==MODE_MEN)
                    {
                        status = MODE_LEFIT_MEN;
                    }
                    else if(status==MODE_AGE)
                    {
                        status = MODE_LEFIT_MEN;
                    }
                    
                    squareFlag = 0;
                }   
            }
            else
            {
                squareFlag = 1;
            }  
            if(psData.CIRCLE)
            {
                if(circleFlag)
                {
                    if(status==MODE_MEN)
                    {
                        status = MODE_RIGHT_MEN;
                    }
                    else if(status==MODE_AGE)
                    {
                        status = MODE_RIGHT_MEN;
                    }
                    circleFlag = 0;
                }   
            }
            else
            {
                circleFlag = 1;
            }
            
            if(psData.DOWN)//伸び
            {
                if(downFlag)
                {
                    if(status==MODE_KAIHI)
                    {
                        status = MODE_NOBI;
                    }
                    else if(status==MODE_KAIHI2)
                    {
                        status = MODE_NOBI2;
                    }
                    else if(status==MODE_TYUDAN)
                    {
                        status = MODE_SAGE;
                    }
                    downFlag = 0;
                }   
            }
            else
            {
                downFlag = 1;
                if(status==MODE_SAGE)
                {
                    status = MODE_TYUDAN;
                }
            }
            if(psData.L3)//回転
            {
                if(l3Flag)
                {
                    if(status==MODE_NOBI)
                    {
                        status = MODE_KAITEN;
                    }
                    
                    else if(status==MODE_NOBI2)
                    {
                        status = MODE_KAITEN2;
                    }
                    l3Flag = 0;
                }   
            }
            else
            {
                l3Flag = 1;
            }
            if(psData.UP)//回転
            {
                if(upFlag)
                {
                    if(status==MODE_TYUDAN)
                    {
                        status = MODE_AGE;
                    }
                    upFlag = 0;
                }   
            }
            else
            {
                upFlag = 1;
                if(status==MODE_AGE)
                {
                    status = MODE_TYUDAN;
                }
            }
            if(psData.CROSS)
            {
                if(crossFlag)
                {
                    if(status==MODE_TYUDAN)
                    {
                        status = MODE_NIGE;
                    }

                    circleFlag = 0;
                }   
            }
            else
            {
                circleFlag = 1;  
                if(status==MODE_NIGE)
                {
                    status = MODE_TYUDAN;
                    
                }
            }
            
            if(psData.SELECT)
            {
                for(;;){
                    
                    Free(0);
                    Free(1);
                    Free(2);
                    Free(3);
                    Free(4);
                }
            }
            
            /*---状態変化---*/            
            if(status==MODE_TYUDAN)
            {
                tyudan();
                menflag=0;
            }
            else if(status==MODE_MEN)
            {
                karimen();
            }
            else if(status==MODE_MEN3)
            {
                karimen2();
                CyDelay(300);
                jodan();
                CyDelay(800);
                status=MODE_TYUDAN;
            }
            else if(status==MODE_JODAN)
            {
                jodan();
                if(kaihiFlag==1)
                {
                    CyDelay(50);
                    speed(1,127);
                    kaihiFlag=0;
                }
            }
            else if(status==MODE_KAIHI)
            {
                kaihiFlag=1;
                speed(1,50);
                Pos_Set(1,81);
            }
            else if(status==MODE_NOBI)
            {
                Pos_Set(2,4);
                Pos_Set(3,0);
                Pos_Set(4,-80);//-60
            }
            else if(status==MODE_KAITEN)
            {
                speed(0,100);
                Pos_Set(0,135);
                CyDelay(1000);
                status=MODE_JODAN2;
            }
            else if(status==MODE_JODAN2)
            {
                speed(1,40);
                Pos_Set(1,5);
                Pos_Set(2,-45);
                Pos_Set(3,5);
                Pos_Set(4,0);
                CyDelay(200);
                speed(1,127);
            }
            else if(status==MODE_KAIHI2)
            {
                speed(1,30);
                Pos_Set(1,-102);
            }
            else if(status==MODE_NOBI2)
            {
                Pos_Set(2,0);
                Pos_Set(3,0);
                Pos_Set(4,50);
            }
            else if(status==MODE_KAITEN2)
            {
                Pos_Set(0,-70);
                CyDelay(1000);
                status=MODE_TYUDAN;
            }
            else if(status==MODE_DOU)
            {
                dou();
            }
            else if(status==MODE_DOU2)
            {
                dou2();
                CyDelay(100);
                dou3();
                CyDelay(600);
                status = MODE_TYUDAN;
            }
            else if(status==MODE_MEN2)//上段からの面
            {
                men();
            }
            else if(status==MODE_DOU3)
            {
                dou2();
                CyDelay(100);
                dou3();
                CyDelay(700);
                status = MODE_TYUDAN;
            }
            else if(status==MODE_LEFIT_MEN)
            { 
                Pos_Set(0,-70);
                Pos_Set(1,-60);
                Pos_Set(2,0);
                Pos_Set(3,-35);
                Pos_Set(4,-15);
                CyDelay(400);
                jodan();
                CyDelay(800);
                status=MODE_TYUDAN;
            }
            else if(status==MODE_RIGHT_MEN)
            {             
                Pos_Set(0,-70);
                Pos_Set(1,-60);
                Pos_Set(2,0);
                Pos_Set(3,35);
                Pos_Set(4,-15);
                CyDelay(400);
                jodan();
                CyDelay(800);
                status=MODE_TYUDAN;
            }
            else if(status==MODE_SAGE)
            {
                Pos_Set(4,-45);
            }
            else if(status==MODE_AGE)
            {
                Pos_Set(4,80);
            }
            else if(status==MODE_NIGE)
            {
                //speed(1,50);
                Pos_Set(0,-75);
                Pos_Set(1,81);
                Pos_Set(2,-100);
                Pos_Set(3,6);
                Pos_Set(4,95);
            }
            else if(status==MODE_FREE)
            {
                Free(0);
                Free(1);
                Free(2);
                Free(3);
                Free(4);
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
    Pos_Set(1,-60);
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
    Pos_Set(2,-100);
    Pos_Set(3,5);
    Pos_Set(4,0);
}
void dou(void){
    Pos_Set(0,-45);
    Pos_Set(2,-70);
    Pos_Set(3,55);
    Pos_Set(4,70);
}
void dou2(void){
    Pos_Set(0,-70);
    Pos_Set(1,-30);
    Pos_Set(2,-45);
    Pos_Set(3,-35);
    Pos_Set(4,45);
}
void dou3(void){
    Pos_Set(0,-70);
    Pos_Set(1,-75);
    Pos_Set(2,0);//変更有り
    Pos_Set(3,-45);
    Pos_Set(4,-40);
}
void men(void){
    Pos_Set(0,-70);
    Pos_Set(1,-45);
    Pos_Set(2,-45);
    Pos_Set(3,5);
    Pos_Set(4,45);
}

/* [] END OF FILE */

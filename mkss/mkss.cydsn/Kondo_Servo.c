#include "ICS3_5.h"

int16 Free(uint8 ID)
{
    int16 kakudo,Data,pos_h,pos_l,pos;
    uint8 i, tx[3], rx[6];
    char buffer[40];
    //ポジション
    tx[0]=0x80|ID;
    tx[1]=0x00;
    tx[2]=0x00;
    //送信
    UART_Servo_PutArray(tx,3);
    CyDelay(1);
    //受信
    for (i = 0;i < 6;i++){
        rx[i] = UART_Servo_GetChar(); // モーターからの返値を受け取り、rxに代入する
    }
    pos_h=((int)rx[4]<<7)&0x3f80; 
    pos_l=(int)rx[5]&0x7f;
    pos=pos_h|pos_l; //値に変換
    pos=(7500-pos)*0.0333333; //角度に変換
    return pos;
}

void Pos_Set(uint8 ID, int16 kakudo){
    //int pos_h,pos_l,pos;
    int16 Data;
    uint8 i, tx[3], rx[6];
    Data = 7500 -( 30 * kakudo );
    tx[0]=0x80|ID;
    tx[1]=(unsigned char)(Data>>7) & 0x7F;
    tx[2]=(unsigned char)Data & 0x7F;
    UART_Servo_PutArray(tx,3);
}

void ID_Read()
{       
    int i,pos_h,pos_l,pos,kakudo,Data;
    uint8 tx[4], rx[6];
    char buffer[40];
    //ID読み込み
    tx[0]=0xFF;
    tx[1]=0x00;
    tx[2]=0x00;
    tx[3]=0x00;

    //送信
    for (i = 0;i < 4;i++){
        UART_Servo_PutChar(tx[i]); // コマンドを１バイトずつ送信する
    }
    //受信
    for (i = 0;i < 5;i++){
        rx[i] = UART_Servo_GetChar(); // モーターからの返値を受け取り、rxに代入する
    }
    sprintf(buffer,"%x,%x,%x,%x,%x\n",(int)rx[0], (int)rx[1], (int)rx[2], (int)rx[3], (int)rx[4]);
}


void ID_Write (uint8 ID)
{
    int pos_h,pos_l,pos,kakudo,Data;
    uint8 i,tx[4], rx[6];
    char buffer[40];
    //ID書き込み
    tx[0]=0xE0|ID;
    tx[1]=0x01;
    tx[2]=0x01;
    tx[3]=0x01;
            
    //送信
    for (i = 0;i < 4;i++){
        UART_Servo_PutChar(tx[i]); // コマンドを１バイトずつ送信する
    }
    //受信
    for (i = 0;i < 5;i++){
        rx[i] = UART_Servo_GetChar(); // モーターからの返値を受け取り、rxに代入する
    }       
    CyDelay(1000);
}

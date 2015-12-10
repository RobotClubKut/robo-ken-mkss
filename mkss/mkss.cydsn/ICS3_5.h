/*
ICS3.5
UART_Servoでモジュールを作る
*/
#ifndef ICS3_5_H
#define ICS3_5_H

#include <stdio.h>
#include <project.h>

int16 Free(uint8 ID);
void speed(uint8 ID, uint8 speed);
void Pos_Set(uint8 ID, int16 kakudo);
void ID_Read();
void ID_Write(uint8 ID);

#endif
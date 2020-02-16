/*
    File: ir_com.h
    Authors: Ryan Sadler, Ronan Fraser
    Desc: Infra-red communication module, sending and recieving information.
*/

#ifndef IR_COM_H
#define IR_COM_H

#include "ir_uart.h"
#include "system.h"
#include "pio.h"


/* Send a projectile to the other board */
void sendpoint(int* infoArray);


/* Recieve a projectile from the other board */
bool getpoint(int* infoArray);


#endif

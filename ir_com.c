/*
    File: ir_com.c
    Authors: Ryan Sadler, Ronan Fraser
    Desc: Infra-red communication module, sending and recieving information.

*/

#include "system.h"
#include "ir_uart.h"
#include "ir_com.h"
#include "pio.h"

#define COLUMN 0

/* Taking the given position and direction before transfer and
     * finding the position and direction after transfer, then sending
     * that information as a character */

void sendpoint(int* proj)
{
    int proj_loc = 0;
    int position = 0;
    int direction = 0;
    char outchar = '0';

    while (!(proj[0] & (1 << proj_loc))) {
        proj_loc++;
    }
    position = 7 - proj_loc; // Location of projectile on screen
    direction = proj[2]; // Direction of projectile

    outchar = direction * 10 + position; // Making character to send

    ir_uart_putc(outchar); // Sending character

}


bool getpoint(int* proj) // Retrieve the data of a projectile fired by the opponent
{
    int map = 0x01;
    bool proj_exists = false;
    int inchar;
    char received;

    proj[1] = COLUMN;


    if (ir_uart_read_ready_p()) { // Continue only if a projectile has been received
        proj_exists = true;
        received = ir_uart_getc();
        inchar = received;


        if (inchar < 10) {
            proj[0] = (uint8_t) map << (inchar - 1);
            proj[2] = 0; // Move straight

        } else if (inchar >= 10 && inchar < 20) {
            proj[0] = (uint8_t) map << (inchar - 11);
            proj[2] = 2; // Move left

        } else if (inchar >= 20 && inchar < 30) {
            proj[0] = (uint8_t) map << (inchar - 21);
            proj[2] = 1; // Move right
        } else if (inchar >= 30) {
            proj[0] = (uint8_t) map << (inchar - 31);
            proj[2] = 3; // Other player has lost
        } else if (inchar >= 40) {
            proj[0] = (uint8_t) map << (inchar - 41);
            proj[2] = 4; // Other board is ready to continue
        }
    }
    return proj_exists;
}


/*
    File: shields.c
    Authors: Ryan Sadler, Ronan Fraser
    Desc: Controls the shield movement during the initial setup phase
*/

#define NAV_UP 3
#define NAV_DOWN 2
#define NAV_LEFT 1
#define NAV_RIGHT 7
#define NAV_P 4
#define BUTTON_P 5

#include "system.h"
#include "pio.h"
#include "led.h"
#include "shields.h"


/* Shield location setup, allows user to move shields around the screen and place them in the desired location */
uint8_t move_shield(uint8_t col, uint8_t shield)
{
    if (shield != 1) {
        bitmap[shield1_col] = bitmap[shield1_col] ^ shield1_map;
    }
    if ((buttons[NAV_RIGHT] == true) && ((bitmap[col] & 0x01) == 0) && !((col == shield1_col) && (bitmap[col] & (shield1_map << 1)))) { // Move right
        bitmap[col] = bitmap[col] >> 1;
        buttons[NAV_RIGHT] = false;
    } else if ((buttons[NAV_LEFT] == true) && ((bitmap[col] & (0x01 << 6)) == 0) && !((col == shield1_col) && (bitmap[col] & (shield1_map >> 1)))) { // Move left
        bitmap[col] = bitmap[col] << 1;
        buttons[NAV_LEFT] = false;
    } else if ((buttons[NAV_UP] == true) && (col != 0) && !(((col - 1) == shield1_col) && (bitmap[col] & shield1_map))) { // Move up
        bitmap[col - 1] = bitmap[col];
        bitmap[col] = 0x00;
        col--;
        buttons[NAV_UP] = false;
    } else if ((buttons[NAV_DOWN] == true) && (col != 3) && !(((col + 1) == shield1_col) && (bitmap[col] & shield1_map))) { // Move down
        bitmap[col + 1] = bitmap[col];
        bitmap[col] = 0x00;
        col++;
        buttons[NAV_DOWN] = false;
    }
    bitmap[shield1_col] = bitmap[shield1_col] | shield1_map;
    return col;
}

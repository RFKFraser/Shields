/*
File: player.c
Authors: Ryan Sadler, Ronan Fraser

    Controls the player movement and fireing mechanics
*/
 
#include "system.h"
#include "player.h"
#include "led.h"
#include "pio.h"

#define LED_PIO PIO_DEFINE (PORT_C, 2)
#define NAV_UP 3
#define NAV_DOWN 2
#define NAV_LEFT 1
#define NAV_RIGHT 7
#define NAV_P 4
#define BUTTON_P 5


/* Moves the player left or right on the buttom of the screen */
uint8_t move_player(int* buttons, uint8_t playermap)
{
    if (buttons[BUTTON_P] == true) {
        // Don't move, trying to fire
    } else if ((buttons[NAV_LEFT] == true) && ((playermap & (0x01 << 6)) == 0)) {
        buttons[NAV_LEFT] = true;
        playermap = playermap << 1; // Move player left
    } else if ((buttons[NAV_RIGHT] == true) && ((playermap & 0x01) == 0)) {
        buttons[NAV_RIGHT] = false;
        playermap = playermap >> 1; // Move player right
    }
    return playermap;
}

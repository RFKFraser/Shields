/*
    File: button_checker.c
    Authors: Ryan Sadler, Ronan Fraser
    Desc: Allows pressed buttons to be found
*/


#define NAV_UP 3
#define NAV_DOWN 2
#define NAV_LEFT 1
#define NAV_RIGHT 7
#define NAV_P 4
#define BUTTON_P 5
#define LED_PIO PIO_DEFINE (PORT_C, 2)
#define STRAIGHT 0
#define LEFT 1
#define RIGHT 2


#include "system.h"
#include "pio.h"
#include "fire.h"
#include "led.h"


/* Fire projectile to the left */
void fire_proj_left(uint8_t map, int* proj)
{
	proj[0] = map;
	proj[1] = 3;
	proj[2] = LEFT;
}


/* Fire projectile to the right */
void fire_proj_right(uint8_t map, int* proj)
{
	proj[0] = map;
	proj[1] = 3;
	proj[2] = RIGHT;
}


/* Fire projectile straight ahead */
void fire_proj(uint8_t map, int* proj)
{
	proj[0] = map;
	proj[1] = 3;
	proj[2] = STRAIGHT;
}


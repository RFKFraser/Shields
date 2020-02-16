/*
    File: button_checker.h
    Authors: Ryan Sadler, Ronan Fraser
    Desc: Allows pressed buttons to be found
*/


#ifndef FIRE_H
#define FIRE_H

#include "system.h"
#include "pio.h"
#include "led.h"

/* Fire projectile to the left */
void fire_proj_left(uint8_t map, int* proj);


/* Fire projectile to the right */
void fire_proj_right(uint8_t map, int* proj);


/* Fire projectile straight ahead */
void fire_proj(uint8_t map, int* proj);

#endif

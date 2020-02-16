/*
File: player.h
Authors: Ryan Sadler, Ronan Fraser

	Controls the player movement and fireing mechanics
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "system.h"
#include "led.h"
#include "pio.h"

/* Moves the player either left or right on the bottom of the screen */
uint8_t move_player(int* buttons, uint8_t playermap);

#endif

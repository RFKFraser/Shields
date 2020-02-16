/*
file:    matrix.h
authors: Ryan Sadler, Ronan Fraser

    This module allows the LED matrix display to light up in a desired patturn using bitmaps
*/


#ifndef MATRIX_H
#define MATRIX_H

#include "system.h"
#include "pio.h"


/*  Initialize the LED matrix display's rows and columns
    Usage: matrix_init() */
void matrix_init (void);


/* Returns the currently active column number */
uint8_t get_active(void);


/*  Cycle through columns, turning on the current one and turning off the previous one
    Usage: display_column (<bitmap patturn for current row>, <integer number of current row>)*/
void display_column (uint8_t row_pattern, uint8_t current_column);

#endif

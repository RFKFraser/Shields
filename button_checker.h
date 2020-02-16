/*
    File: button_checker.h
    Authors: Ryan Sadler, Ronan Fraser
    Desc: Allows pressed buttons to be found
*/


#ifndef BUTTON_H
#define BUTTON_H


#include "button.h"
#include "navswitch.h"
#include "system.h"
#include "pio.h"


/*  Initializes the button and nav switch */
void buttons_init(void);


/* Checks each button on the ucfk4 and places a 1 or 0 into the 'buttons' array depending on whether they are currently pressed or not */
int* check_buttons(void);

#endif

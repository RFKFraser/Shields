/*
    File: button_checker.c
    Authors: Ryan Sadler, Ronan Fraser
    Desc: Allows pressed buttons to be found
*/


#define PUSH_BUTTON 0
#define NAV_UP 3
#define NAV_DOWN 2
#define NAV_LEFT 1
#define NAV_RIGHT 7
#define NAV_P 4
#define BUTTON_P 5


#include "button.h"
#include "navswitch.h"
#include "system.h"
#include "pio.h"
#include "button_checker.h"
#include "matrix.h"


int buttons[8] = {0}; // Navigational switch north; Navigational switch south; Navigational switch east; Navigational switch west; Navigational switch down; Push-button


/*  Initializes the button and nav switch */
void buttons_init(void)
{
    button_init();
    navswitch_init();
}


/* Checks each button on the ucfk4 and places a 1 or 0 into the 'buttons' array depending on whether they are currently pressed or not */
int* check_buttons(void)
{
    uint8_t active_column;
    active_column = get_active();
	pio_output_high (active_column); // Disable column to avoid led matrix flickering
    navswitch_update();
    button_update();
    
    if (navswitch_push_event_p (NAVSWITCH_NORTH)) { // Move right from perspective of game's intended orientation
        buttons[NAV_RIGHT] = true;
    } else {
        buttons[NAV_RIGHT] = false;
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) { // Move left from perspective of game's intended orientation
        buttons[NAV_LEFT] = true;
    } else {
        buttons[NAV_LEFT] = false;
    }

    if (navswitch_push_event_p (NAVSWITCH_EAST)) { // Move down from perspective of game's intended orientation
        buttons[NAV_DOWN] = true;
    } else {
        buttons[NAV_DOWN] = false;
    }

    if (navswitch_push_event_p (NAVSWITCH_WEST)) { // Move up from perspective of game's intended orientation
        buttons[NAV_UP] = true;
    } else {
        buttons[NAV_UP] = false;
    }

    if (navswitch_push_event_p (NAVSWITCH_PUSH)) { // Nav switch pressed down
        buttons[NAV_P] = true;
    } else {
        buttons[NAV_P] = false;
    }

    if (button_down_p (PUSH_BUTTON)) { // Momentary push button is pressed
        buttons[BUTTON_P] = true;
    } else {
        buttons[BUTTON_P] = false;
    }
    pio_output_low (active_column); // Re-enable led column
    return buttons;
}

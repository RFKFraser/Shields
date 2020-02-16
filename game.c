/*
    File: game.c
    Authors: Ryan Sadler, Ronan Fraser
    Desc: The main program for the ucfk4 'Shields' game developed for ENCE260
*/


// include files
#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "matrix.h"
#include "ir_uart.h"
#include "button_checker.h"
#include "led.h"
#include "tinygl.h"
#include "text_init.h"
#include "player.h"
#include "fire.h"
#include "ir_com.h"


// definitions
#define CYCLE 500
#define BUTTON_POLL 10
#define SHIELD_POLL 10
#define PLAYER_POLL 10
#define PROJ_POLL 100
#define TEST_DEATH_POLL 100
#define LED_PIO PIO_DEFINE (PORT_C, 2)
#define TEXT_POLL 1
#define NAV_UP 3
#define NAV_DOWN 2
#define NAV_LEFT 1
#define NAV_RIGHT 7
#define NAV_P 4
#define BUTTON_P 5
#define STRAIGHT 0
#define LEFT 1
#define RIGHT 2

uint8_t shield1_col = 10;
uint8_t shield1_map = 0x00;
uint8_t shield2_col = 10;
uint8_t shield2_map = 0x00;

int* buttons; // Navigational switch north; Navigational switch south; Navigational switch east; Navigational switch west; Navigational switch down; Push-button


/* Screen display */
uint8_t bitmap[] = {
    0x00, 0x00, 0x00, 0x00, 0x00
};


/* Call all initialization functions */
void initialize (void)
{
    system_init ();
    matrix_init();
    buttons_init();
    pacer_init(CYCLE);
    ir_uart_init();
    led_init();
    textscreen_init();
}


/* Tells the matrix display to show the next column in the bitmap */
int refresh_screen(int current_column)
{
    display_column (bitmap[current_column], current_column);

    current_column++;

    if (current_column > 4) {
        current_column = 0;
    }

    return current_column;
}


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


/* Main function, uses a scheduler to call other functions */
int main (void)
{
    // Define local variables
    uint8_t current_column = 0;
    uint8_t button_tick = BUTTON_POLL;
    uint8_t shield_tick = SHIELD_POLL;
    uint8_t player_tick = PLAYER_POLL;
    uint8_t proj_tick = PROJ_POLL;
    uint8_t text_tick = TEXT_POLL;
    uint8_t test_death_tick = TEST_DEATH_POLL;
    uint8_t prev_map = 0x00;
    bool proj_exists = false;
    bool enemy_exists = false;
    bool setup = false;
    bool ingame = false;
    bool startup = true;
    bool set_text = true;
    bool inst_1 = false;
    bool inst_2 = false;
    bool win = false;
    uint8_t col = 0;
    uint8_t shield = 1;
    int fired_proj[3] = {0}; // Projectile fired by player
    int attacking_proj[3] = {0}; // Projectile fired by opponent
    initialize();
    pio_output_low(LED_PIO);
    bitmap[0] = 0x60;
    bitmap[4] = 0x7F;


    while (1) {
        pacer_wait(); // Wait

        // Increment ticks
        button_tick++;
        shield_tick++;
        text_tick++;
        player_tick++;
        proj_tick++;

        // Perform tasks
        if (setup || ingame) {
            current_column = refresh_screen(current_column);
        }

        if (button_tick >= BUTTON_POLL) { // Check which button/s is/are pressed
            buttons = check_buttons();
            button_tick = 0;
        }

        if (shield_tick >= SHIELD_POLL) {
            shield_tick = 0;
            if (setup) { // Allow shield placement if in setup mode
                col = move_shield(col, shield);
                if (buttons[NAV_P] && (shield == 1) && (col != 0)) { // Set location of shield 1
                    buttons[NAV_P] = 0;
                    shield1_col = col;
                    shield1_map = bitmap[col];
                    shield += 1;
                    col = 0;
                    bitmap[0] = 0x60;
                } else if (buttons[NAV_P] && (shield == 2) && (col != 0)) { // Set location of shield 2
                    buttons[NAV_P] = 0;
                    shield2_col = col;
                    shield2_map = bitmap[col];
                    shield += 1;
                    col = 0;
                    setup = false;
                    bitmap[4] = 0x08;
                    inst_2 = true;
                    set_text = true;
                }
            }
        }
        if ((text_tick >= TEXT_POLL) && startup) { // Display scrolling text on screen
            text_tick = 0;
            if (buttons[NAV_P]) {
                buttons[NAV_P] = 0;
                startup = false;
                inst_1 = true;
                set_text = true;
            } else {
                if (set_text) {
                    tinygl_text (" SHIELDS!  PUSH TO START ");
                    set_text = false;
                } else {
                    tinygl_update ();
                }
            }
        } else if (inst_1 && (text_tick >= TEXT_POLL)) { // Display scrolling instructions on screen
            text_tick = 0;
            if (buttons[NAV_P]) {
                buttons[NAV_P] = 0;
                inst_1 = false;
                setup = true;
            } else {
                if (set_text) {
                    tinygl_text (" PLACE SHIELDS USING NAV ");
                    set_text = false;
                } else {
                    tinygl_update ();
                }
            }
        } else if (inst_2 && (text_tick >= TEXT_POLL)) { // Display scrolling instructions on screen
            text_tick = 0;
            if (buttons[NAV_P]) {
                buttons[NAV_P] = 0;
                inst_2 = false;
                ingame = true;
            } else {
                if (set_text) {
                    tinygl_text (" USE BUTTON AND NAV TO SHOOT ");
                    set_text = false;
                } else {
                    tinygl_update ();
                }
            }
        }
        if (ingame) {
            if ((player_tick >= PLAYER_POLL) & !(attacking_proj[2] == 3)) { // Test for player movement and projectile firing
                player_tick = 0;
                bitmap[4] = move_player(buttons, bitmap[4]);
                if ((buttons[BUTTON_P] == true) && !proj_exists) {
                    buttons[BUTTON_P] = false;
                    if (navswitch_release_event_p (NAVSWITCH_SOUTH)) { // Player fires a projectile to the left
                        fire_proj_left(bitmap[4], fired_proj);
                        proj_exists = true;
                        proj_tick = PROJ_POLL;
                    } else if (navswitch_release_event_p (NAVSWITCH_NORTH)) { // Player fires a projectile to the right
                        fire_proj_right(bitmap[4], fired_proj);
                        proj_exists = true;
                        proj_tick = PROJ_POLL;
                    } else if (navswitch_release_event_p (NAVSWITCH_WEST)) { // Player fires a projectile straight ahead
                        fire_proj(bitmap[4], fired_proj);
                        proj_exists = true;
                        proj_tick = PROJ_POLL;
                    }
                }
            }
            if ((proj_tick >= PROJ_POLL) & !(attacking_proj[2] == 3)) {
                proj_tick = 0;
                if (!enemy_exists) { // Only check for incoming projectile if there isn't one on the screen currently
                    enemy_exists = getpoint(attacking_proj);
                }
                if (proj_exists) { // Player has fired a projectile
                    if ((fired_proj[2] == LEFT) && (fired_proj[0] == 0x40)) { // Projectile is on left wall
                        fired_proj[2] = RIGHT;
                    } else if ((fired_proj[2] == RIGHT) && (fired_proj[0] == 0x01)) { // Projectile is on right wall
                        fired_proj[2] = LEFT;
                    }
                    if (fired_proj[2] == LEFT) {
                        fired_proj[0] = fired_proj[0] << 1;
                    } else if (fired_proj[2] == RIGHT) {
                        fired_proj[0] = fired_proj[0] >> 1;
                    }
                    if (fired_proj[1] < 3) {
                        bitmap[fired_proj[1] + 1] = prev_map;
                    }
                    prev_map = bitmap[fired_proj[1]];
                    bitmap[fired_proj[1]] = fired_proj[0] | bitmap[fired_proj[1]]; // Display projectile in position (using bitmap)
                    fired_proj[1]--;
                    if (fired_proj[1] == -2) { // projectile is now on the first tile of the opponents screen
                        proj_exists = false;
//                        sendpoint(fired_proj);
                    }
                }
                if (enemy_exists  & !(attacking_proj[2] == 3)) { // An enemy projectile is on the player's screen
                    if (attacking_proj[1] != 0) {
                        bitmap[attacking_proj[1] - 1] = prev_map;
                    }
                    if ((attacking_proj[2] == LEFT) && (attacking_proj[0] == 0x40)) { // Projectile is on left wall
                        attacking_proj[2] = RIGHT;
                    } else if ((attacking_proj[2] == RIGHT) && (attacking_proj[0] == 0x01)) { // Projectile is on right wall
                        attacking_proj[2] = LEFT;
                    }
                    if (attacking_proj[2] == LEFT) {
                        attacking_proj[0] = attacking_proj[0] << 1;
                    } else if (attacking_proj[2] == RIGHT) {
                        attacking_proj[0] = attacking_proj[0] >> 1;
                    }
                    prev_map = bitmap[attacking_proj[1]];
                    if (bitmap[attacking_proj[1]] & attacking_proj[0]) {
                        enemy_exists = false;
                    }
                    bitmap[attacking_proj[1]] = attacking_proj[0] ^ bitmap[attacking_proj[1]]; // Display projectile in position (using bitmap)
                    attacking_proj[1]++;
                }
            }
            if (proj_exists) { // Turn blue led on if you can fire a projectile
                pio_output_low(LED_PIO);
            } else {
                pio_output_high(LED_PIO);
            }
            if (attacking_proj[2] == 3) {
                ingame = false;
                win = true;
                set_text = true;
            }
        }
        if (setup) { // Allow user to move shields around screen
            col = move_shield(col, shield);
        }
        if (ingame) {
            if (test_death_tick >= TEST_DEATH_POLL) { // Check to see if the player has died
                if (bitmap[4] == 0x00) { // Player is dead
                    ingame = false;
                    fired_proj[0] = 0x01;
                    fired_proj[1] = 6;
                    fired_proj[2] = 3;
                    sendpoint(fired_proj); // Tell other board that they have won
                    set_text = true;
                }
            }
        }

        if (!ingame && !setup && !startup && !inst_1 && !inst_2) { // && !ready) {
            if (win) {
                if (set_text) {
                    tinygl_text (" YOU WIN ");
                    set_text = false;
                }
                if (text_tick >= TEXT_POLL) { // Display scrolling text on screen
                    text_tick = 0;
                    tinygl_update ();
                }
            } else {
                if (set_text) {
                    tinygl_text (" YOU LOSE ");
                    set_text = false;
                }
                if (text_tick >= TEXT_POLL) { // Display scrolling text on screen
                    text_tick = 0;
                    tinygl_update ();
                }
            }
        }
    }
    return 0;
}



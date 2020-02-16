/*
    File: text_init.c
    Authors: Ryan Sadler, Ronan Fraser
    Desc: Displays scrolling text
    
*/
 
#include "system.h"
#include "tinygl.h"
#include "pacer.h"
#include "../fonts/font3x5_1.h"
#include "text_init.h"

// Define polling rate in HZ
#define LOOP_RATE 500

// Define text upadate rate in characters per 10s
#define MESSAGE_RATE 30


void textscreen_init(void)  //initialise values required by tinygl
{
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    
}


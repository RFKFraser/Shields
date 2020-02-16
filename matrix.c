/*
    File: matrix.c
    Authors: Ryan Sadler, Ronan Fraser
    Desc: Controls the LED matrix display using bitmaps
    
*/

#include "system.h"
#include "pio.h"
#include "matrix.h"


/* Define PIO pins driving LED matrix rows. */
const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, 
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/* Define PIO pins driving LED matrix columns. */
const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


uint8_t prev_column = 0;
pio_t active_column = 0;


/* Initialize the LED matrix display's rows and columns */
void matrix_init (void)
{
    pio_config_set(LEDMAT_ROW1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW5_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW6_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW7_PIO, PIO_OUTPUT_HIGH);

    pio_config_set(LEDMAT_COL1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL5_PIO, PIO_OUTPUT_HIGH);
}


/* Returns the currently active column number */
uint8_t get_active(void)
{
    return active_column;
}


/* Cycle through columns, turning on the current one and turning off the previous one */
void display_column (uint8_t row_pattern, uint8_t current_column)
{
    int current_row;
    pio_output_high(cols[prev_column]);
    for (current_row = 0; current_row < 7; current_row++) {
        if (row_pattern & (1 << current_row)) {
            pio_output_low (rows[current_row]);
        } else {
            pio_output_high (rows[current_row]);
        }
    }
    pio_output_low (cols[current_column]);
    prev_column = current_column;
	active_column = cols[current_column];
}

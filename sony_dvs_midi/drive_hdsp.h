#pragma once

#include "font5x7.h"

#define ROW_COUNT 7
#define COL_COUNT 5
#define DIGIT_COUNT 4
#define MODULE_COUNT 3
#define CHAR_COUNT (DIGIT_COUNT*MODULE_COUNT)
#define DATA_LENGTH (COL_COUNT * CHAR_COUNT)

void setup_screen_io();

void flush_row();
void flush_column();

char custom_font_table[DATA_LENGTH] = {
  0b0000000,
  0b0001000,
  0b0011100,
  0b0110110,
  0b1111111,

  0b1111111,
  0b0111110,
  0b0011100,
  0b0001000,
  0b0000000,

  0b0000000,
  0b0001000,
  0b0011100,
  0b0111110,
  0b1111111,

  0b0001000,
  0b0001000,
  0b1111111,
  0b0001000,
  0b0001000,
};


bool center = false;
bool right_align = false;


#define MAX_TEXT_LEN 255
char text[MAX_TEXT_LEN+1] = "xxhello \1\2\3\4";

uint32_t ms_per_col = 20;
uint32_t ms_per_char = ms_per_col * COL_COUNT;

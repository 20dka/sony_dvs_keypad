#include "drive_hdsp.h"

const int digit_row_data = 10;
const int sipo_clk = 16;
const int sipo_column_select = 14;
const int digit_clk = 15;
const int digit_blanking = A0;

void setup_screen_io() {
  pinMode(digit_row_data, OUTPUT);
  pinMode(sipo_clk, OUTPUT);
  pinMode(sipo_column_select, OUTPUT);
  pinMode(digit_clk, OUTPUT);
  pinMode(digit_blanking, OUTPUT);

  digitalWrite(digit_blanking, HIGH);
  digitalWrite(digit_clk, HIGH);
  digitalWrite(sipo_clk, HIGH);
}

void row_clock_adv() {
  PORTB = bitClear(PORTB, 1);
  PORTB = bitSet(PORTB, 1);
}

void col_clock_adv() {
  digitalWrite(sipo_clk, LOW);
  digitalWrite(sipo_clk, HIGH);
}

void flush_row() {
  digitalWrite(digit_row_data, HIGH);
  for (int i = 0; i < ROW_COUNT * DIGIT_COUNT * MODULE_COUNT; i++)
    row_clock_adv();
}

void flush_column() {
  digitalWrite(sipo_column_select, HIGH);
  for (int i = 0; i < COL_COUNT; i++)
    col_clock_adv();
}

void send_one_row(uint8_t row) {
  for(int i = 0; i < ROW_COUNT; i++) {
    if ((row & 1)) {
      PORTB = bitClear(PORTB, 6);
    } else {
      PORTB = bitSet(PORTB, 6);
    }
    row = row >> 1;
    row_clock_adv();
  }
}

void col_clock_feed() {
  digitalWrite(sipo_column_select, LOW);
  col_clock_adv();
  digitalWrite(sipo_column_select, HIGH);
}


unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void update_screen() {
  col_clock_feed();

  uint32_t len = strlen(text);
  uint32_t time_offset = millis();

  bool scroll = len > CHAR_COUNT;
  bool scroll_col = false;

  for (byte c_i = 0; c_i < COL_COUNT; c_i++) { // which column of this digit
    for (byte m_i = 0; m_i < MODULE_COUNT; m_i++) { // which module
      for (byte d_i = DIGIT_COUNT; d_i-- > 0;) { //which digit of this module
        { // send from col_data
          //int idx = c_i;
          //idx += (d_i) * COL_COUNT;
          //idx += (m_i) * (COL_COUNT*DIGIT_COUNT);
          //send_one_row(col_data[idx]);
        }

        int col_scroll_offset = 0;
        if (scroll && scroll_col)
          col_scroll_offset = ( time_offset / ms_per_col) % COL_COUNT; //offset column based on timer

        int text_offset = d_i + (m_i*DIGIT_COUNT); //get text offset

        if (scroll) {
          text_offset -= CHAR_COUNT;
          text_offset += (time_offset / ms_per_char) % (len + CHAR_COUNT); //adjust character offset based on timer

        } else if (center) {
          text_offset -= CHAR_COUNT/2;
          text_offset += len/2;
          
        } else if (right_align) {
          text_offset -= CHAR_COUNT;
          text_offset += len;
          
        }

        text_offset += (col_scroll_offset + c_i) / COL_COUNT;

        if (text_offset >= len || text_offset < 0) {
          send_one_row(0);
        } else {
          int col_offset = (col_scroll_offset + c_i) % COL_COUNT;

          int char_code = text[text_offset]; //get character base
          int font_idx = char_code * COL_COUNT; 

          font_idx += col_offset;
          
          uint8_t row = 0;
          
          if (font_idx < (CHAR_COUNT * COL_COUNT))
            row = custom_font_table[font_idx - COL_COUNT];
          else
            row = pgm_read_word_near(font5x7 + font_idx);
  
          row = reverse(row) >> 1;
          send_one_row(row);
        }
      }
    }

    digitalWrite(digit_blanking, LOW);
    delayMicroseconds(500);
    digitalWrite(digit_blanking, HIGH);
    col_clock_adv();
  }

  //flush_row();
  //flush_column();
}

const int digit_row_data = 10;
const int sipo_clk = 16;
const int sipo_column_select = 14;
const int digit_clk = 15;
const int digit_blanking = A0;

#define ROW_COUNT 7
#define COL_COUNT 5
#define DIGIT_COUNT 4
#define MODULE_COUNT 3
//1 byte per row
#define DATA_LENGTH (1 * COL_COUNT * DIGIT_COUNT * MODULE_COUNT)

void row_clock_adv() {
  digitalWrite(digit_clk, LOW);
    //delayMicroseconds(50);
  digitalWrite(digit_clk, HIGH);
}

void col_clock_adv() {
  digitalWrite(sipo_clk, LOW);
    //delayMicroseconds(50);
  digitalWrite(sipo_clk, HIGH);
}

void flush_row() {
  digitalWrite(digit_row_data, HIGH);
  for (int i = 0; i < ROW_COUNT*DIGIT_COUNT*MODULE_COUNT;i++)
    row_clock_adv();
}

void flush_column() {
  digitalWrite(sipo_column_select, HIGH);
  for (int i = 0; i < COL_COUNT; i++)
    col_clock_adv();
}


void setup() {
  Serial.begin(115200);
  pinMode(digit_row_data, OUTPUT);
  pinMode(sipo_clk, OUTPUT);
  pinMode(sipo_column_select, OUTPUT);
  pinMode(digit_clk, OUTPUT);
  pinMode(digit_blanking, OUTPUT);
  digitalWrite(digit_blanking, HIGH);

  
  flush_row();
  flush_column();
}

void show_all_columns() {
  digitalWrite(sipo_column_select, LOW);
  for (int i = 0; i < COL_COUNT; i++)
    col_clock_adv();
}


uint8_t col_data[DATA_LENGTH] = {
  0b0000000,
  0b0001000,
  0b0011100,
  0b0111110,
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

  0b1111111,
  0b0111110,
  0b0011100,
  0b0001000,
  0b0000000,
};

void send_one_row(uint8_t row) {
  for(int i = 0; i < ROW_COUNT; i++) {
    digitalWrite(digit_row_data, (row & 1));
    row>>=1;
    row_clock_adv();
  }
}

void col_clock_feed() {
  digitalWrite(sipo_column_select, LOW);
  col_clock_adv();
  digitalWrite(sipo_column_select, HIGH);
}

void loop() {
/*
    //enable all columns
    show_all_columns();


    send_one_row(0b00001110);

    send_one_row(0b00001101);

    send_one_row(0b00001011);

    send_one_row(0b01010101);
*/


    col_clock_feed();

  for (byte c_i = 0; c_i < COL_COUNT; c_i++) {
    for (byte m_i = 0; m_i < MODULE_COUNT; m_i++) {
      for (byte d_i = 0; d_i < DIGIT_COUNT; d_i++) {
        int idx = c_i;
        idx += (DIGIT_COUNT - d_i-1) * COL_COUNT;
        idx += (MODULE_COUNT - m_i-1) * DIGIT_COUNT;
        send_one_row(col_data[idx]);
      }

    }
    col_clock_adv();
    digitalWrite(digit_blanking, LOW);
    delayMicroseconds(200);
    digitalWrite(digit_blanking, HIGH);
  }

  //delay(2000);
}

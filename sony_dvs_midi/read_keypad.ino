const int clk_bit = 1;
const int row_data = 3;
const int mode_select = 4;
const int input_bit = 6;

void setup_keypad_io() {
  DDRD = bitSet(DDRD, clk_bit); //pinMode(clk, OUTPUT);
  pinMode(row_data, OUTPUT);
  pinMode(mode_select, OUTPUT);
  DDRD = bitClear(DDRD, input_bit); //pinMode(input_pin, INPUT);
}

//reads D5
bool read_pin() {
  return PINC & bit(input_bit);
}

//toggles D2
void clock_adv() {
  PORTD = bitClear(PORTD, clk_bit);
  PORTD = bitSet(PORTD, clk_bit);
}

void row_adv() {
  digitalWrite(row_data, HIGH);
  digitalWrite(mode_select, HIGH);
  clock_adv();
  digitalWrite(mode_select, LOW);
}

void flush_row_reg() {
#ifdef VERBOSE
  Serial.print("Flushing row register...");
#endif
  for(int i=0;i<BUTTON_ROW_COUNT;i++) {
    row_adv();
    clock_adv();
  }
#ifdef VERBOSE
  Serial.println(" Done");
#endif
}

void set_latch() { digitalWrite(mode_select, LOW); }
void clear_latch() { digitalWrite(mode_select, HIGH); }

void set_row_data() { digitalWrite(row_data, LOW); }
void clear_row_data() { digitalWrite(row_data, HIGH); }


void read_keypad() {
  noInterrupts();
  uint32_t t1 = micros();
  flush_row_reg();
#ifdef VERBOSE
  Serial.println("Setting row register bit");
#endif
  clear_latch();
  set_row_data();
  clock_adv();
  clear_row_data();

#ifdef VERBOSE
  Serial.println("Loading row register bit");
#endif
  set_latch();
  
  memset(data.arr, 0, OUTPUT_REGISTER_COUNT*BUTTON_ROW_COUNT);

  for(int row = 0; row < BUTTON_ROW_COUNT; row++) {

#ifdef VERBOSE
    Serial.print("Shifting out bits for row ");
    if (row < 10) Serial.print('0');
    Serial.print(row);
    Serial.print(": ");
#endif

    clock_adv();
    //shift PISOs
    for(int i = 0; i < OUTPUT_REGISTER_COUNT; i++) {
      for(int f = 0; f < 8; f++) {
        clock_adv();
        bool state = read_pin();
        data.arr[(row*OUTPUT_REGISTER_COUNT)+i] <<= 1;
        data.arr[(row*OUTPUT_REGISTER_COUNT)+i] += (state ? 1 : 0);
#ifdef VERBOSE
        Serial.print(state ? 'x' : '.');
#endif
      }
#ifdef VERBOSE
      Serial.print(" ");
#endif
    }

#ifdef VERBOSE
    Serial.println("");
interrupts();
    delay(100);
noInterrupts();
#endif

    row_adv();
  }

uint32_t t2 = micros();
interrupts();

}

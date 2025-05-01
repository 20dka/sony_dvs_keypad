const int clk_bit = 1;
const int row_data = 3;
const int mode_select = 4;
const int input_bit = 6;

const int led_enable = 6;
const int led_data = 7;

/*
const char* signal_names[] = {"Clock", "Row Data", "Mode Select"};
const int signal_map[] = {clk, row_data, mode_select};
const int buttons[] = {9,10,11};
bool buttonstate[3];
void check_btn(int pin);
*/

#define VERBOSE

// number of 74xx164 chips used for driving button matrix rows _per board_
#define ROW_CHIPS 1
#define ROW_COUNT (ROW_CHIPS*8)

// _total_ number of 74xx166 chips on bus(ex: Key Frame control [3] + Numeric keypad [2])
#define OUTPUT_REGISTER_COUNT 5
//uint8_t data[OUTPUT_REGISTER_COUNT*ROW_COUNT];

union {
  struct test {
    uint8_t parity0;
    uint8_t enc0_low;
    uint8_t :8;
    uint8_t parity1;
    uint8_t :16;
    uint8_t enc0_high;
    uint8_t :40;
    uint8_t btn0_0;
    uint8_t :32;
    uint8_t btn0_1;
    uint8_t :8;
    uint8_t btn1_0;
    uint8_t :32;
    uint8_t btn1_1;
    uint8_t :32;
    uint8_t btn1_2;
    uint8_t :32;
    uint8_t btn1_3;
    uint8_t :16;
    uint8_t btn0_2;
    uint8_t :8;
    uint8_t btn1_4;
    
  }__attribute__((packed)) fields;
  uint8_t arr[OUTPUT_REGISTER_COUNT*ROW_COUNT];
} data;

void clock_adv();
void row_adv();
void clear_row_reg();

void setup() {
  Serial.begin(115200);
  //Serial.println("\nHello :)");
  DDRD = bitSet(DDRD, clk_bit); //pinMode(clk, OUTPUT);
  pinMode(row_data, OUTPUT);
  pinMode(mode_select, OUTPUT);
  pinMode(led_enable, OUTPUT);
  pinMode(led_data, OUTPUT);
  DDRD = bitClear(DDRD, input_bit); //pinMode(input_pin, INPUT);
/*
  pinMode(buttons[0], INPUT_PULLUP);
  pinMode(buttons[1], INPUT_PULLUP);
  pinMode(buttons[2], INPUT_PULLUP);
*/
  flush_row_reg();
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
  for(int i=0;i<ROW_COUNT;i++) {
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

void loop() {
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
  
  memset(data.arr, 0, OUTPUT_REGISTER_COUNT*ROW_COUNT);

  for(int row = 0; row < ROW_COUNT; row++) {

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

  uint16_t enc0 = data.fields.enc0_low + (data.fields.enc0_high<<8);
  Serial.println(enc0);
  //Serial.println(data.fields.btn0_2 + (data.fields.btn0_1<<8) + ((uint32_t)data.fields.btn0_0<<16));

  //Serial.println(t2-t1);
  //Serial.println(1000000/(t2-t1));
/*
  static bool last_state = false;
  bool state = digitalRead(input_pin);
  if (state != last_state) {
    Serial.println(state ? 'H' : 'L');
  }

  for (int i = 0; i<3;i++){
    check_btn(i);
  }

  last_state = state;
  */
}
/*
void check_btn(int pin){
  bool state = digitalRead(buttons[pin]);
  if (state != buttonstate[pin]) {
    delay(100);
    digitalWrite(signal_map[pin], state);
    buttonstate[pin] = state;
    Serial.print("Setting pin ");
    Serial.print(pin+1);
    Serial.print("/ ");
    Serial.print(signal_names[pin]);
    Serial.print(" to ");
    Serial.println(state ? 'H' : 'L');
  }
}
*/

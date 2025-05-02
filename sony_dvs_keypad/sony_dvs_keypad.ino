//#define VERBOSE

// number of 74xx164 chips used for driving button matrix rows _per board_
#define ROW_CHIPS 1
#define ROW_COUNT (ROW_CHIPS*8)

// _total_ number of 74xx166 chips on bus(ex: Key Frame control [3] + Numeric keypad [2])
#define OUTPUT_REGISTER_COUNT 5


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

void setup_keypad_io();
void flush_row_reg();

void setup() {
  Serial.begin(115200);
  Serial.println("\nHello :) Sony DVS Keypad test");
  setup_keypad_io();

  flush_row_reg();
}


void loop() {
  read_keypad();

  uint16_t enc0 = data.fields.enc0_low + (data.fields.enc0_high<<8);
  Serial.println(enc0);
}

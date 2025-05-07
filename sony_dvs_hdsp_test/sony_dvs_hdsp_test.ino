#include "font5x7.h"

#include "drive_hdsp.h"

void setup() {
  Serial.begin(115200);

  setup_screen_io();
  
}

void loop() {
  center = true;
  right_align = false;

  //memset(text, 0, MAX_TEXT_LEN);
  //ultoa(millis(), text, 10);

  noInterrupts();
  uint32_t t1 = micros();

  update_screen();

  uint32_t t2 = micros();
  interrupts();
  //Serial.println(t2-t1);

  recv_serial();
}

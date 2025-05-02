const uint8_t serial_len = 100;
char serial_buf[serial_len];

void recv_serial() {
    static byte idx = 0;

    while (Serial.available() > 0) {
        char rc = Serial.read();

        if (rc == '\n') {
            serial_buf[idx] = '\0'; // terminate the string
            idx = 0;
            strcpy(text, serial_buf);
        } else if (rc >= ' ' && rc <= '~') {
            serial_buf[idx++] = rc;
            if (idx >= serial_len) {
                idx = serial_len - 1;
            }
        }
    }
}

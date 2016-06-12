// LEDs
const unsigned int green_led_pin = 7;
bool green_led = false;

// STOP button pin
const unsigned int stop_pin = 2;

// application states
const unsigned int INITIAL_PING = 0;
const unsigned int PING = 1;
const unsigned int PONG = 2;
const unsigned int STOP_REQUEST = 3;
const unsigned int STOP_RESPONSE = 4;

// initial state
unsigned int state = INITIAL_PING;

// counters
unsigned int stop_counter = 0;  // count how many times stop was sent

// I/O
const byte EOM = 0x00;
const unsigned int msg_length = 2;
// msg format: 2B msg, 1B eom

byte in_buffer[msg_length];
byte out_buffer[msg_length];

// for handling emergency interrupt
volatile bool emergency = false;

void setup() {
  // input pins
  pinMode(green_led_pin, OUTPUT);
  pinMode(stop_pin, INPUT);
  
  // start serial connection
  Serial.begin(9600);  // pins 0 and 1
  Serial.setTimeout(1000);

  // initialize pins with desired states
  led(green_led_pin, green_led);

  // set interrupt function on STOP pin
  attachInterrupt(digitalPinToInterrupt(stop_pin), emergencyInterrupt, RISING);
}

void loop() {
  // reset buffers
  set_buffer(in_buffer, 0x00, 0x00);
  set_buffer(out_buffer, 0x00, 0x00);

  if (emergency) {
    state = STOP_REQUEST;
    emergency = false;
  }

  // send ping every second
  // green led: toggle every second
  if (state == INITIAL_PING) {
    set_buffer(out_buffer, 0xAA, EOM);
    Serial.write(out_buffer, msg_length);

    if (Serial.available() >= msg_length) {
      // Serial.readBytes(in_buffer, msg_length);
      Serial.readBytesUntil(EOM, in_buffer, msg_length);

      if (is_pong(in_buffer)) {
        state = PONG;
      } else {
        // wrong pong format
      }
      serial_flush();
    }
    
    green_led = !green_led; led(green_led_pin, green_led);
    delay(1000);
  }

  // send ping every second
  // green led: on at all times
  else if (state == PING) {
    green_led = true; led(green_led_pin, green_led);
    
    set_buffer(out_buffer, 0xAA, EOM);
    Serial.write(out_buffer, msg_length);

    delay(1000);

    if (Serial.available() >= msg_length) {
      // Serial.readBytes(in_buffer, msg_length);
      Serial.readBytesUntil(EOM, in_buffer, msg_length);

      if (is_pong(in_buffer)) {
        state = PONG;
      } else {
        // wrong pong format
      }
      serial_flush();
    }
  }

  // PONG msg came back!
  // green led: keep
  else if (state == PONG) {
    state = PING;
  }

  // STOP button pushed
  // green led: pulse 10x quickly
  else if (state == STOP_REQUEST) {
    if (stop_counter++ >= 10) {
      stop_counter = 0;
      state = INITIAL_PING;
    } else {
      green_led = !green_led; led(green_led_pin, green_led);
      set_buffer(out_buffer, 0xFF, EOM);
      Serial.write(out_buffer, msg_length);

      state = STOP_RESPONSE;
      delay(50);
    }
  }

  // module B responded for STOP
  // green led: keep
  else if (state == STOP_RESPONSE) {
    // this is fake
    state = STOP_REQUEST;
  }

  else {
    // unknown state, switch to INIT_PING?
    state = INITIAL_PING;
  }
}

void emergencyInterrupt() {
  emergency = true;
}

void led(const unsigned int pin, const bool state) {
  if (state)
    digitalWrite(pin, HIGH);
  else
    digitalWrite(pin, LOW);
}

bool is_pong(byte *buf) {
  return buf[0] == 0xCC && buf[1] == 0x00;
}

void serial_flush() {
  // Since Arduino 1.0, the Serial.flush() only removes outgoing data,
  // not incoming. This function only drops messages from incoming buffer
  // and only up to termination char.
  int ch;
  while (Serial.available() > 0) {
    ch = Serial.read();

    // flush only incomplete package
    if (ch == EOM) {
      break;
    }
  }
}

void set_buffer(byte *buf, byte b0, byte b1) {
  buf[0] = b0;
  buf[1] = b1;
}

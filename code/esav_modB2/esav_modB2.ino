// LEDs
const unsigned int red_led_pin = 7;
bool red_led = false;
const unsigned int green_led_pin = 6;
bool green_led = false;

// Remote ON/OFF button
const unsigned int remote_button_pin = 4;

// RELAY (SSR)
const unsigned int relay_pin = 5;

// application states
const unsigned int WAIT = 0;  // wait for first ping (relay is off)
const unsigned int WAIT_FOR_PING = 1;  // wait for ping (relay is on)
const unsigned int PONG = 2;  // answer ping with pong
const unsigned int STOP_REQUEST = 3;  // stop request came
const unsigned int STOP = 4;  // turn the relay off
const unsigned int STOP_RESPONSE = 5; // answer to the stop request
const unsigned int DEBUG = 6;  // turn relay on no matter what

// initial state
unsigned int state = WAIT;

// I/O
const byte EOM = 0x00;
const unsigned int msg_length = 2;
// msg format: 2B msg, 1B EOM

byte in_buffer[msg_length];
byte out_buffer[msg_length];

// counter for number of missed inbound messages
unsigned int no_data_counter = 0;
const unsigned int max_tries = 5;

void setup() {
  // input pins
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  
  // it may hang so let's pull it up (this inverts logic!)
  pinMode(remote_button_pin, INPUT_PULLUP);

  // start serial connection
  Serial.begin(9600);  // pins 0 and 1
  Serial.setTimeout(1000);

  // initialize pins with desired states
  led(red_led_pin, red_led);
  led(green_led_pin, green_led);
  relay(relay_pin, false);
}

void loop() {
  // reset buffers
  set_buffer(in_buffer, 0x00, 0x00);
  set_buffer(out_buffer, 0x00, 0x00);
  
  // handle debug mode
  if (!debug_on(remote_button_pin) && state == DEBUG) {
    state = WAIT;
  } else if (debug_on(remote_button_pin)) {
    state = DEBUG;
  }

  // wait for the first ping to come (with relay turned off)
  // relay: off
  // red led: on
  // green led: toggle every second
  if (state == WAIT) {
    relay(relay_pin, false);
    red_led = true; led(red_led_pin, red_led);

    if (Serial.available() >= msg_length) {
      // Serial.readBytes(in_buffer, msg_length);
      Serial.readBytesUntil(EOM, in_buffer, msg_length);

      if (is_ping(in_buffer)) {
        state = PONG;
      } else if (is_stop(in_buffer)) {
        state = STOP_REQUEST;
      } else {
        // something's wrong, that's not the correct message
      }

      serial_flush();
    }

    if (state == WAIT) {
      // don't delay if we're decided to go to PONG
      green_led = !green_led; led(green_led_pin, green_led);
      delay(990);
    }
  }

  // wait for cosequtive pings to come (with relay turned on)
  // relay: on
  // red led: off
  // green led: on
  else if (state == WAIT_FOR_PING) {
    relay(relay_pin, true);
    red_led = false; led(red_led_pin, red_led);
    green_led = true; led(green_led_pin, green_led);

    if (Serial.available() >= msg_length) {
      // Serial.readBytes(in_buffer, msg_length);
      Serial.readBytesUntil(EOM, in_buffer, msg_length);

      if (is_ping(in_buffer)) {
        state = PONG;
      } else if (is_stop(in_buffer)) {
        state = STOP_REQUEST;
      } else {
        // something's wrong, that's not the correct message
        delay(990);
      }

      serial_flush();

    } else {
      // no inbound msgs
    }
  }

  // answer PING with PONG
  // relay: keep
  // red led: keep
  // green led: keep
  else if (state == PONG) {
    // fake state, we don't need to do anything
    delay(1000);
    state = WAIT_FOR_PING;
  }

  // someone wants to turn the relay off
  // relay: keep
  // red led: keep
  // green led: off
  else if (state == STOP_REQUEST) {
    green_led = false; led(green_led_pin, green_led);

    state = STOP;
  }

  // turn the relay off
  // relay: off
  // red led: on
  // green led: off
  else if (state == STOP) {
    relay(relay_pin, false);
    red_led = true; led(red_led_pin, red_led);
    green_led = false; led(green_led_pin, green_led);
    
    state = STOP_RESPONSE;
    delay(100);
  }

  // answer to the stop request
  // relay: keep
  // red led: keep
  // green led: keep
  else if (state == STOP_RESPONSE) {
    // fake state, we don't need to do anything
    delay(100);
    state = STOP;
  }

  // enable the relay
  // relay: on
  // red led: quickly toggle
  // green led: quickly toggle with other phase than red led
  else if (state == DEBUG) {
    relay(relay_pin, true);
    red_led = !red_led; led(red_led_pin, red_led);
    green_led = !red_led; led(green_led_pin, green_led);

    state = DEBUG;
    serial_flush_all();
    delay(500);
  }

  else {
    // unknown state, let's stop
    state = STOP;
  }

}

void relay(const unsigned int pin, const bool state) {
  if (state)
    digitalWrite(pin, HIGH);
  else
    digitalWrite(pin, LOW);
}

void led(const unsigned int pin, const bool state) {
  if (state)
    digitalWrite(pin, HIGH);
  else
    digitalWrite(pin, LOW);
}

bool debug_on(const unsigned int pin) {
  // Decide if debug mode should be on based on pin state.
  return digitalRead(pin);
}

bool is_ping(byte *buf) {
  return buf[0] == 0xAA && buf[1] == 0x00;
}

bool is_stop(byte *buf) {
  return buf[0] == 0xFF && buf[1] == 0x00;
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

void serial_flush_all() {
  // Since Arduino 1.0, the Serial.flush() only removes outgoing data,
  // not incoming. This function all leftover chars in the incoming buffer.
  int ch;
  while (Serial.available() > 0) {
    ch = Serial.read();
  }
}

void set_buffer(byte *buf, byte b0, byte b1) {
  buf[0] = b0;
  buf[1] = b1;
}


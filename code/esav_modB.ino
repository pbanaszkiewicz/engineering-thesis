unsigned int counter = 0;
unsigned int no_data_counter = 0;

// LEDs
const unsigned int red_led_pin = 7;
bool red_led = false;
const unsigned int green_led_pin = 6;
bool green_led = false;

// relay
const unsigned int relay_pin = 13;

// application states
const unsigned int WAIT = 0;
const unsigned int RUN = 1;
const unsigned int STOP = 2;

// initial state
unsigned int state = WAIT;

struct package {
  byte emergency;
  unsigned int counter;
  byte stop;
};

// I/O
//String input;
const byte package_end = 0x00;
const unsigned int package_length = 4;

byte in_buffer[package_length] = {0x00, 0x00, 0x00, 0x00};
byte out_buffer[package_length] = {0x00, 0x00, 0x00, 0x00};

void setup() {
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);

  // start serial connection
  Serial.begin(9600);  // pins 0 and 1
  Serial.setTimeout(1000);

  digitalWrite(green_led_pin, green_led);
  disableRelay(relay_pin);
}

void loop() {
  if (state == WAIT) {
    // wait for the activation data (greetings) && blink red
    
    if (Serial.available()) {
      Serial.readBytesUntil(package_end, in_buffer, package_length);

      if (isConversationStart(in_buffer)) {
        // correctly started the conversation
        state = RUN;

        // correct response
        out_buffer[0] = 0x11;
        out_buffer[1] = 0xFF;
        out_buffer[2] = 0xFF;
        out_buffer[3] = 0x00;
        
        Serial.write(out_buffer, package_length);
        
      } else {
        // something's wrong, that's not the start of the conversation
      }
    }

    red_led = !red_led;
    digitalWrite(red_led_pin, red_led);
    delay(100);
    
  } else if (state == RUN) {
    // running

    green_led = true;
    red_led = false;
    digitalWrite(green_led_pin, green_led);
    digitalWrite(red_led_pin, red_led);
    enableRelay(relay_pin);
    
    if (Serial.available()) {
      no_data_counter = 0;
      ++counter;
      
      Serial.readBytes(in_buffer, package_length);

      if (readEmergencyByte(in_buffer) == 0xFF) {
        state = STOP;
      } else if (counter != readCounter(in_buffer)) {
        state = STOP;
      } else {
        delay(1000);
      }
      
    } else {
      // no response, therefore we should stop
      
      if (no_data_counter == 3) {
        state = STOP;
      } else {
        delay(1000);
        ++no_data_counter;
      }
    }
    
    
  } else if (state == STOP) {
    disableRelay(relay_pin);
    green_led = false;
    red_led = true;
    digitalWrite(green_led_pin, green_led);
    digitalWrite(red_led_pin, red_led);
    
  } else {
    // unknown state, let's stop
    state = STOP;
  }
}

void disableRelay(const int pin) {
  digitalWrite(pin, LOW);
}

void enableRelay(const int pin) {
  digitalWrite(pin, HIGH);
}

byte readEmergencyByte(byte* buf) {
  return buf[0];
}

unsigned int readCounter(byte* buf) {
  return buf[1] * 256 + buf[2];
}

byte readStopByte(byte* buf) {
  return buf[3];
}

bool isConversationStart(byte* buf) {
  return buf[0] == 0xFF && buf[1] == 0xFF && buf[2] == 0xFF && buf[3] == 0x00;
}

unsigned int counter = 0;

// LEDs
const unsigned int green_led_pin = 7;
bool green_led = false;

const unsigned int reset_pin = 2;

// application states
const unsigned int CONNECTING = 0;
const unsigned int PING = 1;
const unsigned int STOP = 2;

// initial state
unsigned int state = CONNECTING;

// I/O
const byte package_end = 0x00;
const unsigned int package_length = 4;

byte in_buffer[package_length] = {0x00, 0x00, 0x00, 0x00};
byte out_buffer[package_length] = {0x00, 0x00, 0x00, 0x00};

// for handling emergency interrupt
volatile bool emergency = false;

void setup() {
  pinMode(green_led_pin, OUTPUT);
  pinMode(reset_pin, INPUT);
  
  Serial.begin(9600);  // pins 0 and 1
  Serial.setTimeout(1000);
  
  attachInterrupt(digitalPinToInterrupt(reset_pin), emergencyInterrupt, RISING);
}

void loop() {
  if (state == CONNECTING) {
    // try to initialize the conversation
    
    if (Serial.available()) {
      // receive RSVP
      Serial.readBytesUntil(package_end, in_buffer, package_length);

      if (isConversationRSVP(in_buffer)) {
        state = PING;
        counter = 0;
      }
    } else {
      // send greetings
      out_buffer[0] = 0xFF;
      out_buffer[1] = 0xFF;
      out_buffer[2] = 0xFF;
      out_buffer[3] = 0x00;
      
      Serial.write(out_buffer, package_length);
      
      green_led = !green_led;
      digitalWrite(green_led_pin, green_led);
      delay(1000);
      
    }
    
  } else if (state == PING) {
    ++counter;

    if (emergency) {
      state = STOP;
    } else {
      out_buffer[0] = 0x22;

      green_led = true;
      digitalWrite(green_led_pin, green_led);
      
      writeCounter(counter, out_buffer);
      Serial.write(out_buffer, package_length);
      
      green_led = false;
      digitalWrite(green_led_pin, green_led);
  
      delay(1000);
    }
    
  } else if (state == STOP) {
    setEmergency(out_buffer);
    Serial.write(out_buffer, package_length);
    
    delay(500); // run this faster, for safety sake
    
  } else {
    // unknown state
    state = STOP;
  }
}

bool isConversationRSVP(byte* buf) {
  return buf[0] == 0x11 && buf[1] == 0xFF && buf[2] == 0xFF && buf[3] == 0x00;
}

void writeCounter(unsigned int counter, byte* buf) {
  buf[1] = highByte(counter);
  buf[2] = lowByte(counter);
}

void setEmergency(byte* buf) {
  buf[0] = 0xFF;
}

void emergencyInterrupt() {
  emergency = true;
}


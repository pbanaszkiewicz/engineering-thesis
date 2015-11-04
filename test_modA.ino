int counter = 0;

// LEDs
const int green_led_pin = 7;
bool green_led = false;

// application states
const int CONNECTING = 0;
const int PING = 1;
const int STOP = 2;

// initial state
int state = CONNECTING;

// useful input string variable
String input;

void setup() {
  pinMode(green_led_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (state == CONNECTING) {
    // try to initialize the conversation
    
    if (!Serial.available()) {
      // send greetings
      Serial.print("Start ");
      
      green_led = !green_led;
      digitalWrite(green_led_pin, green_led);
      delay(1000);
      
    } else {
      // receive RSVP
      input = Serial.readStringUntil(' ');

      if (input == "Started") {
        state = PING;
      }
    }
  } else if (state == PING) {
    
  }
}

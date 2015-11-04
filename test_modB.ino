int counter = 0;

// LEDs
const int red_led_pin = 7;
bool red_led = false;
const int green_led_pin = 6;
bool green_led = false;

// application states
const int WAIT = 0;
const int RUN = 1;
const int STOP = 2;

// initial state
int state = WAIT;

// useful input string variable
String input;

void setup() {
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
  pinMode(13, OUTPUT);

  // start serial connection
  Serial1.begin(9600);  // pins 0 and 1
  Serial.begin(9600);

  Serial1.setTimeout(1000);
}

void loop() {
  if (state == WAIT) {
    // wait for the activation data (greetings) && blink red
    
    if (Serial1.available()) {
      input = Serial1.readStringUntil(' ');
    }

    if (input == "Start") {
      input = "";
      state = RUN;
      Serial1.print("Started ");
    } else {
      Serial.write(input.c_str());
      input = "";
      blink(red_led_pin);
    }
    
  } else if (state == RUN) {
    // running
    
    if (Serial1.available()) {
      input = Serial1.readStringUntil(' ');
    }
    
    green_led = !green_led;
    digitalWrite(green_led_pin, green_led);
    delay(1000);
  }
}

void blink(int pin) {
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
  delay(100);
}


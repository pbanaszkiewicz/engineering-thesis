//#include <SoftwareSerial.h>
//SoftwareSerial antenna(2, 3);

int counter = 0;
const int led_pin = 13;
bool led_status = true;

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial1.begin(9600);  // pins 0 and 1
  Serial1.println("Starting conversation");
  Serial.begin(9600);
  Serial.println("Starting conversation with computer");
  //antenna.begin(9600);
  //antenna.println("Starting conversation");
}

void loop() {
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  //Serial1.println("Hello");
  //antenna.println("Hello");
  //if (Serial.available()) {
  //Serial.println(Serial1.readStringUntil('\n'));
  //Serial.write(Serial1.read());
  //Serial.write(antenna.read());
  //}
  led_status = !led_status;
  digitalWrite(led_pin, led_status);
  delay(100);
//  antenna.write(counter++);
//    
//  led_status = !led_status;
//  digitalWrite(led_pin, led_status);
//  
//  delay(1000);
}

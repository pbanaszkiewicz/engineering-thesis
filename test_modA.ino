//#include <SoftwareSerial.h>
//SoftwareSerial antenna(2, 3);

int counter = 0;
const int led_pin = 13;
bool led_status = true;

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
  
  Serial.println("Starting conversation");
  //antenna.begin(9600);
}

void loop() {
//  if (Serial.available()) {
//    Serial.read();
//    Serial.println("Hello");
//    led_status = !led_status;
//    digitalWrite(led_pin, led_status);
//  }
  Serial.println("Hello");
  led_status = !led_status;
  digitalWrite(led_pin, led_status);
  delay(1000);
//  if (antenna.available()) {
//    Serial.print(antenna.read());
//    //counter = Serial.parseInt();
//    
//    led_status = !led_status;
//    digitalWrite(led_pin, led_status);
//  }
//  
//  if (counter % 10 == 0) {
//    Serial.println("test message");
//  }
//  
//  delay(1000);
//  
//  counter++;
}

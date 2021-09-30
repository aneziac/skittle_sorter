/*
Blinks LED in an SOS pattern
Implementation of https://www.youtube.com/watch?v=CfdaJ4z4u4w&t=1586s

Connect pin 8 to a 200-400 ohm resistor and the resistor to longer end of LED
Connect shorter end of LED to ground
*/


const int ledPin = 8;
const int dot = 100;
const int dash = 500;
const int inBetween = 300;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  for (int i = 0; i < 3; i++) {
      digitalWrite(ledPin, HIGH);
      delay(dot);
      digitalWrite(ledPin, LOW);
      delay(dot);
  }
  delay(inBetween);
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPin, HIGH);
      delay(dash);
      digitalWrite(ledPin, LOW);
      delay(dash);
  }
  delay(inBetween);
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPin, HIGH);
      delay(dot);
      digitalWrite(ledPin, LOW);
      delay(dot);
  }
  delay(1000);
}

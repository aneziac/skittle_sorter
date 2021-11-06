#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>

Servo myservo;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_540MS, TCS34725_GAIN_1X);
const uint16_t red_max = 546, green_max = 816, blue_max = 536;

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

void setup() {
  myservo.attach(9);
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Connected to sensor.");
  } else {
    Serial.println("Sensor not found.");
    while (1);
  }

  myservo.write(30);
  delay(1000);
  myservo.write(180);
  delay(5000);
  myservo.write(94);

  // myservo.write(30);
}

void loop() {
  uint16_t clear, red, green, blue;
  uint16_t n_red, n_green, n_blue;
  bool calibrating = true;

  tcs.getRawData(&red, &green, &blue, &clear);

  n_red = int(255.0 * red / red_max);
  n_green = int(255.0 * green / green_max);
  n_blue = int(255.0 * blue / blue_max);

  if (calibrating) {
    Serial.print("R\t"); Serial.print(n_red);
    Serial.print("\tG\t"); Serial.print(n_green);
    Serial.print("\tB\t"); Serial.print(n_blue);
    Serial.println();
  } else {
    if (n_blue > 265 && n_green > 195) {
      Serial.print("Skittle is purple\n");
    } else if (n_red > 220 && n_green > 180) {
      Serial.print("Skittle is yellow\n");
    } else if (n_red > 210) {
      Serial.print("Skittle is orange\n");
    } else if (n_green > 175 && n_blue > 240) {
      Serial.print("Skittle is green\n");
    } else {
      Serial.print("Skittle is red\n");
    }
  }

}

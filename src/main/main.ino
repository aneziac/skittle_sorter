#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>

/*
Color sensor connections:

  Connect SCL    to analog 5
  Connect SDA    to analog 4
  Connect VDD    to 3.3V DC
  Connect GROUND to common ground

Top servo motor connections:

  Connect black wire to GND
  Connect red wire to 5V
  Connect white wire to digital 9

Bottom servo motor connections:

  Connect black wire to GND
  Connect red wire to 5V
  Connect yellow wire to digital 10
*/

// constants
const Servo top_servo, bottom_servo;
const Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_540MS, TCS34725_GAIN_1X);

const uint16_t red_max = 546, green_max = 816, blue_max = 536;
const double scaled_max = 255.0;

const uint16_t drop_angle = 180, sense_angle = 94, sort_angle = 30;
const uint16_t purple_angle = 0, yellow_angle = 0, red_angle = 0, blue_angle = 0, green_angle = 0;

bool calibrating = true;

// receive and normalize RGB values so they fall into the closed interval [0, scaled_max]
void get_norm_rgb(uint16_t &n_red, uint16_t &n_green, uint16_t &n_blue) {
  uint16_t clear, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &clear);
  delay(541);  // wait for integration to complete

  n_red = int(scaled_max * red / red_max);
  n_green = int(scaled_max * blue / green_max);
  n_blue = int(scaled_max * green / blue_max);
}

void setup() {
  top_servo.attach(9);
  bottom_servo.attach(10);
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Connected to sensor.");
  } else {
    Serial.println("Sensor not found.");
    while (1);
  }

  top_servo.write(drop_angle); // Return to default state
  delay(2000);
}

void loop() {
  uint16_t n_red, n_green, n_blue;

  if (calibrating) {
    for (int i = 0; i < 10; i++) {
      top_servo.write(sense_angle - 5 + i);
      get_norm_rgb(n_red, n_green, n_blue);

      Serial.print("R\t"); Serial.print(n_red);
      Serial.print("\tG\t"); Serial.print(n_green);
      Serial.print("\tB\t"); Serial.print(n_blue);
      Serial.println();

      delay(1000);
      top_servo.write(drop_angle);
    }

    top_servo.write(drop_angle);
    return 0;
  }

  top_servo.write(sense_angle)
  get_norm_rgb(n_red, n_green, n_blue);

  /*
  if (n_blue > 265 && n_green > 195) {
    Serial.print("Skittle is purple\n");
    bottom_servo.write(purple_angle);
  } else if (n_red > 220 && n_green > 180) {
    Serial.print("Skittle is yellow\n");
    bottom_servo.write(yellow_angle);
  } else if (n_red > 210) {
    Serial.print("Skittle is orange\n");
    bottom_servo.write(orange_angle);
  } else if (n_green > 175 && n_blue > 240) {
    Serial.print("Skittle is green\n");
    bottom_servo.write(green_angle);
  } else {
    Serial.print("Skittle is red\n");
    bottom_servo.write(red_angle);
  }
  */

  delay(2000);
  top_servo.write(sort_angle);
  delay(1000);
  top_servo.write(drop_angle);
  delay(5000);
}

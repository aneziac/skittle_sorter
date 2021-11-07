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

struct {  // top servo angles
  const uint16_t drop = 180, sense = 94, sort = 30;
} top_servo_angle;
struct {
  const uint16_t purple = 0, yellow = 0, red = 0, blue = 0, green = 0;
} bottom_servo_angle;

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

  top_servo.write(top_servo_angle.drop); // Return to default state
  delay(2000);
}

void loop() {
  uint16_t n_red, n_green, n_blue;

  if (calibrating) {
    for (int i = 0; i < 7; i++) {
      top_servo.write(top_servo_angle.sense - 3 + i);
      get_norm_rgb(n_red, n_green, n_blue);

      Serial.print("R\t"); Serial.print(n_red);
      Serial.print("\tG\t"); Serial.print(n_green);
      Serial.print("\tB\t"); Serial.print(n_blue);
      Serial.println();

      delay(1000);
      top_servo.write(top_servo_angle.drop);
    }

    top_servo.write(top_servo_angle.drop);
    return 0;
  }

  top_servo.write(top_servo_angle.sense);
  get_norm_rgb(n_red, n_green, n_blue);

  /*
  if (n_blue > 265 && n_green > 195) {
    Serial.print("Skittle is purple\n");
    bottom_servo.write(bottom_servo_angle.purple);
  } else if (n_red > 220 && n_green > 180) {
    Serial.print("Skittle is yellow\n");
    bottom_servo.write(bottom_servo_angle.yellow);
  } else if (n_red > 210) {
    Serial.print("Skittle is orange\n");
    bottom_servo.write(bottom_servo_angle.orange);
  } else if (n_green > 175 && n_blue > 240) {
    Serial.print("Skittle is green\n");
    bottom_servo.write(bottom_servo_angle.green);
  } else {
    Serial.print("Skittle is red\n");
    bottom_servo.write(bottom_servo_angle.red);
  }
  */

  delay(2000);
  top_servo.write(top_servo_angle.sort);
  delay(1000);
  top_servo.write(top_servo_angle.drop);
  delay(5000);
}

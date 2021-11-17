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

Slide servo motor connections:

  Connect black wire to GND
  Connect red wire to 5V
  Connect yellow wire to digital 10
*/

// define integers associated with each skittle color
#define RED 1
#define ORANGE 2
#define YELLOW 3
#define GREEN 4
#define PURPLE 5

// servo and color sensor
Servo top_servo, slide_servo;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_540MS, TCS34725_GAIN_1X);

// constants
const uint16_t red_max = 546, green_max = 816, blue_max = 536;
const double scaled_max = 255.0;

struct {  // top servo angles
  const uint16_t drop = 180, sense = 94, sort = 30;
} top_servo_angle;

bool calibrating = true;

int determine_color(uint16_t r, uint16_t g, uint16_t b) {

}

// receive and normalize RGB values so they fall into the closed interval [0, scaled_max]
void get_norm_rgb(uint16_t &n_red, uint16_t &n_green, uint16_t &n_blue) {
  uint16_t clear, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &clear);
  delay(541);  // wait for integration to complete

  n_red = int(scaled_max * red / red_max);
  n_green = int(scaled_max * blue / green_max);
  n_blue = int(scaled_max * green / blue_max);
}

// change position of slide servo
void slidepos(int x)
{
  if (x == RED)
  {
    slide_servo.write(160);
  }
  else if(x == ORANGE)
  {
    slide_servo.write(140);
  }
  else if(x == YELLOW)
  {
    slide_servo.write(100);
  }
  else if(x == GREEN)
  {
    slide_servo.write(50);
  }
  else if(x == PURPLE)
  {
    slide_servo.write(30);
  }
}

void setup() {
  top_servo.attach(9);
  slide_servo.attach(10);
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Connected to sensor.");
  } else {
    Serial.println("Sensor not found.");
    while (1);
  }

  top_servo.write(top_servo_angle.drop); // Return to default state
  delay(4000);
}

void loop() {
  uint16_t n_red, n_green, n_blue;

  if (calibrating) {
    for (int i = 0; i < 7; i++) {
      top_servo.write(top_servo_angle.sense - 3 + i);
      delay(1000);
      get_norm_rgb(n_red, n_green, n_blue);

      Serial.print("R\t"); Serial.print(n_red);
      Serial.print("\tG\t"); Serial.print(n_green);
      Serial.print("\tB\t"); Serial.print(n_blue);
      Serial.println();

      top_servo.write(top_servo_angle.drop);
      delay(1000);
    }

    top_servo.write(top_servo_angle.sort);
    delay(1000);
    exit(0);
  }

  top_servo.write(top_servo_angle.sense);
  get_norm_rgb(n_red, n_green, n_blue);

  int color = determine_color(n_red, n_green, n_blue);
  slidepos(color);

  delay(2000);
  top_servo.write(top_servo_angle.sort);
  delay(1000);
  top_servo.write(top_servo_angle.drop);
  delay(5000);
}

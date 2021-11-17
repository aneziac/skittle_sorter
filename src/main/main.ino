#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>
#include <Arduino_KNN.h>

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
#define NONE 0
#define GREEN 1
#define RED 2
#define PURPLE 3
#define ORANGE 4
#define YELLOW 5

// servo, color sensor, and classifier
Servo top_servo, slide_servo;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_540MS, TCS34725_GAIN_1X);
KNNClassifier color_KNN(3);

// constants
const uint16_t red_max = 546, green_max = 816, blue_max = 536;
const double scaled_max = 255.0;
const float calib_array[6][16][3] = {
  {
    {213, 172, 293},
    {188, 142, 249},
    {205, 162, 279},
    {183, 137, 241},
    {203, 161, 277},
    {214, 173, 295},
    {226, 183, 310},
    {197, 150, 261},
    {217, 173, 295},
    {224, 182, 309},
    {192, 145, 254},
    {217, 173, 297},
    {230, 184, 314},
    {200, 147, 262},
    {224, 176, 304},
    {237, 190, 326}
  },
  {
    {191, 146, 304},
    {177, 139, 268},
    {196, 157, 298},
    {187, 137, 263},
    {193, 151, 287},
    {194, 159, 297},
    {181, 133, 255},
    {195, 152, 296},
    {205, 166, 321},
    {196, 157, 293},
    {182, 135, 259},
    {197, 153, 293},
    {194, 157, 294},
    {183, 134, 255},
    {198, 154, 297},
    {200, 162, 315}
  },
  {
    {190, 131, 218},
    {181, 119, 207},
    {179, 132, 223},
    {182, 134, 223},
    {172, 118, 206},
    {179, 126, 214},
    {184, 137, 228},
    {208, 158, 264},
    {186, 131, 227},
    {229, 185, 315},
    {195, 146, 257},
    {198, 145, 243},
    {200, 153, 256},
    {180, 128, 224},
    {203, 147, 247},
    {208, 163, 270}
  },
  {
    {159, 126, 216},
    {148, 113, 197},
    {152, 125, 211},
    {152, 126, 213},
    {157, 119, 208},
    {158, 129, 220},
    {160, 138, 231},
    {170, 145, 242},
    {159, 126, 216},
    {167, 139, 233},
    {177, 149, 250},
    {173, 130, 229},
    {177, 144, 245},
    {169, 127, 224},
    {178, 146, 248},
    {179, 154, 258}
  },
  {
    {281, 132, 242},
    {230, 123, 226},
    {250, 136, 245},
    {281, 132, 242},
    {230, 123, 226},
    {250, 136, 245},
    {264, 137, 247},
    {213, 120, 219},
    {233, 138, 243},
    {258, 155, 271},
    {224, 135, 242},
    {253, 150, 263},
    {257, 155, 269},
    {217, 135, 242},
    {244, 154, 269},
    {257, 168, 290}
  },
  {
    {328, 156, 371},
    {218, 131, 264},
    {247, 143, 296},
    {291, 142, 330},
    {226, 128, 268},
    {254, 142, 299},
    {255, 148, 306},
    {308, 160, 353},
    {225, 137, 271},
    {284, 176, 350},
    {266, 161, 323},
    {230, 138, 276},
    {259, 158, 315},
    {275, 161, 329},
    {255, 159, 312},
    {281, 175, 346}
  }
};

struct {  // top servo angles
  const uint16_t drop = 180, sense = 94, sort = 30;
} top_servo_angle;

bool calibrating = true;

void read_calibration_data() {
  for (int i = 0; i < 6 * 21; i++) {
    color_KNN.addExample(calib_array[i / 6][i % 21], i / 6);
  }
}

// uses K-nearest neighbors library: see https://github.com/arduino-libraries/Arduino_KNN
int determine_color(uint16_t r, uint16_t g, uint16_t b) {
  const float color_vec[3] = {float(r), float(g), float(b)};  // must be casted to floats to comply with KNN library
  return color_KNN.classify(color_vec, 5);  // classify with k = 5
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
  switch (x) {
    case RED:
      slide_servo.write(160);
      break;

    case ORANGE:
      slide_servo.write(140);
      break;

    case YELLOW:
      slide_servo.write(100);
      break;

    case GREEN:
      slide_servo.write(50);
      break;

    case PURPLE:
      slide_servo.write(30);
      break;

    case NONE:
      Serial.println("No skittle detected");
  }
}

void setup() {
  top_servo.attach(9);
  slide_servo.attach(10);
  Serial.begin(9600);

  read_calibration_data();

  if (tcs.begin()) {
    Serial.println("Connected to sensor.");
  } else {
    Serial.println("Sensor not found.");
    while (1);  // infinite loop effectively ends program
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
    while (1);
  }

  top_servo.write(top_servo_angle.sense);
  get_norm_rgb(n_red, n_green, n_blue);

  int skittle_color = determine_color(n_red, n_green, n_blue);
  slidepos(skittle_color);

  delay(2000);
  top_servo.write(top_servo_angle.sort);
  delay(1000);
  top_servo.write(top_servo_angle.drop);
  delay(5000);
}

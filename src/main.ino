#include <Wire.h>
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
const uint16_t red_max = 1072, green_max = 1633, blue_max = 1607;


void setup() {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Connected to sensor.")
  } else {
    Serial.println("Sensor not found.")
    while (1);
  }
}

void loop() {
  uint16_t clear, red, green, blue;
  uint16_t n_red, n_green, n_blue;

  tcs.getRawData(&red, &green, &blue, &clear);

  n_red = int(255.0 * red / red_max);
  n_green = int(255.0 * green / green_max);
  n_blue = int(255.0 * blue / blue_max);

  Serial.print("R\t"); Serial.print(n_red);
  Serial.print("\tG\t"); Serial.print(n_green);
  Serial.print("\tB\t"); Serial.print(n_blue);
  Serial.println()
}

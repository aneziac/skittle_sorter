#include <Wire.h>
#include "Adafruit_TCS34725.h"


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_540MS, TCS34725_GAIN_1X);
const uint16_t red_max = 1072, green_max = 1633, blue_max = 1607;

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */
void slidepos(int x)
{
  if (x == 1)
  {
    slideservo.write(160);
  }
  else if(x == 2)
  {
    slideservo.write(140);
  }
  else if(x == 3)
  {
    slideservo.write(100);
  }
  else if(x == 4)
  {
    slideservo.write(50);
  }
  else if(x == 5)
  {
    slideservo.write(30);
  }
}
void setup() {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Connected to sensor.");
  } else {
    Serial.println("Sensor not found.");
    while (1);
  }
}

void loop() {
  uint16_t clear, red, green, blue;
  uint16_t n_red, n_green, n_blue;
  bool calibrating = false;

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
    if (n_red > 480 && n_green > 350) {
      Serial.print("Skittle is yellow\n");
    } else if (n_red > 400) {
      Serial.print("Skittle is orange\n");
    } else if (n_green > 280) {
      Serial.print("Skittle is green\n");
    } else if (n_green > 250) {
      Serial.print("There is no skittle\n");
    } else if (n_red > 265) {
      Serial.print("Skittle is red\n");
    } else {
      Serial.print("Skittle is purple\n");
    }
  }

}

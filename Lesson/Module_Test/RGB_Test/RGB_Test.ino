#include "RGBLed.h"
RGBLed rgbled(A1, 2);
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  rgbled.setColor(1, 255, 0, 0);
  rgbled.setColor(2, 255, 0, 0);
  rgbled.show();
  delay(1000);
  rgbled.setColor(1, 0, 255, 0);
  rgbled.setColor(2, 0, 255, 0);
  rgbled.show();
  delay(1000);
  rgbled.setColor(1, 0, 0, 255);
  rgbled.setColor(2, 0, 0, 255);
  rgbled.show();
  delay(1000);
}

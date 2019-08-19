#include "RGBLed.h"
RGBLed rgbled(A1, 2);
void setup()
{
  rgbled.setColor(1, RGB_RED);
  rgbled.setColor(2, RGB_WHITE);
  rgbled.show();
  delay(1000);
}

void loop()
{
  rgbled.setColor(1, 0, 255, 0);
  rgbled.setColor(2, 0, 0, 255);
  rgbled.show();
}

void setup()
{
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
}

void loop()
{
  for (int i = 200; i <= 800; i++)  // 200HZ ~ 800HZ
  {
    tone(A0, i);
  }
  delay(1000);                    //Max Frequency hold 1s
  for (int i = 800; i >= 200; i--) // 800HZ ~ 200HZ
  {
    tone(A0, i);
    delay(10);
  }
}

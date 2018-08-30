int left1,centre,right1,left2,right2;
void setup()
{
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
}

void loop()
{
	int left1,centre,right1,left2,right2;
	left1=analogRead(A0);
	left2=analogRead(A1);
	center=analogRead(A2);
	right1=analogRead(A4);
	right2=analogRead(A5);
	Serial.print("The left1 is:");
	Serial.print(left1);
	Serial.print("     ");
	Serial.print("The left2 is:");
	Serial.print(left2);
	Serial.print("     ");
	Serial.print("The center is:");
	Serial.print(centre);
	Serial.print("     ");
	Serial.print("The right1 is:");
	Serial.print(right1);
	Serial.print("     ");
	Serial.print("The right2 is:");
	Serial.println(right2);
	delay(100);
}

int motor_L2 = 4; //Bin1
int motor_L1 = 2; //Bin2
int PWMB = 6; //PWMB
int STBY = 7; //STBY

void setup()
{
    Serial.begin(9600);
    pinMode(STBY, OUTPUT);
    pinMode(motor_L1, OUTPUT);
    pinMode(motor_L2, OUTPUT);
}

void loop()
{
    digitalWrite(motor_L2,HIGH);
    digitalWrite(motor_L1, LOW); 
    digitalWrite(STBY,HIGH); 
    analogWrite(PWMB,255);
    delay(2000);
    digitalWrite(motor_L2,LOW);
    digitalWrite(motor_L1, HIGH); 
    digitalWrite(STBY,HIGH); 
    analogWrite(PWMB,255);
    delay(2000);
    digitalWrite(motor_L2,LOW);
    digitalWrite(motor_L1, HIGH); 
    digitalWrite(STBY,HIGH); 
    analogWrite(PWMB,0);
    delay(2000);
}






#define EM_MOTOR_SHIELD_BOARD_VERSION 3

#define AIN1 3
#define BIN1 4
#define PWMA 5
#define PWMB 6
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
#define AIN2 11
#define BIN2 2
#define STBY 7
#endif

void setup()
{
    Serial.begin(9600);
    pinMode(AIN1, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
    pinMode(AIN2, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(STBY, OUTPUT);
#endif
}

void loop()
{
    digitalWrite(AIN1, HIGH);
    digitalWrite(BIN1, HIGH); 
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
    digitalWrite(AIN2, LOW); 
    digitalWrite(BIN2, LOW); 
    digitalWrite(STBY, HIGH); 
#endif
    analogWrite(PWMA,255);
    analogWrite(PWMB,255);
    delay(2000);
    digitalWrite(AIN1, LOW);
    digitalWrite(BIN1, LOW); 
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
    digitalWrite(AIN2, HIGH); 
    digitalWrite(BIN2, HIGH); 
    digitalWrite(STBY, HIGH); 
#endif
    analogWrite(PWMA,255);
    analogWrite(PWMB,255);
    delay(2000);

}






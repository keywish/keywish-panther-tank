#define EM_MOTOR_SHIELD_BOARD_VERSION 3

#if (EM_MOTOR_SHIELD_BOARD_VERSION > 2)
    #define BUZZER_PIN 10
#else
    #define BUZZER_PIN 9
#endif

void setup()
{  
    pinMode(9,OUTPUT);  
}  
   
void loop()  
{  
    for(int i = 200; i <= 800; i++)   // 200HZ ~ 800HZ  
    {  
        tone(9,i);
    }  
    delay(1000);                    //Max Frequency hold 1s   
    for(int i= 800; i >= 200; i--)   // 800HZ ~ 200HZ
    {  
        tone(9,i);  
        delay(10);  
    }  
}

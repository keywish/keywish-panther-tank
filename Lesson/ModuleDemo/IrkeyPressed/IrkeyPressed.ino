#include "IR_remote.h"

#define EM_MOTOR_SHIELD_BOARD_VERSION 3

#if (EM_MOTOR_SHIELD_BOARD_VERSION > 2)
    #define IR_PIN 12
#else
    #define IR_PIN 8
#endif

IRremote ir(IR_PIN);
unsigned char keycode;
char str[128];
void setup()
{
    Serial.begin(9600);
    ir.begin();
}

void loop()
{
    if (keycode = ir.getCode()) {
        String key_name = ir.getKeyMap(keycode);
        sprintf(str, "Get ir code: 0x%x key name: %s \n", keycode, (char *)key_name.c_str());
        Serial.println(str);
    } else {
       //   Serial.println("no key");
    }
    delay(110);
}

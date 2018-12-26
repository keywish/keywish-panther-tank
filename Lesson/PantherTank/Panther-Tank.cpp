#include "Panther-Tank.h"
#include "ProtocolParser.h"
#include "Sounds.h"
#include "debug.h"

//MotorShieldBoard V3.0
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
Tank::Tank(ProtocolParser *Package, uint8_t ain1, uint8_t ain2, uint8_t pwma, uint8_t bin1, uint8_t bin2, uint8_t pwmb, uint8_t standby): SmartCar("Tank", E_PANTHER_TANK, 0x01, E_BLUETOOTH_CONTROL)
#else
Tank::Tank(ProtocolParser * Package, uint8_t ain1, uint8_t bin1, uint8_t pwma, uint8_t pwmb): SmartCar("Tank", E_PANTHER_TANK, 0x01, E_BLUETOOTH_CONTROL)
#endif
{
  this->Ain1Pin = ain1;
  this->PwmaPin = pwma;
  this->Bin1Pin = bin1;
  this->PwmbPin = pwmb;
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
  this->Ain2Pin = ain2;
  this->Bin2Pin = bin2;
  this->StandbyPin = standby;
#endif
  SetStatus(E_STOP);
  mProtocolPackage = Package;
}

Tank::~Tank()
{
  delete mIrRecv;
  delete mBuzzer;
  delete mPs2x;
  delete mRgb;
  delete mUltrasonic;
  delete Mirf;
}

void Tank::init(void)
{
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
  pinMode(Ain2Pin, OUTPUT);
  digitalWrite(Ain2Pin, LOW);
  pinMode(Bin2Pin, OUTPUT);
  digitalWrite(Bin2Pin, LOW);
  pinMode(StandbyPin, OUTPUT);
  digitalWrite(StandbyPin, LOW);
#endif
  //keep TB6612 AIN stop
  pinMode(Ain1Pin, OUTPUT);
  digitalWrite(Ain1Pin, LOW);
  pinMode(PwmaPin, OUTPUT);
  digitalWrite(PwmaPin, LOW);
  //keep TB6612 BIN stop
  pinMode(Bin1Pin, OUTPUT);
  digitalWrite(Bin1Pin, LOW);
  pinMode(PwmbPin, OUTPUT);
  digitalWrite(PwmbPin, LOW);
}

void Tank::GoForward(void)
{
  int value = (Speed / 10) * 25.5;
  DEBUG_LOG(DEBUG_LEVEL_INFO, "GoForward\n");
  SetStatus(E_FORWARD);
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
  digitalWrite(Ain1Pin, HIGH);
  digitalWrite(Ain2Pin, LOW);
  digitalWrite(Bin1Pin, HIGH);
  digitalWrite(Bin2Pin, LOW);
  Serial.println("123");
#else
  digitalWrite(Ain1Pin, HIGH);
  digitalWrite(Bin1Pin, LOW);
#endif 
  analogWrite(PwmaPin, value);
  analogWrite(PwmbPin, value);
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
  digitalWrite(StandbyPin, HIGH);
#endif
}

void Tank::GoBack(void)
{
  int value = (Speed / 10) * 25.5;
  DEBUG_LOG(DEBUG_LEVEL_INFO, "GoBack\n");
  SetStatus(E_BACK);  
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
  digitalWrite(Ain1Pin, LOW);
  digitalWrite(Ain2Pin, HIGH);
  digitalWrite(Bin1Pin, LOW);
  digitalWrite(Bin2Pin, HIGH);
#else
  digitalWrite(Ain1Pin, LOW);
  digitalWrite(Bin1Pin, HIGH);
#endif 
  analogWrite(PwmaPin, value);
  analogWrite(PwmbPin, value);
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
  digitalWrite(StandbyPin, HIGH);
#endif
}

void Tank::KeepStop(void)
{
  DEBUG_LOG(DEBUG_LEVEL_INFO, "KeepStop\n");
  SetStatus(E_STOP);
  digitalWrite(Ain1Pin, LOW);
  analogWrite(PwmaPin, 0);
  digitalWrite(Bin1Pin, LOW);
  analogWrite(PwmbPin, 0);
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
  digitalWrite(Ain2Pin, HIGH);
  digitalWrite(Bin2Pin, HIGH);
  digitalWrite(StandbyPin, HIGH);
#endif
}

void Tank::TurnLeft(void)
{
  int value = (Speed / 10) * 25;   //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
  DEBUG_LOG(DEBUG_LEVEL_INFO, "TurnLeft\n");
  SetStatus(E_LEFT);
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
  digitalWrite(Ain1Pin, HIGH);
  digitalWrite(Ain2Pin, LOW);
  digitalWrite(Bin1Pin, LOW);
  digitalWrite(Bin2Pin, HIGH);
#else
  digitalWrite(Ain1Pin, HIGH);
  digitalWrite(Bin1Pin, HIGH);
#endif
  analogWrite(PwmaPin, value);
  analogWrite(PwmbPin, value);
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
  digitalWrite(StandbyPin, HIGH);
#endif
}

void Tank::TurnRight(void)
{
  int value = (Speed / 10) * 25;   //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
  DEBUG_LOG(DEBUG_LEVEL_INFO, "TurnRight\n");
  SetStatus(E_RIGHT);
  
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
  digitalWrite(Ain1Pin, LOW);
  digitalWrite(Ain2Pin, HIGH);
  digitalWrite(Bin1Pin, HIGH);
  digitalWrite(Bin2Pin, LOW);
#else
  digitalWrite(Ain1Pin, LOW);
  digitalWrite(Bin1Pin, LOW);
#endif
  analogWrite(PwmaPin, value);
  analogWrite(PwmbPin, value);
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
  digitalWrite(StandbyPin, HIGH);
#endif

}

void Tank::Drive(void)
{
  Drive(Degree);
}

void Tank::Drive(int degree)
{
  DEBUG_LOG(DEBUG_LEVEL_INFO, "degree = %d speed = %d\n", degree, Speed);
  int value = (Speed / 10) * 25.5;	 //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
  float f;
  if (degree >= 0 && degree <= 90) {
    f = (float)(degree) / 90;
    if (degree <=5) {
        SetStatus(E_RIGHT);
    } else  if (degree >= 85) {
        SetStatus(E_FORWARD);
    }
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
    digitalWrite(Ain1Pin, HIGH);
    digitalWrite(Ain2Pin, LOW);
    digitalWrite(Bin1Pin, HIGH);
    digitalWrite(Bin2Pin, LOW);
#else
    digitalWrite(Ain1Pin, HIGH);
    digitalWrite(Bin1Pin, LOW);
#endif
    analogWrite(PwmaPin,(float)(value * f));
    analogWrite(PwmbPin, value);
  } else if (degree > 90 && degree <= 180) {
    f = (float)(180 - degree) / 90;
    if (degree <= 95) {
        SetStatus(E_FORWARD);
    } else  if (degree >= 175) {
        SetStatus(E_LEFT);
    }
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
    digitalWrite(Ain1Pin, HIGH);
    digitalWrite(Ain2Pin, LOW);
    digitalWrite(Bin1Pin, HIGH);
    digitalWrite(Bin2Pin, LOW);
#else
    digitalWrite(Ain1Pin, HIGH);
    digitalWrite(Bin1Pin, LOW);
#endif
    analogWrite(PwmaPin, value);
    analogWrite(PwmbPin, (float)(value * f));
  } else if (degree > 180 && degree <= 270) {
    f = (float)(degree - 180) / 90;
    if (degree <= 185) {
        SetStatus(E_LEFT);
    } else  if (degree >= 265) {
        SetStatus(E_BACK);
    }
#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
    digitalWrite(Ain1Pin, LOW);
    digitalWrite(Ain2Pin, HIGH);
    digitalWrite(Bin1Pin, LOW);
    digitalWrite(Bin2Pin, HIGH);
#else
    digitalWrite(Ain1Pin, LOW);
    digitalWrite(Bin1Pin, HIGH);
#endif
    analogWrite(PwmaPin, value);
    analogWrite(PwmbPin, (float)(value * f));
  } else if (degree > 270 && degree <= 360) {
    f = (float)(360 - degree) / 90;
    if (degree <= 275) {
        SetStatus(E_BACK);
    } else  if (degree >= 355) {
        SetStatus(E_RIGHT);
    }

#if (EM_MOTOR_SHIELD_BOARD_VERSION < 3)
    digitalWrite(Ain1Pin, LOW);
    digitalWrite(Ain2Pin, HIGH);
    digitalWrite(Bin1Pin, LOW);
    digitalWrite(Bin2Pin, HIGH);
#else
    digitalWrite(Ain1Pin, LOW);
    digitalWrite(Bin1Pin, HIGH);
#endif
    analogWrite(PwmaPin, (float)(value * f));  
    analogWrite(PwmbPin, value);
  }
  else {
    KeepStop();
    return;
  }
}

#if ARDUINO > 10609
void Tank::SetIrPin(uint8_t pin = TA_IR_PIN)
#else
void Tank::SetIrPin(uint8_t pin )
#endif
{
  IrPin = pin;
  mIrRecv = new IRremote (IrPin);
  mIrRecv->begin();  // Initialize the infrared receiver
}

#if ARDUINO > 10609
void Tank::SetBuzzerPin(uint8_t pin = TA_BUZZER_PIN)
#else
void Tank::SetBuzzerPin(uint8_t pin)
#endif

{
  BuzzerPin = pin;
  mBuzzer = new Buzzer(BuzzerPin);
}

#if ARDUINO > 10609
void Tank::SetRgbPin(uint8_t pin = TA_RGB_PIN)
#else
void Tank::SetRgbPin(uint8_t pin )
#endif
{
  RgbPin = pin;
  mRgb = new RGBLed(TA_RGB_PIN, RgbPin);
  mRgb->setNumber(2);
  // mRgb->reset();
}

#if ARDUINO > 10609
void Tank::LightOn(E_RGB_INDEX index = E_RGB_ALL, long Color = RGB_WHITE)
#else
void Tank::LightOn(E_RGB_INDEX index, long Color)
#endif
{
  // mRgb->reset();
  if (index == E_RGB_ALL) {
    mRgb->setColor(0, Color);
  } else {
    mRgb->setColor(index, Color);
  }
  mRgb->show();
}

#if ARDUINO > 10609
void Tank::LightOff(E_RGB_INDEX index = E_RGB_ALL)
#else
void Tank::LightOff(E_RGB_INDEX index )
#endif
{
  //  mRgb->reset();
  if (index == E_RGB_ALL) {
    mRgb->setColor(0, 0, 0);
  } else {
    mRgb->setColor(index, 0);
  }
  mRgb->show();
}

#if ARDUINO > 10609
void Tank::SetUltrasonicPin(uint8_t Trig_Pin = TA_TRIG_PIN, uint8_t Echo_Pin = TA_ECHO_PIN, uint8_t Sevo_Pin = TA_SERVO_PIN)
#else
void Tank::SetUltrasonicPin(uint8_t Trig_Pin , uint8_t Echo_Pin, uint8_t Sevo_Pin )
#endif
{
  static bool UltrasonicInit = false;
  if (!UltrasonicInit) {
    EchoPin = Echo_Pin;
    TrigPin = Trig_Pin;
    ServoPin = Sevo_Pin;
    mUltrasonic = new Ultrasonic(TrigPin, EchoPin, ServoPin);
    UltrasonicInit = true;
  }
}

#if (EM_MOTOR_SHIELD_BOARD_VERSION > 2)
#if ARDUINO > 10609
void Tank::SetNRF24L01Pin(uint8_t CE_Pin = TA_CE_PIN, uint8_t CSN_Pin = TA_CSN_PIN)
#else
void Tank::SetNRF24L01Pin(uint8_t CE_Pin , uint8_t CSN_Pin )
#endif
{
  static bool NRF24L01Init = false;
  if (!NRF24L01Init) {
    CePin = CE_Pin;
    CsnPin = CSN_Pin;
    Mirf = new Nrf24l(CePin, CsnPin);
    NRF24L01Init = true;
  }
}

void Tank::InitNrf24L01(char *Raddr)
{
  Mirf->spi = &MirfHardwareSpi;
  Mirf->init();
  Mirf->setRADDR((byte *)Raddr);
  Mirf->payload = sizeof(NrfData);
  Mirf->channel = 90;
  Mirf->config();
}
int  Tank::GetNrf24L01(void) {
  Mirf->getData((byte *) &NrfData);
  return NrfData;
}
#endif

#if ARDUINO > 10609
int Tank::SetPs2xPin(uint8_t clk = TA_PS2X_CLK, uint8_t cmd = TA_PS2X_CMD, uint8_t att = TA_PS2X_CS, uint8_t dat = TA_PS2X_DAT)
#else
int Tank::SetPs2xPin(uint8_t clk , uint8_t cmd , uint8_t att , uint8_t dat )
#endif

{
  static bool Ps2xInit = false;
  int error = 0 ;
  if (!Ps2xInit) {
    DEBUG_LOG(DEBUG_LEVEL_INFO, "SetPs2xPin\n");
    Ps2xClkPin = clk;
    Ps2xCmdPin = cmd;
    Ps2xAttPin = att;
    Ps2xDatPin = dat;
    mPs2x = new PS2X();
    //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
    //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
    error = mPs2x->config_gamepad(Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin, false, false);

    if (error == 1) {
    //  DEBUG_LOG(DEBUG_LEVEL_ERR, "No controller found, check wiring\n");
    } else if (error == 2) {
   //   DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller found but not accepting commands\n");
    } else if (error == 3) {
   //   DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller refusing to enter Pressures mode, may not support it\n");
    } else if (error == 0) {
   //   DEBUG_LOG(DEBUG_LEVEL_INFO, "Found Controller, configured successful\n");
    }
    Ps2xInit = true;
  }
  return error;
}

int Tank::ResetPs2xPin(void)
{
  int error = mPs2x->config_gamepad(Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin, false, false);

  if (error == 1) {
  //  DEBUG_LOG(DEBUG_LEVEL_ERR, "No controller found, check wiring\n");
  } else if (error == 2) {
  //  DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller found but not accepting commands\n");
  } else if (error == 3) {
 //   DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller refusing to enter Pressures mode, may not support it\n");
  } else if (error == 0) {
  //  DEBUG_LOG(DEBUG_LEVEL_INFO, "Found Controller, configured successful\n");
  }
  return error;
}

void Tank::PianoSing(ST_MUSIC_TYPE music)
{
    mBuzzer->_tone(music.note, music.beat, 2);
}

void Tank::Sing(byte songName)
{
  switch (songName) {
    case S_connection:
      mBuzzer->_tone(note_E5, 50, 30);
      mBuzzer->_tone(note_E6, 55, 25);
      mBuzzer->_tone(note_A6, 60, 10);
      //  _tone(9, 394);
      break;

    case S_disconnection:
      mBuzzer->_tone(note_E5, 50, 30);
      mBuzzer->_tone(note_A6, 55, 25);
      mBuzzer->_tone(note_E6, 50, 10);
      break;

    case S_buttonPushed:
      mBuzzer->bendTones (note_E6, note_G6, 1.03, 20, 2);
      delay(30);
      mBuzzer->bendTones (note_E6, note_D7, 1.04, 10, 2);
      break;

    case S_mode1:
      mBuzzer->bendTones (note_E6, note_A6, 1.02, 30, 10);  //1318.51 to 1760
      break;

    case S_mode2:
      mBuzzer->bendTones (note_G6, note_D7, 1.03, 30, 10);  //1567.98 to 2349.32
      break;

    case S_mode3:
      mBuzzer->_tone(note_E6, 50, 100); //D6
      mBuzzer->_tone(note_G6, 50, 80); //E6
      mBuzzer->_tone(note_D7, 300, 0); //G6
      break;

    case S_surprise:
      mBuzzer->bendTones(800, 2150, 1.02, 10, 1);
      mBuzzer->bendTones(2149, 800, 1.03, 7, 1);
      break;

    case S_OhOoh:
      mBuzzer->bendTones(880, 2000, 1.04, 8, 3); //A5 = 880
      delay(200);

      for (int i = 880; i < 2000; i = i * 1.04) {
        mBuzzer->_tone(note_B5, 5, 10);
      }
      break;

    case S_OhOoh2:
      mBuzzer->bendTones(1880, 3000, 1.03, 8, 3);
      delay(200);

      for (int i = 1880; i < 3000; i = i * 1.03) {
        mBuzzer->_tone(note_C6, 10, 10);
      }
      break;

    case S_cuddly:
      mBuzzer->bendTones(700, 900, 1.03, 16, 4);
      mBuzzer->bendTones(899, 650, 1.01, 18, 7);
      break;

    case S_sleeping:
      mBuzzer->bendTones(100, 500, 1.04, 10, 10);
      delay(500);
      mBuzzer->bendTones(400, 100, 1.04, 10, 1);
      break;

    case S_happy:
      mBuzzer->bendTones(1500, 2500, 1.05, 20, 8);
      mBuzzer->bendTones(2499, 1500, 1.05, 25, 8);
      break;

    case S_superHappy:
      mBuzzer->bendTones(2000, 6000, 1.05, 8, 3);
      delay(50);
      mBuzzer->bendTones(5999, 2000, 1.05, 13, 2);
      break;

    case S_happy_short:
      mBuzzer->bendTones(1500, 2000, 1.05, 15, 8);
      delay(100);
      mBuzzer->bendTones(1900, 2500, 1.05, 10, 8);
      break;

    case S_sad:
      mBuzzer->bendTones(880, 669, 1.02, 20, 200);
      break;

    case S_confused:
      mBuzzer->bendTones(1000, 1700, 1.03, 8, 2);
      mBuzzer->bendTones(1699, 500, 1.04, 8, 3);
      mBuzzer->bendTones(1000, 1700, 1.05, 9, 10);
      break;

    case S_fart1:
      mBuzzer->bendTones(1600, 3000, 1.02, 2, 15);
      break;

    case S_fart2:
      mBuzzer->bendTones(2000, 6000, 1.02, 2, 20);
      break;

    case S_fart3:
      mBuzzer->bendTones(1600, 4000, 1.02, 2, 20);
      mBuzzer->bendTones(4000, 3000, 1.02, 2, 20);
      break;
  }
}

void Tank::SetBatteryCheckPin(uint8_t pin)
{
    BatteryPin = pin;
    pinMode(BatteryPin, INPUT);
}

uint16_t Tank::GetBatteryValue(void)
{
    short int sensorValue = analogRead(BatteryPin);
    float voltage = sensorValue * (2000.0 / 1023.0);
   // Serial.println((short int)voltage);
    return (uint16_t)voltage;
}

void Tank::SendUltrasonicData(void) {
    union
    {
        float d;
        byte data[4];
    } value;
    value.d = mUltrasonic->GetUltrasonicFrontDistance();
    SendData.start_code = 0xAA;
    SendData.type = E_PANTHER_TANK;
    SendData.addr = 0x01;
    SendData.function = E_ULTRASONIC;
    SendData.data = value.data;
    SendData.len = 10;
    SendData.end_code = 0x55;
    mProtocolPackage->SendPackage(&SendData, 4);
}

void Tank::SendBatteryPackage(uint16_t battery_value)
{
    byte value[2];
    SendData.start_code = PROTOCOL_START_CODE;
    SendData.type = E_PANTHER_TANK;
    SendData.addr = 0x01;
    SendData.function = E_BATTERY;
    value[0] = (battery_value >> 8) & 0xFF;
    value[1] = battery_value & 0xFF;
    SendData.data = value;
    SendData.len = 8;
    SendData.end_code = PROTOCOL_END_CODE;
    mProtocolPackage->SendPackage(&SendData, 2);
}

void Tank::SendVersionPackage(void)
{
    char value[5];
    SendData.start_code = PROTOCOL_START_CODE;
    SendData.type = E_PANTHER_TANK;
    SendData.addr = 0x01;
    SendData.function = E_VERSION;
    sprintf(value, "%d.%s", E_PANTHER_TANK, VERSION);
    SendData.data =(byte*)value;
    SendData.len = 13;
    SendData.end_code = PROTOCOL_END_CODE;
    Serial.println(value);
    // mProtocolPackage->SendPackage(&SendData, 5);
}


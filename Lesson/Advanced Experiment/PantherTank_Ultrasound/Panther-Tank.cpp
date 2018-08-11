#include "Panther-Tank.h"
#include "ProtocolParser.h"
#include "Sounds.h"
#include "debug.h"


// For single motor drive
// Tank::Tank(ProtocolParser *Package, uint8_t Servo_Pin = TA_SERVO_PIN, uint8_t bin1 = TA_BIN1_PIN, uint8_t bin2 = TA_BIN2_PIN, uint8_t pwmb = TA_PWMB_PIN, uint8_t standby = TA_STANBY_PIN):SmartCar("Tank", E_AURORA_RACING, 0x01, E_BLUTOOTH_CONTROL)
Tank::Tank(ProtocolParser *Package,  uint8_t bin1, uint8_t bin2, uint8_t pwmb, uint8_t standby): SmartCar("Tank", E_AURORA_RACING, 0x01, E_BLUTOOTH_CONTROL)
{
  this->Bin1Pin = bin1;
  this->Bin2Pin = bin2;
  this->PwmbPin = pwmb;
  this->StandbyPin = standby;
  SetStatus(E_STOP);
  mProtocolPackage = Package;
}

// For dual motor drive
//Tank::Tank(ProtocolParser *Package, uint8_t ain1 = TA_AIN1_PIN, uint8_t ain2 = TA_AIN2_PIN, uint8_t pwma = TA_PWMA_PIN, uint8_t bin1 = TA_BIN1_PIN, uint8_t bin2 = TA_BIN2_PIN, uint8_t pwmb = TA_PWMB_PIN, uint8_t standby = TA_STANBY_PIN):SmartCar("Tank", E_AURORA_RACING, 0x01, E_BLUTOOTH_CONTROL)
Tank::Tank(ProtocolParser *Package, uint8_t ain1, uint8_t ain2, uint8_t pwma, uint8_t bin1, uint8_t bin2, uint8_t pwmb, uint8_t standby): SmartCar("Tank", E_AURORA_RACING, 0x01, E_BLUTOOTH_CONTROL)
{
  this->Ain1Pin = ain1;
  this->Ain2Pin = ain2;
  this->PwmaPin = pwma;
  this->Bin1Pin = bin1;
  this->Bin2Pin = bin2;
  this->PwmbPin = pwmb;
  this->StandbyPin = standby;
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
}

void Tank::init(void)
{
  //keep TB6612 AIN stop
  pinMode(Ain1Pin, OUTPUT);
  digitalWrite(Ain1Pin, LOW);
  pinMode(Ain2Pin, OUTPUT);
  digitalWrite(Ain2Pin, LOW);
  pinMode(PwmaPin, OUTPUT);
  digitalWrite(PwmaPin, HIGH);
  //keep TB6612 BIN stop
  pinMode(Bin1Pin, OUTPUT);
  digitalWrite(Bin1Pin, LOW);
  pinMode(Bin2Pin, OUTPUT);
  digitalWrite(Bin2Pin, LOW);
  pinMode(PwmbPin, OUTPUT);
  digitalWrite(PwmbPin, HIGH);
  pinMode(StandbyPin, OUTPUT);
  digitalWrite(StandbyPin, LOW);
}

void Tank::GoForward(void)
{
  int value = (Speed / 10) * 25;
  DEBUG_LOG(DEBUG_LEVEL_INFO, "GoForward\n");
  SetStatus(E_FORWARD);
  digitalWrite(Ain1Pin, HIGH);
  digitalWrite(Ain2Pin, LOW);
  analogWrite(PwmaPin, value);
  digitalWrite(Bin1Pin, HIGH);
  digitalWrite(Bin2Pin, LOW);
  analogWrite(PwmbPin, value);
  digitalWrite(StandbyPin, HIGH);
}

void Tank::GoBack(void)
{
  int value = (Speed / 10) * 25;
  DEBUG_LOG(DEBUG_LEVEL_INFO, "GoBack\n");
  SetStatus(E_BACK);
  digitalWrite(Ain1Pin, LOW);
  digitalWrite(Ain2Pin, HIGH);
  analogWrite(PwmaPin, value);
  digitalWrite(Bin1Pin, LOW);
  digitalWrite(Bin2Pin, HIGH);
  analogWrite(PwmbPin, value);
  digitalWrite(StandbyPin, HIGH);
}

void Tank::KeepStop(void)
{
  DEBUG_LOG(DEBUG_LEVEL_INFO, "KeepStop\n");
  digitalWrite(PwmaPin, 0);
  digitalWrite(Ain1Pin, LOW);
  digitalWrite(Ain2Pin, HIGH);
  digitalWrite(PwmbPin, 0);
  digitalWrite(Bin1Pin, LOW);
  digitalWrite(Bin2Pin, HIGH);
  digitalWrite(StandbyPin, HIGH);
  SetStatus(E_STOP);
}

void Tank::TurnLeft(void)
{
  int value = (Speed / 10) * 25;   //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
  DEBUG_LOG(DEBUG_LEVEL_INFO, "TurnLeft\n");
  digitalWrite(Ain1Pin, HIGH);
  digitalWrite(Ain2Pin, LOW);
  analogWrite(PwmaPin, value);
  digitalWrite(Bin1Pin, HIGH);
  digitalWrite(Bin2Pin, LOW);
  analogWrite(PwmbPin, 0);
  digitalWrite(StandbyPin, HIGH);
  SetStatus(E_LEFT);
}

void Tank::TurnRight(void)
{
  int value = (Speed / 10) * 25;   //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
  DEBUG_LOG(DEBUG_LEVEL_INFO, "TurnRight\n");
  digitalWrite(Ain1Pin, HIGH);
  digitalWrite(Ain2Pin, LOW);
  analogWrite(PwmaPin, 0);
  digitalWrite(Bin1Pin, HIGH);
  digitalWrite(Bin2Pin, LOW);
  analogWrite(PwmbPin, value);
  digitalWrite(StandbyPin, HIGH);
  SetStatus(E_RIGHT);
}

void Tank::Drive(void)
{
  Drive(Degree);
}

void Tank::Drive(int degree)
{
  DEBUG_LOG(DEBUG_LEVEL_INFO, "degree = %d speed = %d\n", degree, Speed);
  int value = (Speed / 10) * 25;	 //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
  float f;
  if ((0 <= degree && degree <= 5 ) || (degree >= 355 && degree <= 360)) {
    digitalWrite(Ain1Pin, LOW);
    digitalWrite(Ain2Pin, HIGH);
    analogWrite(PwmaPin, value);
    digitalWrite(Bin1Pin, HIGH);
    digitalWrite(Bin2Pin, LOW);
    analogWrite(PwmbPin, value);
    digitalWrite(StandbyPin, HIGH);
    SetStatus(E_RIGHT);
  }
  else if (degree > 5 && degree <= 80) {
    f = (float)(degree) / 79;
    digitalWrite(Ain1Pin, HIGH);
    digitalWrite(Ain2Pin, LOW);
    analogWrite(PwmaPin,  (float)(value * f));
    digitalWrite(Bin1Pin, HIGH);
    digitalWrite(Bin2Pin, LOW);
    analogWrite(PwmbPin, value);
    digitalWrite(StandbyPin, HIGH);
    SetStatus(E_RIGHT);
  }
  else if (degree > 80 && degree < 100) {
    value = (Speed / 10) * 25;
    GoForward();
  }
  else if (degree >= 100 && degree < 175) {
    f = (float)(180 - degree) / 79;
    digitalWrite(Ain1Pin, HIGH);
    digitalWrite(Ain2Pin, LOW);
    analogWrite(PwmaPin, value);

    digitalWrite(Bin1Pin, HIGH);
    digitalWrite(Bin2Pin, LOW);
    analogWrite(PwmbPin,  (float)(value * f));
    digitalWrite(StandbyPin, HIGH);
    SetStatus(E_LEFT);
  }
  else if (175 <= degree && degree <= 185) {
    digitalWrite(Ain1Pin, HIGH);
    digitalWrite(Ain2Pin, LOW);
    analogWrite(PwmaPin, value);
    digitalWrite(Bin1Pin, LOW);
    digitalWrite(Bin2Pin, HIGH);
    analogWrite(PwmbPin, value);
    digitalWrite(StandbyPin, HIGH);
  }
  else if (degree > 185 && degree <= 260) {
    f = (float)(degree - 180) / 79;
    digitalWrite(Ain1Pin, LOW);
    digitalWrite(Ain2Pin, HIGH);
    analogWrite(PwmaPin, value);
    digitalWrite(Bin1Pin, LOW);
    digitalWrite(Bin2Pin, HIGH);
    analogWrite(PwmbPin, (float)(value * f));
    digitalWrite(StandbyPin, HIGH);
    SetStatus(E_LEFT);
  }
  else if (degree > 260 && degree < 280) {
    value = (Speed / 10) * 25;
    GoBack();
  }
  else if (degree >= 280 && degree < 355) {
    f = (float)(360 - degree) / 79;
    digitalWrite(Ain1Pin, LOW);
    digitalWrite(Ain2Pin, HIGH);
    analogWrite(PwmaPin,  (float)(value * f));
    digitalWrite(Bin1Pin, LOW);
    digitalWrite(Bin2Pin, HIGH);
    analogWrite(PwmbPin, value);
    digitalWrite(StandbyPin, HIGH);
    SetStatus(E_RIGHT);
  }
  else {
    KeepStop();
    return;
  }
}

void Tank::SetIrPin(uint8_t pin = TA_IR_PIN)
{
  IrPin = pin;
  mIrRecv = new IRremote (IrPin);
  mIrRecv->begin();  // Initialize the infrared receiver
}

void Tank::SetBuzzerPin(uint8_t pin = TA_BUZZER_PIN)
{
  BuzzerPin = pin;
  mBuzzer = new Buzzer();
  mBuzzer->setpin(BuzzerPin);
}

void Tank::SetRgbPin(uint8_t pin = TA_RGB_PIN)
{
  RgbPin = pin;
  mRgb = new RGBLed(7, RgbPin);
  mRgb->setNumber(2);
  // mRgb->reset();
}

void Tank::LightOn(E_RGB_INDEX index = E_RGB_ALL, long Color = RGB_WHITE)
{
  // mRgb->reset();
  if (index == E_RGB_ALL) {
    mRgb->setColor(0, Color);
  } else {
    mRgb->setColor(index, Color);
  }
  mRgb->show();
}
void Tank::LightOff(E_RGB_INDEX index = E_RGB_ALL)
{
  //  mRgb->reset();
  if (index == E_RGB_ALL) {
    mRgb->setColor(0, 0, 0);
  } else {
    mRgb->setColor(index, 0);
  }
  mRgb->show();
}

void Tank::SetInfraredTracingPin(uint8_t Pin1 = TA_INFRARED_TRACING_PIN1, uint8_t Pin2 = TA_INFRARED_TRACING_PIN2, uint8_t Pin3 = TA_INFRARED_TRACING_PIN3)
{
  static bool InfraredTracingInit = false;
  if (!InfraredTracingInit) {
    InfraredTracingPin1 = Pin1;
    InfraredTracingPin2 = Pin2;
    InfraredTracingPin3 = Pin3;
    mInfraredTracing = new InfraredTracing(InfraredTracingPin1, InfraredTracingPin2, InfraredTracingPin3);
    // mInfraredTracing->begin();
    InfraredTracingInit = true;
  }
}

void Tank::SetUltrasonicPin(uint8_t Trig_Pin = TA_TRIG_PIN, uint8_t Echo_Pin = TA_ECHO_PIN, uint8_t Sevo_Pin = TA_SERVO_PIN)
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

int Tank::SetPs2xPin(uint8_t clk = TA_PS2X_CLK, uint8_t cmd = TA_PS2X_CMD, uint8_t att = TA_PS2X_CS, uint8_t dat = TA_PS2X_DAT)
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
      DEBUG_LOG(DEBUG_LEVEL_ERR, "No controller found, check wiring\n");
    } else if (error == 2) {
      DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller found but not accepting commands\n");
    } else if (error == 3) {
      DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller refusing to enter Pressures mode, may not support it\n");
    } else if (error == 0) {
      DEBUG_LOG(DEBUG_LEVEL_INFO, "Found Controller, configured successful\n");
    }
    Ps2xInit = true;
  }
  return error;
}

int Tank::ResetPs2xPin(void)
{
  int error = mPs2x->config_gamepad(Ps2xClkPin, Ps2xCmdPin, Ps2xAttPin, Ps2xDatPin, false, false);

  if (error == 1) {
    DEBUG_LOG(DEBUG_LEVEL_ERR, "No controller found, check wiring\n");
  } else if (error == 2) {
    DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller found but not accepting commands\n");
  } else if (error == 3) {
    DEBUG_LOG(DEBUG_LEVEL_ERR, "Controller refusing to enter Pressures mode, may not support it\n");
  } else if (error == 0) {
    DEBUG_LOG(DEBUG_LEVEL_INFO, "Found Controller, configured successful\n");
  }
  return error;
}

float Tank::PianoSing(byte b[])
{
    union result
    {
      float d;
      unsigned char data[4];
    }r1,r2;
    r2.data[0]=b[0];
    r2.data[1]=b[1];
    r2.data[2]=b[2];
    r2.data[3]=b[3];
    //mBuzzer->noTone(9);
    mBuzzer->_tone(r2.d,120, 2);
    return (r2.d);
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

void Tank::SendBatteryPackage(byte *battery_value)
{
  ST_PROTOCOL send_dat;
  send_dat.start_code = PROTOCOL_START_CODE;
  send_dat.type = 0x01;
  send_dat.addr = 0x01;
  send_dat.function = E_BATTERY;
  send_dat.data = battery_value;
  send_dat.end_code = PROTOCOL_END_CODE;
  mProtocolPackage->SendPackage(&send_dat, 1);
}

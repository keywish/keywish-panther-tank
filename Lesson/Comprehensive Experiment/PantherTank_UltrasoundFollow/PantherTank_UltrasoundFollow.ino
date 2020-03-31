#include<Arduino.h>
#include<Wire.h>
#include "Panther_Tank.h"
#include "ProtocolParser.h"
#include "BluetoothHandle.h"
#include "debug.h"
#include "KeyMap.h"
ProtocolParser *mProtocol = new ProtocolParser();
Tank mTank(mProtocol);
void setup()
{

  Serial.begin(9600);
  mTank.init(M2, M1);
  mTank.SetControlMode(E_ULTRASONIC_FOLLOW_MODE);
  mTank.InitServo();
  mTank.InitRgb();
  mTank.InitBuzzer();
  mTank.SetSpeed(50);
  mTank.InitUltrasonic();
  mTank.SetServoBaseDegree(90);
  mTank.SetServoDegree(1, 90);
  Serial.println("init ok");
  mTank.sing(S_connection);
}

void UltrasonicFollow()
{
  mTank.SetSpeed(40);
  uint16_t UlFrontDistance =  mTank.GetUltrasonicValue(FRONT);
  delay(10);
  if (UlFrontDistance < 10) {
    mTank.GoBack();
  } else if (UlFrontDistance > 14) {
    mTank.GoForward();
  } else if (10 <= UlFrontDistance <= 14) {
    mTank.KeepStop();
  }
}

void loop()
{
  static byte mode;
  static bool recv_flag;
  mProtocol->RecevData();
  if (recv_flag = mProtocol->ParserPackage()) {
    if (mProtocol->GetRobotControlFun() == E_CONTROL_MODE) {
      mTank.SetControlMode(mProtocol->GetControlMode());
      return;
    }
  }
  switch (mTank.GetControlMode()) {
    case E_ULTRASONIC_FOLLOW_MODE:
      UltrasonicFollow();
      break;
    default:
      break;
  }
  switch (mTank.GetStatus()) {
    case E_FORWARD:
      mTank.SetRgbColor(E_RGB_ALL, RGB_WHITE);
      break;
    case E_LEFT:
      mTank.SetRgbColor(E_RGB_LEFT, RGB_WHITE);
      break;
    case E_RIGHT:
      mTank.SetRgbColor(E_RGB_RIGHT, RGB_WHITE);
      break;
    case E_BACK:
      mTank.SetRgbColor(E_RGB_ALL, RGB_RED);
      break;
    case E_STOP:
      mTank.LightOff();
      break;
    default:
      break;
  }
}

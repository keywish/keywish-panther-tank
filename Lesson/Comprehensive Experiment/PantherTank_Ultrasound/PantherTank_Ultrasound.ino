#include "debug.h"
#include "Emakefun_MotorDriver.h"
#include "Panther_Tank.h"
#include "ProtocolParser.h"
#include "BluetoothHandle.h"
#include "KeyMap.h"
ProtocolParser *mProtocol = new ProtocolParser();
Tank mTank(mProtocol);
byte count = 0;

void setup()
{
  Serial.begin(9600);
  mTank.init(M2, M1);
  mTank.SetControlMode(E_ULTRASONIC_AVOIDANCE);
  mTank.InitServo();
  mTank.InitRgb();
  mTank.InitBuzzer();
  mTank.SetSpeed(50);
  mTank.SetServoBaseDegree(90);
  mTank.SetServoDegree(1, 90);
  mTank.InitUltrasonic();
  Serial.println("init ok");
  mTank.sing(S_connection);
}

void HandleUltrasonicAvoidance(void)
{
  uint16_t UlFrontDistance, UlLeftDistance, UlRightDistance;
  UlFrontDistance = mTank.GetUltrasonicValue(0);
  if (count++ > 50) {
    mTank.SendUltrasonicData();
    count = 0;
  }
  DEBUG_LOG(DEBUG_LEVEL_INFO, "UlFrontDistance = %d \n", UlFrontDistance);
  if (UlFrontDistance < UL_LIMIT_MIN)
  {
    mTank.SetSpeed(80);
    mTank.GoBack();
    delay(200);
  }
  if (UlFrontDistance < UL_LIMIT_MID)
  {
    mTank.KeepStop();
    delay(100);
    UlRightDistance = mTank.GetUltrasonicValue(2);
    delay(50);
    UlLeftDistance = mTank.GetUltrasonicValue(1);
    if ((UlRightDistance > UL_LIMIT_MIN) && (UlRightDistance < UL_LIMIT_MAX)) {
      mTank.SetSpeed(100);
      mTank.TurnRight();
      delay(400);
    } else if ((UlLeftDistance > UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MAX)) {
      mTank.SetSpeed(100);
      mTank.TurnLeft();
      delay(400);
    } else if ((UlRightDistance < UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MIN) ) {
      mTank.SetSpeed(400);
      mTank.TurnLeft();
      delay(800);
    }
  } else {
    mTank.SetSpeed(80);
    mTank.GoForward();
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
    case E_ULTRASONIC_AVOIDANCE:
      HandleUltrasonicAvoidance();
      break;
    default:
      break;
  }
  switch (mTank.GetStatus()) {
    case E_FORWARD:
      mTank.SetRgbColor(E_RGB_ALL, RGB_WHITE);
      break;
    case E_LEFT_ROTATE:
      mTank.SetRgbColor(E_RGB_LEFT, RGB_WHITE);
      break;
    case E_RIGHT_ROTATE:
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

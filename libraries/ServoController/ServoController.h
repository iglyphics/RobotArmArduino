#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include "Arduino.h"
#include "MotorController.h"
#include "PidController.h"
#include "RobotArmRequest.h"


class ServoController : public MotorController
{
  public:
  enum Modes
  {
    STOP, GET_MIN_MAX, SEEKING, HOLDING
  };
  
  int Angle;
  int PotPin;
  int MinPotValue;
  int MaxPotValue;
  int Mode;
  int StartPos;
  int SeekPos;
  int Speed;
  int MaxSpeed;
  int SpeedFactor;
  int PowerBoost;
  int PosOffset;
  float AngleOffset;
  int LastPos;
  unsigned long LastMillis;
  PidController Pid;
  long PidCycle;
  void (*DisplayFunc)(ServoController &);
  void (*StatusFunc)(ServoController &);
  int DisplayCycle;
  int LastDisplayMillis;
  RobotArmRequest *Req;
  float AngleFactor;
  int TargetAngle;

  
  
  //----------------------------------------
  
  ServoController(int LeftPin, int RightPin, int SpeedPin, int PotPin);
  
  //----------------------------------------
  
  void Command(byte Action, float Value);
  void Command(RobotArmRequest &Req);
  void DoWork();
  void GetMinMax();
  void Initialize();
  void Stop();
  void SetAngle(float Angle);
  void Seek();
  int CurrentPos();
  int CurrentAngle();
  void SetPos();
  void SetDisplayFunc(void (*Func)(ServoController &));
  void SetStatusFunc(void (*Func)(ServoController &));
  void DoDisplay();
  void SendMessage(String Msg);
};

#endif

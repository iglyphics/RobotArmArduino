#ifndef TURNTABLECONTROLLER_H
#define TURNTABLECONTROLLER_H

#include "Arduino.h"
#include "MotorController.h"
#include "RobotArmRequest.h"

class TurnTableController : public MotorController
{
  public:
  enum Modes
  {
    STOP, CALIBRATE, SEEK
  };

  enum SeekTypes
  {
	  SEEK_LIGHT, SEEK_DARK, SEEK_NONE
  };

  int Mode;
  int PhotoPin;
  float MaxLightValue;
  float MinLightValue;
  int LightThreshold;
  int DarkThreshold;
  int SeekSpeed;
  int SeekType;
  unsigned long LastMillis;
  int WorkCycle;

  
  TurnTableController(int LeftPin, int RightPin, int SpeedPin, int PhotoPin);
  
  //----------------------------------------

  void CalculateThresholds();
  void Command(RobotArmRequest &Req);
  void Stop();
  void Initialize();
  void Calibrate();
  void StartSeek(int Value);
  void Seek();
  void SeekLight(int Value);
  void SeekDark(int Value);
  void DoWork();
  void SendStatus();
  int TimeForWork();
};

#endif

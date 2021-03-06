#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include "Arduino.h"

class MotorController
{
  public:
  enum State {STOPPED, FWD, REV};
  int Direction;
  int Power;
  int MaxPower;
  public:
  int LeftPin;
  int RightPin;
  int PowerPin;
  char Output[256];
  
  //----------------------------------------
  
  MotorController();
  MotorController(int LeftPin, int RightPin, int PowerPin);
  
  //----------------------------------------
  
  void Attach();
  void IncreasePower();
  void IncreasePower(int Val);
  void DecreasePower();
  void DecreasePower(int Val);
  void SetDynamicPower(int Val);
  int DynamicPower();
  void DecreaseDynamicPower(int Val);
  void IncreaseDynamicPower(int Val);
  void SetPower(int Val);
  void GoForward();
  void GoForward(int Power);
  void GoReverse();
  void GoReverse(int Power);
  void Brake();
};



#endif


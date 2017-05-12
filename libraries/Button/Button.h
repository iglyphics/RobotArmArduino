#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"
#include "Servo.h"

class Button : public Servo
{
  public:
  enum Modes
  {
    STOP, PRESS, PRESS_DELAY, RELEASE, RELEASE_DELAY
  };
  
  int ReleaseAngle;
  int PressAngle;
  long PressDelay;
  long LastMillis;
  int Pressed;
  int Initialized;
  int Mode;
  
  Button();
  
  //----------------------------------------

  void Press();
  void Depress();
  void Release();
  void DoWork();
};






#endif

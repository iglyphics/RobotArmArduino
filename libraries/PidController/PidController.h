#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include "Arduino.h"

class PidController
{
  public:
  double Kp;
  double Ki;
  double Kd;
  double SetPos;
  double MinI;
  double MaxI;
  double SumI;
  double LastPos;
  long LastMillis;
  
  
  int Update(double Pos);
};

#endif


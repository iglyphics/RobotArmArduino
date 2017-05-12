#ifndef ROBOTARMREQUEST_H
#define ROBOTARMREQUEST_H

class RobotArmRequest
{
  public:
  
  char  Joint[5];
  char  Command[11];
  char  Action[5];
  int   Value;
  float   FloatValue;
};

#endif

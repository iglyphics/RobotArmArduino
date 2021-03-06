#include "Arduino.h"
#include "MotorController.h"

MotorController::MotorController()
  {
    this->Direction = MotorController::STOPPED;
    this->Power = 0;
    this->LeftPin = 0;
    this->RightPin = 0;
    this->PowerPin = 0;
    this->MaxPower = 30;
    this->Attach();
  }
  
  //----------------------------------------
  
MotorController::MotorController(int LeftPin, int RightPin, int PowerPin)
  {
    this->Direction = MotorController::STOPPED;
    this->Power = 0;
    this->LeftPin = LeftPin;
    this->RightPin = RightPin;
    this->PowerPin = PowerPin;
    this->MaxPower = 30;
    this->Attach();
  }
  
  //----------------------------------------
  
  void MotorController::Attach()
  {
    pinMode(this->LeftPin, OUTPUT);
    pinMode(this->RightPin, OUTPUT);
    pinMode(this->PowerPin, OUTPUT);   
  }
  
  //----------------------------------------
  
  void MotorController::IncreasePower()
  {
    this->IncreasePower(1);
  }
  
  //----------------------------------------
  
  void MotorController::IncreasePower(int Val)
  {
    this->SetPower(this->Power + Val);
  }
  
  //----------------------------------------
  
  void MotorController::DecreasePower()
  {
    this->DecreasePower(1);
  }
  
  //----------------------------------------
  
  void MotorController::DecreasePower(int Val)
  {
    this->SetPower(this->Power - Val);
  }
   
  //----------------------------------------

  void MotorController::SetDynamicPower(int Val)
  {
    if (Val < 0)
    {
      GoReverse(abs(Val));
    }
    else if (Val > 0)
    {
      GoForward(Val);
    }
    else
    {  
      Brake();
    }
  }

  //----------------------------------------

  int MotorController::DynamicPower()
  {
    int P;
  
    P = this->Power;
  
    if (this->Direction ==  MotorController::REV)
    {
      P = P * -1;
    }
    
    return P;
  }
  
  //----------------------------------------

  void MotorController::DecreaseDynamicPower(int Val)
  {
    int P;
  
    P = this->Power - Val;
    if (this->Direction == MotorController::REV)
    {
      P = P * -1;
    }
    SetDynamicPower(Val);
  }
  
    //----------------------------------------

  void MotorController::IncreaseDynamicPower(int Val)
  {
    int P;
  
    P = this->Power + Val;
    if (this->Direction == MotorController::REV)
    {
      P = P * -1;
    }
    SetDynamicPower(Val);
  }
  
  //----------------------------------------

  void MotorController::SetPower(int Val)
  {
    if (Val < 0)
    {
      Val = 0;
    }
    else if (Val > this->MaxPower)
    {
      Val = this->MaxPower;
    }
    
    if (Val != this->Power)
    {
      
      this->Power = Val;
      //this->ServoCtl.write(255 - Val);
      analogWrite(this->PowerPin, 255 - Val);
      //sprintf(Output, "Set Speed = %d", 255 - Val);
      //Serial.println(Output);
    }
  }
  
  //----------------------------------------
  
  void MotorController::GoForward()
  {
    if (this->Direction != MotorController::FWD)
    {
      if (this->Direction == MotorController::REV)
      {
        this->Brake();
      }
      this->Direction = MotorController::FWD;
      digitalWrite(this->LeftPin, 0);
      digitalWrite(this->RightPin, 1);
    }
  }
  
  
  //----------------------------------------
  
  void MotorController::GoForward(int Power)
  {
    this->GoForward();
    this->SetPower(Power); 
  }
  
  //----------------------------------------
  
  void MotorController::GoReverse()
  {
    if (this->Direction != MotorController::REV)
    {
      if (this->Direction == MotorController::FWD)
      {
        this->Brake();
      }
      this->Direction = MotorController::REV;
      digitalWrite(this->RightPin, 0);
      digitalWrite(this->LeftPin, 1);
    }
  }
  
  //----------------------------------------
  
  void MotorController::GoReverse(int Power)
  {
    this->GoReverse();
    this->SetPower(Power);
  }
    
  //----------------------------------------
  
  void MotorController::Brake()
  {
    if (this->Direction != MotorController::STOPPED)
    {
      this->Direction = MotorController::STOPPED;
      SetPower(0);
      digitalWrite(this->RightPin, 1);
      digitalWrite(this->LeftPin, 1);
    }
  }   



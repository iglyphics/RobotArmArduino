#include "Arduino.h"
#include "TurnTableController.h"
#include "RobotArmRequest.h"

TurnTableController::TurnTableController(int LeftPin, int RightPin, int SpeedPin, int PhotoPin)  : MotorController(LeftPin, RightPin, SpeedPin)
  {
    this->PhotoPin = PhotoPin; 
    this->Mode = STOP;
    this->MaxLightValue = 500;
    this->MinLightValue = 250;
	 this->SeekSpeed = 5;
    this->CalculateThresholds();
	 this->LastMillis = 0;
	 this->WorkCycle = 100;
  }
  
  //----------------------------------------

  void TurnTableController::CalculateThresholds()
  {
    float Delta;
    Delta = this->MaxLightValue - this->MinLightValue;
    this->DarkThreshold = this->MinLightValue + .35 * Delta;
    this->LightThreshold = this->MaxLightValue - .35 * Delta;
  }
  
  //----------------------------------------
  
  void TurnTableController::Command(RobotArmRequest &Req)
  {
    //sprintf(Output, "Action: %c, Value: %d", Action, Value);
    //Serial.println(Output);
    switch (Req.Action[0])
    {      
      case '!':
        this->Stop();
        //this->Brake();
        break; 
        
      case 'S':
        this->StartSeek(Req.Value);
        break;
        
      case 'I':
        this->Initialize();
        break;

		 case '?':
		  SendStatus();
    } 
  }
  
  //----------------------------------------
  
	int TurnTableController::TimeForWork()
	{
		int RetVal = 0;
		if (millis() - this->LastMillis > this->WorkCycle)
		{
			this->LastMillis = millis();
			RetVal = 1;
		}
		return RetVal;
	}

  //----------------------------------------
  
	void TurnTableController::SendStatus()
	{
		Serial.print("~");
		Serial.print(this->Mode);
		Serial.print(", ");
		Serial.print(analogRead(this->PhotoPin));
		Serial.print(", ");
		Serial.print(this->LightThreshold);
		Serial.print(", ");
		Serial.print(this->DarkThreshold);
		Serial.print("!");
	}

  //----------------------------------------

  void TurnTableController::Stop()
  {
    this->Mode = STOP;
    this->Brake();
  }  
  
  //----------------------------------------
  
  void TurnTableController::Initialize()
  {
    this->Mode = CALIBRATE;
    this->MaxLightValue = 0;
    this->MinLightValue = 1024;
  }
  
  //----------------------------------------
  
  void TurnTableController::Calibrate()
  {
    int LightValue;
    int Changed = 0;
    
    LightValue = analogRead(this->PhotoPin);
    if (LightValue < this->MinLightValue)
    {
      this->MinLightValue = LightValue;
    }
    if (LightValue > this->MaxLightValue)
    {
      this->MaxLightValue = LightValue;
    }
    this->CalculateThresholds();
  }
  
  //----------------------------------------
  
  void TurnTableController::StartSeek(int Value)
  {
	 this->SeekSpeed = Value; 
	 this->SeekType = SEEK_LIGHT;
    this->Mode = SEEK;
  }

  //----------------------------------------
  
  void TurnTableController::Seek()
  {
		int LightValue;
    
		LightValue = analogRead(this->PhotoPin);
	  switch(this->SeekType)
	  {
		  case SEEK_LIGHT:
			  this->SeekLight(LightValue);
			  break;

			case SEEK_DARK:
			  this->SeekDark(LightValue);
			  break;

			default:
			  this->Stop();
			  break;
	  }
	  if (this->Mode == SEEK)
	  {
		  this->GoForward(5);
	  }
  }
  
  //----------------------------------------
  
  void TurnTableController::SeekLight(int Value)
  {
    
		 if (Value > this->LightThreshold)
		 {
			this->SeekType = SEEK_DARK;
		 }
  }
  
 //----------------------------------------
   
  void TurnTableController::SeekDark(int Value)
  {
		 if (Value < this->DarkThreshold)
		 {
			 this->SeekType = SEEK_NONE;
		 }
  }
  
  //----------------------------------------
  
  void TurnTableController::DoWork()
  {
    switch(this->Mode)
    {
      case CALIBRATE:
        this->Calibrate();
        break;
        
      case SEEK:
        this->Seek();
        break;
         
      case STOP:
        this->Brake();
        break;
    }
  }


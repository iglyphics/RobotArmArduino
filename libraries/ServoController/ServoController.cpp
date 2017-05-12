#include "Arduino.h"
#include "ServoController.h"
#include "PidController.h"
#include "RobotArmRequest.h"
#include "string.h"

ServoController::ServoController(int LeftPin, int RightPin, int SpeedPin, int PotPin) : MotorController(LeftPin, RightPin, SpeedPin)
  {
    this->PotPin = PotPin;
    Mode = ServoController::STOP;
    this->MinPotValue = 512;
    this->MaxPotValue = 512;
    this->MaxSpeed = 7;
    this->MaxPower = 18;
    this->SpeedFactor = 1;
    this->PowerBoost = 0;
    this->PosOffset = 0;
    this->AngleOffset = 0; 
 
    this->Pid.Kp = 1;
    this->Pid.Ki = .5;
    this->Pid.Kd = .1;
    this->Pid.MinI = -15;
    this->Pid.MaxI = 15;
    this->Pid.LastPos = this->CurrentPos();
    this->PidCycle = 20;
	 this->DisplayFunc = NULL;
	 this->StatusFunc = NULL;
	 this->DisplayCycle = 250;
	 this->LastDisplayMillis = 0;
	 this->Req = NULL;
	 this->AngleFactor = 4.5;
  }
  
  //----------------------------------------

	void ServoController::Command(RobotArmRequest &Req)
	{
		String Cmd;
		this->Req = &Req;
		if (strlen(Req.Action) == 1)
		{
			Command(Req.Action[0], Req.FloatValue);
		}
		else
		{
		  
			
			Cmd = Req.Action;
			Cmd.toLowerCase();
			if (Cmd == "kp")
			{
				this->Pid.Kp = Req.FloatValue;
			}
			else if (Cmd == "ki")
			{
				this->Pid.Ki = Req.FloatValue;
			}
			else if (Cmd == "kd")
			{
				this->Pid.Kd = Req.FloatValue;
			}
			else if (Cmd == "mini")
			{
				this->Pid.MinI = Req.Value;
			}
			else if (Cmd == "maxi")
			{
				this->Pid.MaxI = Req.Value;
			}
		}
	}

  //----------------------------------------
  
  void ServoController::Command(byte Action, float Value)
  {
    //sprintf(Output, "Action: %c, Value: %d", Action, Value);
    //Serial.println(Output);
    switch (Action)
    {
      case 'A':
        this->SetAngle(Value);
        break;
        
      case 'I':
        this->Initialize();
        break;
        
      case 'S':
        this->MaxSpeed = Value;
        
      case 'P':
        this->MaxPower = Value;
        
      case '!':
        this->Stop();
        break; 

		case '?':
		  //Serial.print("~inquiry!");
		  if (this->StatusFunc != NULL)
		  {
			  this->StatusFunc(*this);
		  }
		  break;
    } 
  }
  
  //----------------------------------------
  
  void ServoController::DoWork()
  {
    switch (this->Mode)
    {
      case GET_MIN_MAX:
        GetMinMax();
        break;
      
      case SEEKING:
        Seek();
        break;
        
      case STOP:
        this->Brake();
        break;
      
    }
	 this->DoDisplay();
  }
  
  //----------------------------------------
  
  void ServoController::GetMinMax()
  {
    int CurrentValue;
    CurrentValue = analogRead(this->PotPin);
    if (CurrentValue < this->MinPotValue)
    {
      this->MinPotValue = CurrentValue;
    }
    if (CurrentValue > this->MaxPotValue)
    {
      this->MaxPotValue = CurrentValue;
    }
    //sprintf(Output, "Min: %d, Max: %d", this->MinPotValue, this->MaxPotValue);
    //Serial.println(Output);
  }
  
  //----------------------------------------
  
  void ServoController::Initialize()
  {
    this->MaxPotValue = 512;
    this->MinPotValue = 512;
    this->Mode = GET_MIN_MAX;
    this->Brake();
  }
  
  //----------------------------------------
  
  void ServoController::Stop()
  {
    this->Mode = STOP;
    this->Brake();
    //Serial.println("STOP!!!");
  }
 
  //----------------------------------------
 
  void ServoController::SetAngle(float Angle)
  {
    if (Angle >= this->AngleOffset and Angle < 180)
    {
		this->TargetAngle = Angle;
      float Range;
      Range = this->MaxPotValue - this->MinPotValue;
      this->AngleFactor = Range / (180 - this->AngleOffset);
      this->StartPos = analogRead(this->PotPin);
      this->SeekPos = (Angle - this->AngleOffset) * this->AngleFactor + this->MinPotValue;
      this->Mode = SEEKING;
      this->SpeedFactor = (this->MaxPotValue - this->MinPotValue) / this->MaxSpeed;
      this->LastPos = -1;
      this->LastMillis = millis();
      this->Pid.LastPos = this->CurrentPos();
      this->Pid.SetPos = this->SeekPos;
    }
  }
  
  //----------------------------------------
  
  void ServoController::Seek()
  {
    SetPos();
  
    //sprintf(Output, "Seek Pos: %d, Current Pos: %d, Speed: %d", this->SeekPos, Pos, this->SpeedMeter.Speed);
    //Serial.println(Output);
  }
  
  //----------------------------------------
  
  int ServoController::CurrentPos()
  {
    return analogRead(this->PotPin);
  }

  //----------------------------------------
  
  int ServoController::CurrentAngle()
  {
	  float Angle;
	  Angle = (this->CurrentPos() - this->MinPotValue) / this->AngleFactor + this->AngleOffset;
	  if (Angle < 0)
	  {
		  Angle = 0;
	  }
    return Angle;
  }

  //----------------------------------------

  void ServoController::SetPos()
  {
     int Power;
     
     if (millis() - this->LastMillis >= this->PidCycle && this->Mode != STOP)
     {
       Power = this->Pid.Update(CurrentPos());
       this->SetDynamicPower(Power);
       this->LastMillis = millis();
     }
  }

  //----------------------------------------

	void ServoController::SetDisplayFunc(void (*Func)(ServoController &))
	{
		this->DisplayFunc = Func;
	}

  //----------------------------------------

	void ServoController::SetStatusFunc(void (*Func)(ServoController &))
	{
		this->StatusFunc = Func;
	}

  //----------------------------------------

	void ServoController::DoDisplay()
	{
		if (this->DisplayFunc != NULL)
		{
			if (millis() - this->LastDisplayMillis > this->DisplayCycle)
			{
				this->DisplayFunc(*this);
				this->LastDisplayMillis = millis();
			}
		}
	}

  //----------------------------------------

	void ServoController::SendMessage(String Msg)
	{
	  Serial.print("~");
	  Serial.print(Msg);
	  Serial.print("!");
	}



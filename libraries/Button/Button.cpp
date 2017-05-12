#include "Arduino.h"
#include "Button.h"

Button::Button()
  {
    this->ReleaseAngle = 0;
    this->PressAngle = 50;
    this->PressDelay = 750;
    this->Pressed = 0;
    this->Initialized = 0;
    this->Mode = STOP;
  }
  
  //----------------------------------------

  void Button::Press()
  {
    if (Mode == STOP)
    {
      Mode = PRESS;
    }
  }
  
  //----------------------------------------

  void Button::Depress()
  {
    this->write(this->PressAngle);
    this->LastMillis = millis();
  }
  
  //----------------------------------------
   
  void Button::Release() 
  {
    this->write(this->ReleaseAngle);
    this->LastMillis = millis();
  }

  //----------------------------------------
  
  void Button::DoWork()
  {
    switch(Mode)
    {
      case PRESS:
        this->Depress();
        Mode = PRESS_DELAY;
        break;
        
      case PRESS_DELAY:
        if (millis() - this->LastMillis >= this->PressDelay)
        {
          Mode = RELEASE;
        }
        break;
        
      case RELEASE:
        this->Release();
        Mode = RELEASE_DELAY;
        break;
        
      case RELEASE_DELAY:
        if (millis() - this->LastMillis >= this->PressDelay)
        {
          Mode = STOP;
        }
        break;
        
       default:
         //this->write(this->ReleaseAngle);
         break;
        
    }
  }


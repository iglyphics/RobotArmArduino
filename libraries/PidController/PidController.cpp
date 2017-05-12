#include "Arduino.h"
#include "PidController.h"

int PidController::Update(double Pos)
{
    double E;
    double P;
    double I;
    double D;
    int Power;
    
    E = this->SetPos - Pos;
    P = E * this->Kp;
    this->SumI += E;
    if (this->SumI < this->MinI)
    {
      this->SumI = this->MinI;
    }
    else if (this->SumI > this->MaxI)
    {
      this->SumI = this->MaxI;
    }
    I = this->SumI * Ki;
    D = (Pos - this->LastPos) * Kd;
    this->LastPos = Pos;
    
    Power = (P + I + D) / 8;
    
	 return Power;
}





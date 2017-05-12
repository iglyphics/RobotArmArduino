#include "Arduino.h"
#include "WireServer.h"
#include <Wire.h>

WireServer::ServerState WireServer::State = WireServer::StartListen;
WireServer::ServerMode WireServer::Mode = WireServer::NoReply;
void (*WireServer::user_onReceive)(void *, int);
uint8_t WireServer::Buffer[255];

WireServer::WireServer(int Address, void (*user_func)(void *, int), WireServer::ServerMode ModeIn)
{
    this->Address = Address;
    user_onReceive = user_func;
    this->SendMsgLength = 0;
    this->SendMsgPtr = NULL;
    Mode = ModeIn;
    State = WireServer::StartListen;
}

void WireServer::Run()
{
    switch (State)
    {
        case WireServer::StartListen:
            //Serial.println("Listening for request");
            Wire.begin(this->Address);
            Wire.onReceive(OnWireReceive);
            this->State = WireServer::Listening;
            break;

        case WireServer::WaitForReply:
            if (this->SendMsgLength > 0)
            {
                //Serial.println("Sending reply");
                Wire.begin();
                Wire.beginTransmission(this->Address);
                Wire.write(this->OutBuffer, this->SendMsgLength);
                Wire.endTransmission(true);
                this->SendMsgLength = 0;
                State = WireServer::StartListen;
            }
            break;
    }
}

void WireServer::Send(uint8_t *Ptr, int Len)
{
    //Serial.println("Got Server Send");
    memcpy(this->OutBuffer, Ptr, Len);
    this->SendMsgLength = Len;
}

void WireServer::OnWireReceive(int Length)
{
    //Serial.println("Got request");
    int Cnt = 0;
      while (Cnt < Length)
      {
        if (Wire.available())
        {
          Buffer[Cnt] = Wire.read();
          Cnt++;
        }
      }
    user_onReceive((void *)Buffer, Length);
    if (Mode == WireServer::SendReply)
    {
       // Serial.println("Set state to wait for reply");
        State = WireServer::WaitForReply;
    }
    else
    {
       //Serial.println("Set state to start listen");
        State = WireServer::StartListen;
    }
        
}






#ifndef WireServer_h
#define WireServer_h

#include "Arduino.h"

class WireServer
{
    public:
        enum ServerMode { NoReply, SendReply };
        enum ServerState {StartListen, Listening, WaitForReply };
        int Address;
        boolean ReplyAvailable;
        int ReplyLength;
        WireServer(int Address, void (*user_onReceive)(void *, int), WireServer::ServerMode Mode);
        void Run();
        void Send(uint8_t *Ptr, int Len);
        



    private:
        static WireServer::ServerState State;
        static WireServer::ServerMode Mode;
        static void (*user_onReceive)(void *, int);
        static void OnWireReceive(int);
        static uint8_t Buffer[255];
        int SendMsgLength;
        uint8_t *SendMsgPtr;
        uint8_t OutBuffer[255];
    
       
};

#endif

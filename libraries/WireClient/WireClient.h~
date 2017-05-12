#ifndef WireClient_h
#define WireClient_h

#include "Arduino.h"

class WireClient
{
    public:
        enum ClientState {ClearToSend, WaitingForReply};
        int Address;
        WireClient(int Address, void (*user_onReceive)(void *, int));
        WireClient(int Address);
        void Send(uint8_t *Ptr, int Len);
        int IsClearToSend();
        static WireClient::ClientState State;
        int MaxReplyWait;
        void Run();


    private:
        static void (*user_onReceive)(void *, int);
        static void OnWireReceive(int);
        static char Buffer[255];
        unsigned long ReplyStart;
        char ReqBuffer[255];
        int ReqLength;
    
       
};

#endif

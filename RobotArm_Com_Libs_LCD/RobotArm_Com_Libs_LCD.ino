
#include <Servo.h>
#include <Messenger.h>
#include <LiquidCrystal.h>
#include <LcdScreen.h>
#include <PidController.h>
#include <MotorController.h>
#include <ServoController.h>
#include <Button.h>
#include <TurnTableController.h>
#include <LcdField.h>
#include <string.h>
#include <RobotArmRequest.h>
#include <stdio.h>
#include <Wire.h>


//void* operator new(size_t size) { return malloc(size); }
//void operator delete(void* ptr) { free(ptr); } 

//#include <Ethernet.h>

#define REQ_QUEUE_SIZE 25

//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//byte ip[] = { 192, 168, 1, 177 };
//Server server(80);
char Output[256];
Messenger Msg = Messenger(','); 
LcdScreen lcd(38, 42, 44, 46, 48, 50, 52);
//LcdField ElbowField(&lcd, 1, 0, "E:", 4);
//LcdField ShoulderField(&lcd, 0, 0, "S:", 4);
LcdField CmdField(&lcd, 2, 0, "Cmd:", 16);
LcdField MsgField(&lcd, 3, 0, "Msg:", 24);
int MsgCount;
int WireMsgLength;
int WireByteCount;
char ch;

//================================================================================

class JointStatus
{
  public:
  int CurAngle;
  int CurPower;
  int TargetAngle;
  LcdField *FldCurAngle;
  LcdField *FldTargetAngle;
  LcdField *FldCurPower;
  
  
  //----------------------------------------
  
  JointStatus(LcdScreen &Lcd, char *Name, int Row)
  {
    this->FldCurAngle = new LcdField(&Lcd, Row, 0, Name, 3);
    this->FldTargetAngle = new LcdField(&lcd, Row, 5, ":", 3);
    this->FldCurPower = new LcdField(&Lcd, Row, 9, ":", 4);
    //this->FldCurAngle->PrintLabel();
    //this->FldCurPower->PrintLabel();
    this->CurPower = 0;
    this->CurAngle = 0;
    this->TargetAngle = 0;
  }
  
  //----------------------------------------
  
  void Print()
  {
    this->FldCurAngle->Print(this->CurAngle);
    this->FldTargetAngle->Print(this->TargetAngle);
    this->FldCurPower->Print(this->CurPower);
  }
  
  //----------------------------------------
  
  void PrintLabel()
  {
    this->FldCurAngle->PrintLabel();
    this->FldTargetAngle->PrintLabel();
    this->FldCurPower->PrintLabel();
  }
  
  //----------------------------------------
  
  ~JointStatus()
  {
    delete this->FldCurAngle;
    delete this->FldTargetAngle;
    delete this->FldCurPower;
  }
  
  //----------------------------------------
};
    
JointStatus ShoulderStatus(lcd, "S:", 0);
JointStatus ElbowStatus(lcd, "E:", 1);

//================================================================================

//class ClientRequest
//{
//  public:
//  
//  byte  Joint;
//  char  Command[11];
//  byte  Action;
//  int   Value;
//};


//================================================================================


//================================================================================

class CameraController
{
  public:
  TurnTableController *TurnTable;
  Button *Shutter;
  enum Modes
  {
    STOP, CALIBRATE, BUTTON, TABLE, SEEK
  };
  int Mode;
  int TurnTableSpeed;
  
  //----------------------------------------
  
  CameraController(TurnTableController *t, Button *s)
  {
    TurnTable = t;
    Shutter = s;
    TurnTableSpeed = 5;
    
  }
  
  //----------------------------------------
  
  void TakePicture(int Value)
  {
    this->TurnTableSpeed = Value;
    this->Mode = BUTTON;
    this->Shutter->Press();
  }
  
  //----------------------------------------
  
  void Initialize()
  {
    this->Mode = CALIBRATE;
    this->TurnTable->Initialize();
  }
  
  //----------------------------------------
  
  void Command(RobotArmRequest &Req)
  {   
    //sprintf(Output, "Action: %c, Value: %d", Action, Value);
    //Serial.println(Output);
    switch (Req.Action[0])
    {      
      case '!':
        this->Stop();
        //this->Brake();
        break; 
        
      case 'T':
        this->TakePicture(Req.Value);
        break;
        
      case 'S':
        this->Mode = TABLE;
        this->TurnTable->StartSeek(Req.Value);
        break;
        
      case 'I':
        this->Initialize();
        break;
    } 
  }
  
  //----------------------------------------
  
  void Stop()
  {
    Mode = STOP;
    this->TurnTable->Stop();
  }
  
  //----------------------------------------
  
  void DoWork()
  {
    switch(this->Mode)
    {
      case CALIBRATE:
        this->TurnTable->Calibrate();
        break;
        
      case BUTTON:
        this->Shutter->DoWork();
        if (this->Shutter->Mode == Button::STOP)
        {
          Mode = TABLE;
          this->TurnTable->StartSeek(this->TurnTableSpeed);
        }
        break;
         
      case TABLE:
        this->TurnTable->DoWork();
        if (this->TurnTable->Mode == TurnTableController::STOP)
        {
          Mode = STOP;
          Serial.println("~ok!");
        }
        break;
        
    }
  }
};

//================================================================================

ServoController Shoulder(2, 4, 3, 0);
ServoController Elbow(7, 8, 5, 1);
TurnTableController TurnTable(12, 13, 11, 2);
Servo Wrist;
//Servo Shutter;
Button Shutter;
CameraController Camera(&TurnTable, &Shutter);
char ActionStr[11];
float WristAngleOffset = 15;
float WristAngleFactor = 1;

//================================================================================

void DisplayElbow(ServoController &sc)
{
  ElbowStatus.CurAngle = sc.CurrentAngle();
  ElbowStatus.CurPower = sc.Power;
  ElbowStatus.Print();
  //ElbowField.Print(Ac);
  //CmdField.Print(Req.Command);
}

//----------------------------------------

void DisplayShoulder(ServoController &sc)
{
  ShoulderStatus.CurAngle = sc.CurrentAngle(); //Ac;
  ShoulderStatus.CurPower = sc.Power;
  ShoulderStatus.Print();
  //ShoulderField.Print(Ac);
  //CmdField.Print(Req.Command);
}

//----------------------------------------

void SendElbowStatus(ServoController &sc)
{
  SendServoStatus("E", sc);
}

//----------------------------------------

void SendShoulderStatus(ServoController &sc)
{
 
  SendServoStatus("S", sc);
}

//----------------------------------------

void SendServoStatus(char *Name, ServoController &sc)
{
  char Line[80];
  char Kp[10];
  char Ki[10];
  char Kd[10];
  
  ftoa(sc.Pid.Kp, 4, Kp);
  ftoa(sc.Pid.Ki, 4, Ki);
  ftoa(sc.Pid.Kd, 4, Kd);
  
  memset(Line, 0, sizeof(Line));
  //MsgField.Print(MsgCount);
  //Serial.print("~test!");
  sprintf(Line, "?,%s,%d,%d,%s,%s,%s", 
      Name, 
      sc.CurrentAngle(), 
      sc.MaxPower,
      Kp,
      Ki,
      Kd
      );
  SendMessage(Line);
}

//----------------------------------------


//----------------------------------------

char *ftoa(double fval, int dec, char *str)
{
  int ival;
  int Len = 0;
  int PadLen = 0;
  char DecStr[21];
  
  if (dec > 20)
  {
    dec = 20;
  }
  
  if (fval < 0)
  {
    str[0] = '-';
    Len++;
  }
  ival = int(fval);
  itoa(ival, str + Len, 10);
  fval = fval - ival;
  ival = int((fval * double(pow(10.0, dec + 1)) + 5) / 10);
  
  if (ival > 0)
  {
    strcat(str, ".");
    Len = strlen(str);
    itoa(ival, DecStr, 10);
    PadLen = dec - strlen(DecStr);
    memset(str + Len, '0', PadLen);
    strcpy(str + Len + PadLen, DecStr);
  }
 
  return str;
}

//================================================================================

void SendMessage(char *Msg)
{
  Serial.print("~");
  Serial.print(Msg);
  Serial.print("!");
}

void SendMessage(String Msg)
{
  Serial.print("~");
  Serial.print(Msg);
  Serial.print("!");
}

//================================================================================

void ProcessMessage()
{
  RobotArmRequest Req;
  int WristAngle;
  char Joint;
  char  Line[21];
  char Value[21];
  
    
  
  memset(&Req, 0, sizeof(RobotArmRequest));
  memset(Line, 0, sizeof(Line));
  memset(Req.Joint, 0, sizeof(Req.Joint));
  Msg.copyString(Req.Joint, sizeof(Req.Joint) - 1);
  Msg.copyString(Req.Action, sizeof(Req.Action) - 1);
  Msg.copyString(Value, sizeof(Value) - 1);
  Req.Value = atoi(Value); //Msg.readInt();
  Req.FloatValue = strtod(Value, NULL);
  //SendMessage(Req.FloatValue);
  Joint = Req.Joint[0];
  
  
  
  sprintf(Line, "%s,%s,%s", Req.Joint, Req.Action, Value);
  CmdField.Print(Req.Joint);
  
  MsgField.Print(Line);
 
 
  if (Joint == 'E' || Joint == '*')
  {
    Elbow.Command(Req);
    //Elbow.Command(Req.Action, Req.Value);
  }
  if (Joint == 'S' || Joint == '*')
  {
    Shoulder.Command(Req);
    
    //Shoulder.Command(Req.Action, Req.Value);
  }
  if (Joint == 'T' || Joint == '*')
  {
    TurnTable.Command(Req);
  }
  if (Joint == 'C' || Joint == '*')
  {
    Camera.Command(Req);
  }
  if (Joint == 'B')
  {
    Shutter.Press();
  }
  if (Joint == 'W' || Joint == '*')
  {
    if (Req.Action[0] == 'A')
    {
      WristAngle = Req.Value * WristAngleFactor;
      WristAngle -= WristAngleOffset;
      
      if (WristAngle < 10)
      {
        WristAngle = 10;
      }
      else if (WristAngle > 180)
      {
        WristAngle = 180;
      }
      Wrist.write(WristAngle);
    }
    else if (Req.Action[0] == 'O')
    {
      WristAngleOffset = Req.FloatValue;
    }
    else if (Req.Action[0] == 'F')
    {
      WristAngleFactor = Req.FloatValue;
    }
  }
}

//================================================================================

void setup() 
{ 
  MsgCount = 0;
  
  TCCR1B = TCCR1B & 0b11111000 | 0x02;
  TCCR2B = TCCR2B & 0b11111000 | 0x02;
  
  lcd.begin(20, 4, __FILE__, __DATE__, 5000);
  
  Elbow.Stop();
  Shoulder.Stop();
  TurnTable.Stop();
  Wrist.attach(9);
  Wrist.write(90);
  
  Shutter.attach(10);
  Shutter.Release();
  
  Wire.begin(); 
  WireMsgLength = 6;
  WireByteCount = 0;
  
  Serial.begin(115200);
  Msg.attach(ProcessMessage);
  while(Serial.available())
  {
    Serial.read();
  }
  
  //sprintf(Output, "Left Pin: %d\nRight Pin: %d\nSpeed Pin: %d\nPot Pin: %d\n", Elbow.LeftPin, Elbow.RightPin, Elbow.PowerPin, Elbow.PotPin);
  //Serial.println(Output);
  Elbow.AngleOffset = 6.1;
  Elbow.SetDisplayFunc(DisplayElbow);
  Elbow.SetStatusFunc(SendElbowStatus);
  //ElbowField.PrintLabel();
  ElbowStatus.PrintLabel();
  Elbow.MinPotValue = 35;
  Elbow.MaxPotValue = 879;
  
  
  Shoulder.SetDisplayFunc(DisplayShoulder);
  Shoulder.SetStatusFunc(SendShoulderStatus);
  //ShoulderField.PrintLabel();
  ShoulderStatus.PrintLabel();
  Shoulder.MinPotValue = 172;
  Shoulder.MaxPotValue = 1023;
  
  CmdField.PrintLabel();
  MsgField.PrintLabel();
  //Serial.println("Setup complete");
  
  //Ethernet.begin(mac, ip);
  //server.begin();
 
} 


//================================================================================

void loop() 
{ 
  if (Serial.available())
  {
    Msg.process(Serial.read());
  }
  
  if (WireByteCount == 0)
  {
    Wire.requestFrom(2, WireMsgLength);
    WireByteCount = WireMsgLength;
  }
  
  if (Wire.available())
  {
    ch = Wire.read();
    Serial.print(ch);
    WireByteCount--;
  }
  
  
  Shoulder.DoWork();
  Elbow.DoWork();
  Camera.DoWork();
  //TurnTable.DoWork();
  //Shutter.DoWork();

} 

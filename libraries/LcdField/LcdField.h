#ifndef LCDFIELD_H
#define LCDFIELD_H

#include <LiquidCrystal.h>
#include "string.h"
#include "Arduino.h"

class LcdField
{
  public:
  int Row;
  int Col;
  char Label[21];
  int ValuePos;
  int ValueLen;
  boolean RightJustify;
  LiquidCrystal *lcd;
  char StrVal[21];
  char Spaces[21];
  
  LcdField(LiquidCrystal *lcd, int Row, int Col, char *Label, int ValueLen);
  
  //----------------------------------------
  
  void PrintLabel();
  void Print(char *Value);
  void Print(int Value);
  void Print(String &);
};

#endif


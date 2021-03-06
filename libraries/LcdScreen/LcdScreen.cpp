#include "Arduino.h"
#include "LiquidCrystal.h"
#include "LcdScreen.h"

	LcdScreen::LcdScreen(uint8_t rs, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) : LiquidCrystal(rs, enable, d0, d1, d2, d3, d4, d5, d6, d7) {}

   LcdScreen::LcdScreen(uint8_t rs, uint8_t rw, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) : LiquidCrystal(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7) {}

   LcdScreen::LcdScreen(uint8_t rs, uint8_t rw, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) : LiquidCrystal(rs, rw, enable, d0, d1, d2, d3) {}

   LcdScreen::LcdScreen(uint8_t rs, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) : LiquidCrystal(rs, enable, d0, d1, d2, d3) {}

void LcdScreen::begin(int col, int row, char *file, char *date, int msecs)
{
	LiquidCrystal::begin(col, row);
   this->setCursor(0, 0);
   String Filename = file;
   Filename = Filename.substring(Filename.lastIndexOf("\\") + 1);
   this->print(Filename);
   this->setCursor(0, 1);
   this->print(date);
   delay(msecs);
   this->clear();
}

void LcdScreen::begin(int col, int row, char *file, char *date)
{
	this->begin(col, row, file, date, 15000);
}

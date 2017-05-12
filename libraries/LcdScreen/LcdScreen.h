#ifndef LCDSCREEN_H
#define LCDSCREEN_H

#include "Arduino.h"
#include "LiquidCrystal.h" 

 
class LcdScreen : public LiquidCrystal
{
	public:
	LcdScreen(uint8_t rs, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
   LcdScreen(uint8_t rs, uint8_t rw, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
   LcdScreen(uint8_t rs, uint8_t rw, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
   LcdScreen(uint8_t rs, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

	void begin(int col, int row, char *file, char *date);
	void begin(int col, int row, char *file, char *date, int msecs);
};

#endif


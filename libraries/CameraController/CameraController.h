#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "Arduino.h"
#include "Button.h"
#include "TurnTableController.h"

class CameraController
{
	public:
		Button *Shutter;
		TurnTableController *TurnTable;


	CameraController();
	CameraController(Button *button, TurnTable *turntable);





#endif

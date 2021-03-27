/*
 Name:		Pin_hole_MB1588.ino
 Created:	27.03.2021 12:48:45
 Author:	Ulf
*/


#include "Sensor_C11740.h"

enum Sensor_Status {testen,messen,error };

Sensor_C11740 Sensor;



void setup()
{
	Sensor.Begin();

	
	Serial.begin(115200);
	Serial.println(" start Pin Hole");
}


void loop() 
{
	Sensor.Test_InPut_Signal();
}

/*
 Name:		Pin_hole_MB1588.ino
 Created:	27.03.2021 12:48:45
 Author:	Ulf
*/


#include "Sensor_C11740.h"

enum Sensor_Status { Warten,  TesteInput, TesteOutput, TestSensor, Messen, Error, Debug };

Sensor_C11740 Sensor;

Sensor_Status Status;
byte incomingByte;
const int BUFFER_SIZE = 50;
char buffer[BUFFER_SIZE];
char oldBefehl[10] ="";


void setup()
{
	Sensor.Begin();

	
	Serial.begin(115200);
	Serial.println(">_ start Pin Hole V1.05");
	Serial.println(">_ ");

}


void loop() 
{
	checkSerial();
	switch (Status)
	{
	case TesteInput:
		Sensor.Test_Input_Signal();
		//Status = Warten;
		break;
	case TesteOutput:
		Sensor.Test_Output_Signal(buffer);
		break;
	case TestSensor:
		Sensor.Test_Sensor_();
		break;
	case Messen:
		LoopMessung();
		break;
	case Error:
		Sensor.Show_Error();
		break;
	case Debug:
		if (strcmp(buffer, "_DB_0") == 0)	Sensor.Show_SIG_Wert();
		if (strcmp(buffer, "_DB_1") == 0)	LoopKurveMessung();
		if (strcmp(buffer, "_DB_2") == 0)	LoopDebugMessung();
		Status = Warten;
		break;
	case Warten:
	default:
		break;
	}
}

void LoopMessung()
{
	Sensor.GoMessung();
	do
	{
		unsigned long T = millis();
		while (Sensor.isMessung(T))
		{}
		Sensor.Send_HoleSize();
		checkSerial();
	} while (Status == Messen);
	Sensor.StopMessung();
}

void LoopDebugMessung()
{
	Sensor.GoMessung();
	unsigned long T = millis();
	while (Sensor.isDebugMessung(T))
	{	}
	Sensor.Send_HoleSizeDebug();
	Sensor.StopMessung();
}

void LoopKurveMessung()
{
	Sensor.GoMessung();
	unsigned long T = millis();
	while (Sensor.isKurveMessung(T))
	{
	}
	Sensor.StopMessung();
}

void checkSerial()
{
	if (Serial.available() > 0)
	{
		char Befehl[4] = { 0,0,0,0 };
		byte index = 0;
		while (index < BUFFER_SIZE)
		{
			int c = Serial.read();
			if (c != -1)
			{
				if (c == '\n')
				{
					buffer[index] = 0;
					break;
				}
				if (c >= 0)
				{
					buffer[index] = (char)c;
					index++;
				}
			}
		}
		Serial.print(">");
		Serial.print(buffer);
		Serial.print("\n");
		strncpy(Befehl, buffer, 3);
		if (strcmp(Befehl, "_SW") == 0)	{ Status = TesteOutput; }
		if (strcmp(Befehl, "_TI") == 0) { Status = TesteInput; }
		if (strcmp(Befehl, "_TS") == 0) { Status = TestSensor; }
		if (strcmp(Befehl, "_ME") == 0) { Status = Messen; }
		if (strcmp(Befehl, "_ER") == 0) { Status = Error; }
		if (strcmp(Befehl, "_DB") == 0) { Status = Debug; }
		if (strcmp(Befehl, "_WA") == 0) { Status = Warten; }
	}
}
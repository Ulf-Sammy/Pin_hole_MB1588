#include "Sensor_C11740.h"

#define get_input(A) (!digitalRead(A))


Sensor_C11740::Sensor_C11740()
{
}

void Sensor_C11740::Begin()
{
	pinMode(C11740_RR, INPUT);
	pinMode(C11740_SO, INPUT);
	pinMode(C11740_OR, INPUT);
	pinMode(C11740_PIN, INPUT);
	pinMode(C11740_SCLOUT, INPUT);
	pinMode(CLOCK, INPUT);
	pinMode(OBJECT, INPUT);

	pinMode(C11740_SCL, OUTPUT);
	pinMode(C11740_SENS, OUTPUT);
	pinMode(C11740_GATE, OUTPUT);
	pinMode(READY, OUTPUT);
	pinMode(DEFECT, OUTPUT);

	digitalWrite(C11740_SCL, LOW);
	digitalWrite(C11740_SENS, LOW);
	digitalWrite(C11740_GATE, LOW);
	digitalWrite(READY, LOW);
	digitalWrite(DEFECT, LOW);
}

bool Sensor_C11740::DoMessung()
{
	if (get_input(OBJECT))
	{
		AnalogVal = analogRead(C11740_SIG);
		HoleSize += AnalogVal;
		return true;
	}
	else
	{
		return false;
	}
}

void Sensor_C11740::Test_InPut_Signal()
{
	Show_Signal(" Signal R.R.   = ", &Bit_RR,     C11740_RR);
	Show_Signal(" Signal S.O.   = ", &Bit_SO,     C11740_SO);
	Show_Signal(" Signal O.R.   = ", &Bit_OR,     C11740_OR);
	Show_Signal(" Signal PIN    = ", &Bit_PIN,    C11740_PIN);
	Show_Signal(" Signal SCLOUT = ", &Bit_SCLOUT, C11740_SCLOUT);
	Show_Signal(" Signal CLOCK  = ", &Bit_CLOCK,  CLOCK);
	Show_Signal(" Signal OBJECT = ", &Bit_OBJECT, OBJECT);
}

void Sensor_C11740::Show_SIG_Wert()
{
	word Val = analogRead(C11740_SIG);
	if ((Val - AnalogVal) > 20)
	{
		AnalogVal = Val;
		Serial.print(" SIG Val = ");
		Serial.println(AnalogVal);
	}
}

void Sensor_C11740::Show_HoleSize()
{
	Serial.print(" HoleSize = ");
	Serial.println(HoleSize);
	HoleSize = 0; // nach dem Zeigen werd zurücksetzen
}

void Sensor_C11740::Show_Error()
{
	if ( get_input(C11740_RR)) Serial.println(" ERROR 01   = light-shielding ");
	if (!get_input(C11740_SO)) Serial.println(" ERROR 02   = hight-voltage   ");
	if ( get_input(C11740_OR)) Serial.println(" ERROR 03   = over-light / over Range ");
}

void Sensor_C11740::Show_Signal(const char *Txt, bool *B, unsigned int  Pin)
{
	bool bit;
	bit = get_input(Pin);
	if (bit != *B)
	{
		*B = bit;
		Serial.print(Txt);
		if (*B) Serial.println("ON ");
		else    Serial.println("OFF");
	}
}

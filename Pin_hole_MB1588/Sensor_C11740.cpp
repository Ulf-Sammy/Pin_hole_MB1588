#include "Sensor_C11740.h"

#define get_input(A) (!digitalRead(A))


Sensor_C11740::Sensor_C11740()
{
	Bit_OBJECT = false;
	Bit_CLOCK = false;
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
	pinMode(C11740_POT_CS, OUTPUT);
	pinMode(C11740_POT_RST, OUTPUT);
	pinMode(READY, OUTPUT);
	pinMode(DEFECT, OUTPUT);

	SPI.begin();
	digitalWrite(C11740_POT_RST, LOW);
	delay(1); // Do Rest
	digitalWrite(C11740_POT_RST, HIGH);
	digitalWrite(C11740_SCL, LOW);
	digitalWrite(C11740_SENS, LOW);
	digitalWrite(C11740_GATE, LOW);
	digitalWrite(C11740_POT_CS, LOW);
	digitalWrite(READY, LOW);
	digitalWrite(DEFECT, LOW);
}

bool Sensor_C11740::GoMessung()
{
	while (get_input(OBJECT))
	{	}
	digitalWrite(C11740_SENS, HIGH); 
	digitalWrite(READY, HIGH);
}

void Sensor_C11740::StopMessung()
{
	digitalWrite(C11740_SENS, LOW);
	digitalWrite(READY, LOW);
	digitalWrite(DEFECT, LOW);
	Messung = false;
}

bool Sensor_C11740::isMessung(unsigned long STime)
{
	
	if (Active_Messung == LOW && get_input(OBJECT))
	{
		digitalWrite(C11740_GATE,HIGH); // Hoch spannung einshalten
		digitalWrite(DEFECT, LOW);
		Active_Messung = HIGH;
		Messung = true;
		SumHoleSize = 0;
		Sampels = 0;
	}
	if (Active_Messung)
	{
		AnalogVal = analogRead(C11740_SIG);
		SumHoleSize += AnalogVal;
		Sampels++;
	}
	if (Active_Messung && get_input(OBJECT)== LOW)
	{
		digitalWrite(C11740_GATE, LOW);
		Active_Messung = LOW;
		return false;
	}
	return true;
}

bool Sensor_C11740::isDebugMessung(unsigned long STime)
{
	if (Active_Messung == LOW && get_input(OBJECT))
	{
		digitalWrite(C11740_GATE, HIGH); // Hoch spannung einshalten
		Start_time = millis();
		digitalWrite(DEFECT, LOW);
		Active_Messung = HIGH;
		Messung = true;
		SumHoleSize = 0;
		Sampels = 0;
	}
	if (Active_Messung)
	{
		AnalogVal = analogRead(C11740_SIG);
		SumHoleSize += AnalogVal;
		Sampels++;
	}
	if (Active_Messung && get_input(OBJECT) == LOW)
	{
		digitalWrite(C11740_GATE, LOW);
		End_time = millis();
		Active_Messung = LOW;
		return false;
	}
	return true;
}

bool Sensor_C11740::isKurveMessung(unsigned long STime)
{
	static bool Bit_Cyclus = false;
	static int Wert;
	if (Bit_Cyclus &&  get_input(CLOCK))
	{
		End_time = millis();
		Active_Messung = LOW;
		Bit_Cyclus = false;
		Serial.print(">_Ende :");
		Serial.print(End_time - Start_time);
		Serial.print(" ms \n");
		return false;
	}
	if (Active_Messung == LOW && get_input(OBJECT))
	{
		digitalWrite(C11740_GATE, HIGH); // Hoch spannung einshalten
		Start_time = millis();
		digitalWrite(DEFECT, LOW);
		Active_Messung = HIGH;
		Messung = true;
		SumHoleSize = 0;
		Sampels = 0;
	}
	if (Active_Messung)
	{
		AnalogVal = analogRead(C11740_SIG);
		AnalogVal = AnalogVal >> 1;
		//SumHoleSize += AnalogVal;
		Wert = 0;
		if (get_input(CLOCK))      Wert = 0x01;
		if (get_input(OBJECT))     Wert += 0x02;
		if (get_input(C11740_PIN)) Wert +=0x04;
		if (get_input(C11740_OR))  Wert +=0x08;
		Serial.print(Wert,HEX);
		Serial.print(AnalogVal);
		Serial.print('\n');
		Sampels++;
	}
	if (Active_Messung && get_input(OBJECT) == LOW)
	{
		digitalWrite(C11740_GATE, LOW);
		Bit_Cyclus = true;
	}
	return true;

}



void Sensor_C11740::Test_Input_Signal()
{
	Show_Signal(" Signal R.R.   = ", &Bit_RR,     C11740_RR);
	Show_Signal(" Signal S.O.   = ", &Bit_SO,     C11740_SO);
	Show_Signal(" Signal O.R.   = ", &Bit_OR,     C11740_OR);
	Show_Signal(" Signal PIN    = ", &Bit_PIN,    C11740_PIN);
	Show_Signal(" Signal SCLOUT = ", &Bit_SCLOUT, C11740_SCLOUT);
	Show_Signal(" Signal CLOCK  = ", &Bit_CLOCK,  CLOCK);
	Show_Signal(" Signal OBJECT = ", &Bit_OBJECT, OBJECT);
}

void Sensor_C11740::Test_Output_Signal(const char* Txt)
{
	if (strcmp(Txt, "_SW_SCL 0") == 0)	digitalWrite(C11740_SCL, LOW);	
	if (strcmp(Txt, "_SW_SCL 1") == 0)	digitalWrite(C11740_SCL, HIGH);	
	if (strcmp(Txt, "_SW_SENS 0") == 0) digitalWrite(C11740_SENS, LOW);
	if (strcmp(Txt, "_SW_SENS 1") == 0) digitalWrite(C11740_SENS, HIGH);
	if (strcmp(Txt, "_SW_GATE 0") == 0) digitalWrite(C11740_GATE, LOW);
	if (strcmp(Txt, "_SW_GATE 1") == 0) digitalWrite(C11740_GATE, HIGH);
	if (strcmp(Txt, "_SW_READY 0") == 0) digitalWrite(READY, LOW);
	if (strcmp(Txt, "_SW_READY 1") == 0) digitalWrite(READY, HIGH);
	if (strcmp(Txt, "_SW_DEFECT 0") == 0) digitalWrite(DEFECT, LOW);
	if (strcmp(Txt, "_SW_DEFECT 1") == 0) digitalWrite(DEFECT, HIGH);
}

void Sensor_C11740::Test_Sensor_()
{
}

void Sensor_C11740::Show_SIG_Wert()
{
	AnalogVal = analogRead(C11740_SIG); // 5V = 1023
	double Volt =(AnalogVal* 0.00489);
	Serial.print(" SIG Val = ");
	Serial.print(AnalogVal);
	Serial.print(" : ");
	Serial.print(Volt);
	Serial.println(" V");
}

void Sensor_C11740::Send_HoleSize()
{
	if (Messung)
	{
		HoleSize = ((100 * SumHoleSize) / Sampels);
		Serial.print(">_M ");
		Serial.println(HoleSize);
		if (HoleSize > 100) digitalWrite(DEFECT, HIGH);
		else digitalWrite(DEFECT, LOW);
		HoleSize = 0; // nach dem zeigen Wert zurücksetzen
		Sampels = 0; // auch zurücksetzen
	}
	else
		digitalWrite(DEFECT, LOW);
}

void Sensor_C11740::Send_HoleSizeDebug()
{
	HoleSize = ((100 * SumHoleSize) / Sampels);
	Serial.print(">_M ");
	Serial.print(HoleSize);
	Serial.print(" - Sampel : ");
	Serial.print(Sampels);
	Serial.print(" - Loch Wert: ");
	Serial.print(SumHoleSize);
	Serial.print(" - Zeit: ");
	Serial.print(End_time - Start_time);
	Serial.println(" ms");

	if (HoleSize > 100) digitalWrite(DEFECT, HIGH);
	else digitalWrite(DEFECT, LOW);
	HoleSize = 0; // nach dem zeigen Wert zurücksetzen
	Sampels = 0; // auch zurücksetzen
}

void Sensor_C11740::Show_Error()
{
	if ( get_input(C11740_RR)) Serial.println(" ERROR 01   = light-shielding ");
	//if (!get_input(C11740_SO)) Serial.println(" ERROR 02   = hight-voltage   ");
	if ( get_input(C11740_OR)) Serial.println(" ERROR 03   = over-light / over Range ");
}

void Sensor_C11740::digitalPotWrite(byte value)
{
	digitalWrite(C11740_POT_CS, LOW);

	delay(100);

	SPI.transfer(0x00);
	SPI.transfer(value);

	delay(100);

	digitalWrite(C11740_POT_CS, HIGH);
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

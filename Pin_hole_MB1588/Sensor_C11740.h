#pragma once
#include "Arduino.h"
#include <SPI.h>

// C11740 Hamamatsu Signale 
#define C11740_RR 2     // Detects system error               [OUTPUT]
#define C11740_SO 4     // Detects errors in high voltage     [OUTPUT]
#define C11740_SCLOUT 3 // results of detector check          [OUTPUT]
#define C11740_OR 5     // Over-light detection               [OUTPUT]
#define C11740_PIN 9    // Pinhole inspection Signal          [OUTPUT]

#define C11740_SCL 7   // turns the PMT operation check       [INPUT]
#define C11740_SENS 8  // turns power detection unit on|off   [INPUT]
#define C11740_GATE 6  // turns the high voltage on|off       [INPUT]      hier ist Shut 
// SPS Signale
#define CLOCK A2       // Clock Signal                        [OUTPUT]
#define OBJECT A1      // Object in Station (do measerment)   [OUTPUT]
#define READY A4       // Device is Ready to go               [INPUT]
#define DEFECT A3      // Set measerment-result to defect     [INPUT]
// analog Signal 
#define C11740_SIG A6  // detction unit analog signal output  [ANALOG_OUTPUT]
// analog Refence Signal
#define C11740_POT_CS 10 //Chip Select Poti Low active             []
#define C11740_POT_RST A0 //Reste Poti      Low active             []

#define MAX_CYCLE 600
class Sensor_C11740
{
public:
	Sensor_C11740();
	void Begin();
	bool GoMessung();
	bool isMessung(unsigned long STime);
	bool isDebugMessung(unsigned long STime);
	bool isKurveMessung(unsigned long STime);
	void StopMessung();
	void Test_Input_Signal();
	void Test_Output_Signal(const char* Txt);
	void Test_Sensor_();
	void Show_SIG_Wert();
	void Send_HoleSize();
	void Send_HoleSizeDebug();
	void Show_Error();
	void digitalPotWrite(byte value);
protected:
	void Show_Signal(const char *Txt, bool *B, unsigned int Pin);
	unsigned long SumHoleSize;
	unsigned int HoleSize;
	unsigned int Sampels;
	unsigned long Start_time; 
	unsigned long Gate_time;
	unsigned long End_time;

	word AnalogVal;  // Messwert vom Sensor SIG
	byte AnalogSet; //  ComperatorWert C.Ref
	bool Active_Messung;
	bool Messung;
	bool Bit_RR;
	bool Bit_SO;
	bool Bit_OR;
	bool Bit_PIN;
	bool Bit_SCLOUT;
	bool Bit_CLOCK;
	bool Bit_OBJECT;
};


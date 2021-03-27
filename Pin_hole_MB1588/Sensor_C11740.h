#pragma once
#include "Arduino.h"

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

class Sensor_C11740
{
public:
	Sensor_C11740();
	void Begin();
	bool DoMessung();
	void Test_InPut_Signal();
	void Show_SIG_Wert();
	void Show_HoleSize();
	void Show_Error();
protected:
	void Show_Signal(const char *Txt, bool *B, unsigned int Pin);
	unsigned long HoleSize;
	word AnalogVal;
	bool Bit_RR;
	bool Bit_SO;
	bool Bit_OR;
	bool Bit_PIN;
	bool Bit_SCLOUT;
	bool Bit_CLOCK;
	bool Bit_OBJECT;
};


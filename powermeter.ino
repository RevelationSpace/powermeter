// for ULONG_MAX
#include <limits.h>

// reads out up to 8 S0 outputs (from one I/O port) and dump falling edges to serial.

// constants
const int	NUM_INPUTS		= 3;		// number of inputs
const int	BAUD_RATE		= 115200;	// serial baud rate
const int	DEBOUNCE_INTERVAL	= 30;		// debounce interval in ms
const float	PULSE_ENERGY		= 1.8e6;	// energy per pulse in W*ms; 1.8MW*ms = 0.5W*h

// the PINx value all S0 inputs are on
#define INPUT_PIN PINB


// global state
// state for each S0 input
struct counter {
	// for debouncing
	bool		outValue;	// debounced value
	bool		lastValue;	// last actual pin value
	unsigned long	debounceTime;	// time lastValue has been stable for

	// for tracking time between pulses
	bool		initialized;	// true when lastTime is valid
	unsigned long	lastTime;	// last time 
} counters[NUM_INPUTS];

void setup() {
	Serial.begin(115200);
	// the relevant pins are inputs by default, so that's fine
	unsigned long time = millis();
	for(int i=0; i<NUM_INPUTS; i++) {
		counters[i].outValue	= true;
		counters[i].lastValue	= true;
		counters[i].debounceTime= time;
		counters[i].initialized	= false;
		counters[i].lastTime	= 0;
	}
}

void loop() {
	int d = INPUT_PIN;
	unsigned long time = millis();

	for(int i=0; i<NUM_INPUTS; i++) {
		bool v = INPUT_PIN & _BV(i);

		if(v != counters[i].lastValue)
			counters[i].debounceTime = time;

		if((time - counters[i].debounceTime) > DEBOUNCE_INTERVAL) {
			if(v != counters[i].outValue) {
				counters[i].outValue = v;
				if(!counters[i].outValue) {				// falling edge
					if(counters[i].initialized) {			// not the first falling edge, so we have a reference
						float power;
						if(time < counters[i].lastTime) {	// check for overflow since lastTime
							// we assume it's only overflowed once... more than 50 days between pulses seems a bit silly :)
							power = PULSE_ENERGY / ((float)ULONG_MAX - ((float)time - (float)counters[i].lastTime));
						} else {
							power = PULSE_ENERGY / ((float)time - (float)counters[i].lastTime);
						}

						Serial.print(i);
						Serial.print(": ");
						Serial.print(power);
						Serial.println("W");
					}

					counters[i].lastTime	= time;
					counters[i].initialized	= true;
				}
			}
		}
		counters[i].lastValue = v;
	}
}

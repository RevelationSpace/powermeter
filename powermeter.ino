// reads out up to 8 S0 outputs (from one I/O port) and dump falling edges to serial.

// constants
const int NUM_INPUTS		= 3;		// number of inputs
const int BAUD_RATE		= 115200;	// serial baud rate
const int DEBOUNCE_INTERVAL	= 30;		// debounce interval in ms

// TODO: check
#define INPUT_PIN PINB

// global state
struct counter {
	bool outValue;
	bool lastValue;
	unsigned long debounceTime;
} counters[NUM_INPUTS];	// pulse counter for each input

void setup() {
	Serial.begin(115200);
	// inputs are inputs by default -- this is fine
        unsigned long time = millis();
        for(int i=0; i<NUM_INPUTS; i++) {
                counters[i].outValue = true;
                counters[i].lastValue = true;
                counters[i].debounceTime = time;
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
				if(!counters[i].outValue) {	// falling edge
		                  Serial.print("Puls ");
                                  Serial.println(i);
				}
			}
		}
                counters[i].lastValue = v;
	}
}

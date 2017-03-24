#include "Particle.h"
#include "DS75-RK.h"

const unsigned long PRINT_PERIOD_MS = 5000;
unsigned long lastPrint = 0;

DS75 temp(Wire, 0);


void setup() {
	Serial.begin(9600);

	pinMode(D2, INPUT_PULLUP);

	temp.begin();

	// See comments in DS75-RK.h for what these calls do
	temp.setResolution(12);
	temp.setTOS(26.0);
	temp.setTHYST(25.0);

	temp.setInterruptMode(true);
}

void loop() {
	if (millis() - lastPrint >= PRINT_PERIOD_MS) {
		lastPrint = millis();

		float tempC = temp.getTemperature();

		Serial.printlnf("temp %.4fC %.3fF", tempC, (tempC * 9) / 5 + 32);
	}
	if (digitalRead(D2) == LOW) {
		Serial.printlnf("interrupt");

		// You must get the temperature (or setTOS or setTHYST) to clear the interrupt output
		temp.getTemperature();
	}
}


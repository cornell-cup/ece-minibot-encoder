#include <stdio.h>
#include <stdlib.h>

#include "mraa.h"
#include <time.h>
#include <unistd.h>

void ISR_PULSEIN();
long absoluteTime(void);

float CPR = 64.0;
float RPM;
long prevTime;
long currTime;
float usecPerCount = 0;


int main() {

#define PULSEIN_PIN		8

	currTime = 0;

	// Initialize GPIO pin for input pulse reading
	mraa_gpio_context PULSEIN = mraa_gpio_init(PULSEIN_PIN);
	if (PULSEIN == NULL){
		fprintf(stderr, "Could not initialize pulse input pin on GPIO %d\n", PULSEIN_PIN);
		return 1;
	}

	// Set pin direction to be input
	mraa_gpio_dir(PULSEIN, MRAA_GPIO_IN);

	// Attach ISR to pin
	mraa_gpio_isr(PULSEIN, MRAA_GPIO_EDGE_FALLING, ISR_PULSEIN, NULL);

	fprintf(stdout, "Done configuring!\n");

	while (1){
		fprintf(stdout, "usec/count: %.1f   RPM: %.1f\n", (float)usecPerCount, (float)RPM); // print in seconds
		sleep(1);
	}

	return 0;
}


void ISR_PULSEIN(void* args){

	prevTime = currTime;
	currTime = absoluteTime();
	usecPerCount = currTime - prevTime;
	RPM = 1000000.0 * 60.0 * ((1/usecPerCount)/CPR);

}

long absoluteTime(void){
	// Return absolute time in us
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	//	return (t.tv_nsec/1000);
	return ((long)t.tv_sec*1000+t.tv_nsec/1000);
}



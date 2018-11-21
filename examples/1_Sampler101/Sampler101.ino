// Example usage for Sampler library by calvin@particle.io.
#include "Sampler.h"

// Initialize objects from the lib
int bufferSize = 2*60; // Depends on the looping interval, in this case its ~2 minutes of buffer
int minPublishFrequency = 60*1000; // 1 minute
Sampler temperatureSampler("tempC", bufferSize, minPublishFrequency); 

int value = 0;
int stdDeviationTolerance = 3;

void setup() {
    // This function is useful for testing/demo-ing the functionality.
    // I am using the `random()` function to mock a sensor reading, so 
    // if you change `value` to anything > 3 then the `pretendTemperature`
    // should be 3 stdev outside of the average and you should see high
    // frequency publishs in the device cloud with the event name being "tempC".
    Particle.function("stepChange", stepChange);
}

void loop() {
    // Use the library's initialized objects and functions
    double pretendTemperature = random() + value;
    temperatureSampler.publish(pretendTemperature, stdDeviationTolerance);

    //delay for just a sec
    delay(1000);
}

//change the pretendTemperature by the input
int stepChange(String command) {
    value = command.toInt();
}
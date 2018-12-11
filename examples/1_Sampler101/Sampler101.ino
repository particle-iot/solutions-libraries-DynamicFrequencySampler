// Example usage for DynamicFrequencySampler library by calvin@particle.io.
#include "DynamicFrequencySampler.h"

// Initialize objects from the lib
SamplerSpec tempSpec { eventName: "tempC", method: "range", length: 20, minPublishFrequency: 60*1000, absValueChange: 3};

DynamicFrequencySampler temperatureSampler(tempSpec); 

int value = 0;

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
    temperatureSampler.publish(pretendTemperature);

    //delay for just a sec
    delay(1000);
}

//change the pretendTemperature by the input
int stepChange(String command) {
    value = command.toInt();
}
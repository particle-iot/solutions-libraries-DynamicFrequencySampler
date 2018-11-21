// returns the stdev of the data-set added sofar
#include "Sampler.h"
#include <stdlib.h>
#include "math.h"


Sampler::Sampler(const char *eventName, int size,  int minPublishFrequency) : RunningAverage(size)
{
    _minPublishFrequency = minPublishFrequency;
    _eventName = eventName;
    _debug = false;
    //Register a function that will allow you to 
    char functionName[12];
    sprintf(functionName,"debug_%.6s", eventName);
    Particle.function(functionName, &Sampler::toggleDebug, this);
}

int Sampler::toggleDebug(String dummy)
{
    _debug = !_debug;
    int ret = _debug ? 1 : 0;
    _debugIndex = 0;
    if (ret) {
        Serial.printlnf("~~~~~~ Debugging ~~~~~"); 
        Serial.printlnf("%s:   i,%.10s,%.10s,%.10s",_eventName,"latestValue", "lowerBound", "upperBound"); 
    }
    return ret;
}

double Sampler::getStd()
{
    if (_cnt == 0) return 0;
    double average = _sum / _cnt;
    double variance = 0;
    for(int i = 0; i < _cnt; i++) {
        variance += pow(_ar[i] - average, 2);
    }
    variance /= _cnt;
    return sqrt(variance);
}

void Sampler::publish(double latestValue, int tolerance)
{
    //add the latest value to the buffer.
    addValue(latestValue);
    //get some stats on the buffer
    double average = getAverage();
    double stdev = getStd();
    int timeSinceLast = millis() - _lastPublish;
    double upperBound = average + tolerance*stdev;
    double lowerBound = average - tolerance*stdev;
    
    if (_debug) {
        Serial.printlnf("%s:%5i,%10.3f,%10.3f,%10.3f",_eventName, _debugIndex, latestValue, lowerBound, upperBound); 
        _debugIndex++;
    }

    if (latestValue > upperBound || latestValue < lowerBound || timeSinceLast > _minPublishFrequency) {
        String eventString = String::format("{\"%s\":%f }", _eventName, latestValue);
        Particle.publish(_eventName, eventString, PRIVATE);
        _lastPublish = millis();
        if (_debug) {
            Serial.printlnf("* published '%s'*", _eventName);
        }
    }
}

void Sampler::publish(double latestValue, double lowerBound, double upperBound) 
{
    //even though we dont implicitly get stats on the buffer for absolute bounds
    //the user may want to in the calling program, so well add the value to to the buffer
    addValue(latestValue);

    int timeSinceLast = millis() - _lastPublish;
    
    if (_debug) {
        Serial.printlnf("%s: %4i, %10.3f, %10.3f, %10.3f",_eventName, _debugIndex, latestValue, lowerBound, upperBound); 
        _debugIndex++;
    }

    if (latestValue > upperBound || latestValue < lowerBound || timeSinceLast > _minPublishFrequency) {
        String eventString = String::format("{\"%s\":%f }", _eventName, latestValue);
        Particle.publish(_eventName, eventString, PRIVATE);
        _lastPublish = millis();
        if (_debug) {
            Serial.printlnf("* published '%s'*", _eventName);
        }
    }
}
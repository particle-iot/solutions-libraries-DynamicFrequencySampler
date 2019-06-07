// returns the stdev of the data-set added sofar
#include "DynamicFrequencySampler.h"
#include <stdlib.h>
#include "math.h"


DynamicFrequencySampler::DynamicFrequencySampler(const SamplerSpec &spec) : RunningAverage(spec.length)
{
    //setup protected variables
    _spec = spec;
    _spec.maxInterval  = _spec.maxInterval * 1000; // convert to millis
    _spec.minInterval  = _spec.minInterval * 1000; // convert to millis
    _debug = false;
    
    
}

void DynamicFrequencySampler::enableCloudFunctions()
{
    //Helpful for debugging
    //Register a Particle function that will print all calls to doPublish to Serial
    char functionName[12];
    sprintf(functionName,"debug_%.6s", _spec.eventName);
    Particle.function(functionName, &DynamicFrequencySampler::toggleDebug, this);
    
    //Register a Particle variable that will store the latest value
    char variableName[12];
    sprintf(variableName,"%.12s", _spec.eventName);
    Particle.variable(variableName, _particleVariable);
}

int DynamicFrequencySampler::toggleDebug(String dummy)
{
    _debug = !_debug;
    int ret = _debug ? 1 : 0;
    _debugIndex = 0;
    if (ret) {
        Serial.printlnf("~~~~~~ Debugging ~~~~~"); 
        Serial.printlnf("%s:   i,%.10s,%.10s,%.10s",_spec.eventName,"latestValue", "lowerBound", "upperBound"); 
    }
    return ret;
}

double DynamicFrequencySampler::getStd()
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

void DynamicFrequencySampler::publish(double latestValue)
{
    if (strcmp(_spec.method,"jump")) { 
        defineJumpLimits(latestValue, _spec.absValueChange);
    } else if (strcmp(_spec.method,"range")) {
        doPublish(latestValue, _spec.lower, _spec.upper);
    } else if (strcmp(_spec.method,"gaussian")) {
        defineGaussianLimits(latestValue, _spec.sigma);
    }
}

void DynamicFrequencySampler::defineGaussianLimits(double latestValue, double sigma) 
{
    //get some stats on the buffer
    double average = getAverage();
    double stdev = getStd();
    double upperBound = average + sigma*stdev;
    double lowerBound = average - sigma*stdev;
    
    doPublish(latestValue, lowerBound, upperBound);
}

void DynamicFrequencySampler::defineJumpLimits(double latestValue, double absValueChange) 
{
    double average = getAverage();
    double upperBound = average + abs(absValueChange);
    double lowerBound = average - abs(absValueChange);

    doPublish(latestValue, lowerBound, upperBound);
}

void DynamicFrequencySampler::doPublish(double latestValue, double lowerBound, double upperBound)
{
    //even though we dont implicitly get stats on the buffer for absolute bounds
    //the user may want to in the calling program, so well add the value to to the buffer
    addValue(latestValue);
    _particleVariable = latestValue;
    if (_debug) {
        Serial.printlnf("%s: %4i, %10.3f, %10.3f, %10.3f",_spec.eventName, _debugIndex, latestValue, lowerBound, upperBound); 
        _debugIndex++;
    }
    
    int timeSinceLast = millis() - _lastPublish;
    if (timeSinceLast > _spec.minInterval && (latestValue > upperBound || latestValue < lowerBound || timeSinceLast > _spec.maxInterval)) {
        String eventString = String::format("{\"%s\":%f }", _spec.eventName, latestValue);
        Particle.publish(_spec.eventName, eventString, PRIVATE);
        _lastPublish = millis();
        if (_debug) {
            Serial.printlnf("* published '%s'*", _spec.eventName);
        }
    }
}
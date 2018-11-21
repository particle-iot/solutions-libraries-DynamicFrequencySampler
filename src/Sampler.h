#ifndef Sampler_h
#define Sampler_h
#include "application.h" 
#include "RunningAverage.h" 

class Sampler : public RunningAverage
{
public:
    Sampler(const char *eventName, int size, int minPublishFrequency);
    double getStd();
    void publish(double latestValue, int tolerance);
    void publish(double latestValue, double lowerBound, double upperBound);
    int toggleDebug(String dummy);

protected: 
    int _lastPublish;
    int _minPublishFrequency;
    bool _debug;
    const char *_eventName;
    int _debugIndex;
};

#endif
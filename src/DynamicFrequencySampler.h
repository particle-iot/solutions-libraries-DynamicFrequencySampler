#ifndef DynamicFrequencySampler_h
#define DynamicFrequencySampler_h
#include "application.h" 
#include "RunningAverage.h" 

typedef struct
{
    const char *eventName;
    const char *method;
    int minInterval;
    int maxInterval;
    int length;
    double absValueChange;
    double lower;
    double upper;
    int sigma;
} SamplerSpec;

class DynamicFrequencySampler : public RunningAverage
{
public:
    DynamicFrequencySampler(const SamplerSpec &spec);
    double getStd();
    void publish(double latestValue);
    int toggleDebug(String dummy);
    
protected: 
    int _lastPublish;
    SamplerSpec _spec; 
    bool _debug;
    int _debugIndex;
    double _particleVariable;

private:
    void defineGaussianLimits(double latestValue, double sigma);
    void defineJumpLimits(double latestValue, double absValueChange);
    void doPublish(double latestValue, double lowerBound, double upperBound);
};

#endif
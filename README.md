# DynamicFrequencySampler

Create "dynamic" or "smart" publishing frequencies for sensor data that you want to send to the Particle Cloud. This library depends on `RunningAverage@0.0.1`.

## Welcome to your library!

This library enables two "smart" publishing frequencies so you can make sure you're publish high frequency data when it counts and saving on data when it doesn't. The current "smart" methodologies include: 
1. *"jump"*, the latest value is compared to the average across the last `<length>` measurements. If the latest value is more than `<absValueChange>` greater-than  || less-than that average then it will publish.
2. *"gaussian"*, aka [Gaussian distribution](https://en.wikipedia.org/wiki/Normal_distribution#Standard_deviation_and_coverage) is similar to the "jump" method except that it uses the standard deviation of the last `<length>` measurements and the input `<sigma>` to create upper and lower bounds that trigger a publish.
3. *"range"*, this is the simplist method and allows you to define a static range that will trigger a publish when your latest value falls outs side range.



## Usage
To create an instance of this library, you'll need to initialize a `SamplerSpec` structure.
```
SamplerSpec tempSpec { eventName: "tempC", method: "jump", minInterval: 10, maxInterval: 60, length: 20, absValueChange: 3};
DynamicFrequencySampler temperatureSampler(tempSpec)
```
This above example will create an object that will publish an event named "tempC". The payload of that event will be a JSON object with key `"tempC"`. The library will make sure that events are published at least every `60` seconds, and not more than once every `10` seconds. The length `20` determines how many subsequent publish calls are stored in the library's buffer. This buffer plays a role in both the "jump" and "gaussian" methods because the entire buffer is used to determine the `average` and `stdev` which define the upper and lower thresholds for triggering publishes. 

In order do actually publish data you will have to call into the `publish()` function, the signature of that function is:
```
temperatureSampler.publish(latestSensorValue)
```

This library also automatically creates Particle functions that will toggle a debugging mode that prints out values and thresholds passed to the the `DynamicFrequencySampler.publish` function. Try enabling debugger mode and running `particle serial monitor` from your CLI (while your device is plugged into your computer) to see the output. 

## Examples

1. __1_Sampler101__ - Basic example of how to initialize the library

## Improvements: 
- Make it possible to disable the creation of debugger functions. 

## Changes: 
*v0.0.6* - Was getting compile warnings so changed branching logic to use strcmp

*v0.0.7* - Moved the particle cloud functions to a new method `enableCloudFunctions`

*v0.0.8* - Adding logging and found bug in strcmp logic
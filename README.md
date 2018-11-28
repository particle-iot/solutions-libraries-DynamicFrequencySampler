# DynamicFrequencySampler

Create "dynamic" or "smart" publishing frequencies for sensor data that you want to send to the Particle Cloud. This library depends on `RunningAverage@0.0.1`.

## Welcome to your library!

This library enables two "smart" publishing frequencies so you can make sure you're publish high frequency data when it counts and saving on data when it doesn't. The current "smart" methodologies include: 
1. "Outlier thresholds" aka [Gaussian distribution](https://en.wikipedia.org/wiki/Normal_distribution#Standard_deviation_and_coverage) based thresholds
2. "Absolute thresholds"


## Usage
To create an instance of this library, you'll need to initialize with 3 arguments.
```
DynamicFrequencySampler myInstance("sensorOrEventName",10,60*1000)
```
This above example will create object that will publish an event named "sensorOrEventName". The payload of that event will be a JSON object with key `"sensorOrEventName"`. The library will make sure that events are published at least every `60*1000` milliseconds. The last argument `10` determines the buffer size which plays a role in the "Outlier thresholds" methodology, more on that in a second. 

In order do actually publish data you will have to call into the `publish()` function, the signature of that function depends on which "smart" methodology you want to use. For "Outlier thresholds":
```
myInstance.publish(value,3)
```
This will publish (ie `Particle.publish`) ONLY if `value` is 3 standard deviations away from the average of the buffer which you defined when you initialized `myInstance` OR if the last time publish was called is greater than `60*1000`. 

For  "Absolute thresholds":
```
myInstance.publish(value,63.0,80.0)
```
This will publish (ie `Particle.publish`) ONLY if `value` is less than `63.0`, greater than `80.0` OR if the last time publish was called is greater than `60*1000`. 

This library also automatically creates Particle functions that will toggle a debugging mode that prints out values and thresholds passed to the the `DynamicFrequencySampler.publish` function. Try enabling debugger mode and running `particle serial monitor` from your CLI (while your device is plugged into your computer) to see the output. 

## Examples

1. __1_Sampler101__ - Basic example of how to initialize the library

## Improvements: 
- Make it possible to disable the creation of debugger functions. 
- Enable "derivative" threshold publishing logic, ie when values change by X over y seconds then publish.

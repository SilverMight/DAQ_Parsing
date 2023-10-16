#include <stdio.h>
#include <stdint.h>

#define NORMALIZATION_FACTOR 0x80000

/*
each device ID has it's own GPIO
struct SensorData {
    device id 
    sensor type

    data 
    channel num 
}

*/

float convertVoltage(float data) {
    return data * 0.5;
}

float convertPressure(float data) {
    return data * 0.5 + 50;
}

typedef float(*conversionFunction) (float data); 

conversionFunction conversionFunctionsArray[] = {
    &convertVoltage,
    &convertPressure
};

enum conversionState {
    voltage,
    pressure,
};

float convertToFloat(uint32_t data) {
    int normalizedSignedInt = (int) (data) - NORMALIZATION_FACTOR;

    return (float) (normalizedSignedInt);
}

extern inline float applyConversion(uint32_t data, conversionFunction func) {
    return func(convertToFloat(data));
}

int main() {
    printf("%f\n", applyConversion(10, conversionFunctionsArray[voltage]));

    return 0;
}

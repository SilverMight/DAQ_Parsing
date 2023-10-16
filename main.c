// C time :(
#include <stdio.h>
#include <stdint.h>

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
    const int normalizationFactor = 0x80000;
    int normalizedSignedInt = (int) (data) - normalizationFactor;

    return (float) (normalizedSignedInt);
}

extern inline float applyConversion(uint32_t data, conversionFunction func) {
    return func(convertToFloat(data));
}

int main() {
    printf("%f\n", applyConversion(10, conversionFunctionsArray[voltage]));

    return 0;
}

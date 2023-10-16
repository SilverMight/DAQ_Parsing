#include <functional>
#include <iostream>
#include <cstdint>
#include <functional>

float convertVoltage(float data) {
    return data * 0.5;
}

float convertPressure(float data) {
    return data * 0.5 + 50;
}

using conversionFunction = float(*)(float);

conversionFunction conversionFunctionsArray[] {
    &convertVoltage,
    &convertPressure
};

enum conversionState {
    voltage,
    pressure,
};

float convertToFloat(uint32_t data) {
    constexpr int normalizationFactor = 0x80000;
    int normalizedSignedInt = static_cast<int>(data) - normalizationFactor;

    return static_cast<float>(normalizedSignedInt);
}

inline float applyConversion(uint32_t data, conversionFunction func) {
    return func(convertToFloat(data));
}


int main() {
    std::cout << applyConversion(10, conversionFunctionsArray[voltage]);

    return 0;
    

}

#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream> 

constexpr double decode(long microVolt) { return (microVolt/(2.4*1000000)-2.5); }

constexpr int hexSize = 6;

inline uint32_t hexStringToNum(const char * hexstring) {
    uint32_t num;
    std::stringstream ss;
    ss << std::hex << hexstring;
    ss >> num;

    return num;
}
int main() {
    auto file = std::ifstream{"hextest.txt"};

    // Read in 6-characters
    char hexString[hexSize + 1];
    while(file.get(hexString, hexSize + 1)) {
        std::cout << hexString << " " << hexStringToNum(hexString) << "\n";
        std::cout << decode(hexStringToNum(hexString)) << "\n";
    }
}

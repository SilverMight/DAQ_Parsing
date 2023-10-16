#include <cstdint>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream> 

constexpr double decode(long microVolt) { return (microVolt/(2.4*1000000)-2.5); }

constexpr int hexSize = 6;

long hexStringToNum(const char hexstring[]) {
    long num;
    std::stringstream ss;
    
    // Convert hex number into decimal with stringstream
    ss << std::hex << hexstring;

    // Output the contents to our long
    ss >> num;

    return num;
}
int main() {
    auto file = std::ifstream{"hextest.txt"};

    // Read in 6-characters
    char hexString[hexSize + 1];

    // time delta is 10 ms
    constexpr int timeDelta = 10;

    int curTimeMilliseconds = 0;
    while(file.get(hexString, hexSize + 1)) {
        //std::printf("%s %ld\n", hexString, hexStringToNum(hexString));
        std::printf("%.2lf,%.9lf\n", ((double) curTimeMilliseconds / 1000), decode(hexStringToNum(hexString)));
        curTimeMilliseconds += timeDelta;
    }

    return 0;
}

//
// Created by adoma on 9/19/2024.
//
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include "common.h"

int main(){
    std::string pasirinkimas;
    //fileChoice(pasirinkimas);
    std::string zodziai;  // No need for the u8 prefix in the variable name
    std::cin >> zodziai;  // Take input from the user (UTF-8 encoded string)

    std::ostringstream binaryOutput;

    // Iterate over each byte of the UTF-8 string
    for (unsigned char c : zodziai) {  // Use unsigned char for correct byte handling
        for (int i = 7; i >= 0; --i) {
            binaryOutput << ((c >> i) & 1);  // Shift and mask to get each bit of the byte
        }
        binaryOutput;  // Add space after each byte
    }

    // Output the binary string
    std::cout << binaryOutput.str() << std::endl;
    return 0;
}

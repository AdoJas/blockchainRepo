//
// Created by adoma on 9/19/2024.
//

#include <array>
#include "common.h"

void fileChoice(std::string& s) {
    std::cout << "Pasirinkite, kaip noretumete ivesti duomenis: 1 - Ranka, 2 - is failo" << std::endl;
    do{
        std::cin >> s;
       if(stoi(s) != 1 && stoi(s) != 2) {
              std::cout << "Neteisinga ivestis. Pasirinkite skaiciu nuo 1 iki 2.\n";
       }
    }while(stoi(s) == 1 || stoi(s) != 2);
}

void computeHashFunction(unsigned int x, std::array<uint8_t, HASH_SIZE>& hashArray, unsigned int& previousY) {
     // Pradedame su 1, kad isvengtume 0 reiksmes, kuri butu problematiska skaiciavimuose
    unsigned int p1 = 31; // pirminis skaicius maisymui
    unsigned int p2 = 17; // pirminis skaicius maisymui
    unsigned int p3 = 19; // pirminis skaicius maisymui

    // Kompleksiskesnis skaiciavimas padidina reiksmes atsitiktinuma ir funkcijos jautruma
    double y = (5 * x + 7 * x * x + 3 * pow(previousY, (x % p1)) +
                sin(previousY) + cos(previousY / 2.0) * p2 +
                (previousY ^ x) * p3 + (x * x * x % 23)); // papildoma maisa; funkcija ---- y =(5x + 7x^2 + 3^(previousY mod  x%31) + sin(previousY) + cos(previousY/2.0)*17 + (y xor x)*19 + (x^3 mod 23)) mod 256

    previousY = static_cast<unsigned int>(std::abs(y)) % 256; // gauname absoliuciaja reikseme, kad butu teigiamas skaicius, kuris butu didesnis ir padarome ji tarp 0-255, kad butu manageable dydzio

    // konvertuojame i bitus ir uzpildome hashArray'u
    for (size_t i = 0; i < HASH_SIZE; ++i) {
        hashArray[i] ^= (previousY + i) & 0xFF; // XOR operacija maisant su buvusia reiksme
        previousY = (previousY * 31 + i) % 256; // liekanos operacija - uztikrina, kad gauta reiksme bus nuo 0 iki 255
    }
}

std::string toHexString(const std::array<uint8_t, HASH_SIZE>& hashArray) {
    std::ostringstream oss;

    for (const auto& byte : hashArray) {
        oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
    }

    return oss.str();
}
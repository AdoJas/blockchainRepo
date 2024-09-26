//
// Created by adoma on 9/19/2024.
//

#include <array>
#include "common.h"

int fileChoice() {
    std::cout << "Pasirinkite, kaip noretumete ivesti duomenis: 1 - Ranka, 2 - is failo, 3 - testai" << std::endl;
    std::string s;
    std::string manualInput;
    do{
        std::cin >> s;
       if(stoi(s) != 1 && stoi(s) != 2 && stoi(s) != 3){
              std::cout << "Neteisinga ivestis. Pasirinkite skaiciu nuo 1 iki 2.\n";
       }
    }while(stoi(s) == 1 || stoi(s) != 2 || stoi(s) != 3);

    switch(stoi(s)){
        case 1:
            std::cout << "Iveskite teksta: ";
            std::cin >> manualInput;
            break;
        case 2:
            std::cout << "Iveskite failo pavadinima: ";
            std::cin >> manualInput;
            break;
        case 3:
            std::cout<< "1- Du skirtingi simboliai\n";
            std::cout<< "2- Du failai >1000 atsitiktiniu simboliu\n";
            std::cout<< "3- Du failai >1000 atsitiktiniu simboliu, skiriasi vienu simb.\n";
            std::cout<< "4- Tuscias failas\n";

            break;
    }
    return stoi(s);
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

void konstitucijosTestas(std::string zodziai){
    std::ifstream fd1("konstitucija.txt");

    int totalLinesRead = 0;
    int linesToRead = 1;

    while (!fd1.eof()) {
        std::array<uint8_t, HASH_SIZE> hashArray = {0}; // sukuriame nulinio dydzio arrayu, kuriame laikysime hash reiksmes
        unsigned int previousY = 1; // pradine reiksme
        long long totalHashTime = 0;

        fd1.seekg(0); // Nustatome pointeri i failo pradzia
        std::cout << "Reading " << linesToRead << " lines:" << std::endl;

        for (int i = 0; i < linesToRead && std::getline(fd1, zodziai); ++i) {
            ++totalLinesRead;
            //std::cout << zodziai << std::endl;
            auto start = std::chrono::high_resolution_clock::now();

            // Pereiname per kiekviena string simboli
            for (char c : zodziai) {
                unsigned int decimalValue = static_cast<unsigned int>(c); // Gauname ASCII simbolio reiksme
                // Komputuojame hash funkcija gautai dviajetainei vertei
                computeHashFunction(decimalValue, hashArray, previousY);
            }

            // Verciame hashArray i sesioliktaini stringa
            std::string finalHash = toHexString(hashArray);

            auto end = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            totalHashTime += duration.count();
            //std::cout << "Final computed hash (hex): " << finalHash <<  std::endl; // isvedame gauta hasha

            previousY = 1; // Resetuojame previousY reiksme, kad naujai skaiciuojant hash funkcija butu pradeda nuo 1
        }
        std::cout << "Total time taken for " << linesToRead << " lines: " << totalHashTime << " ms" << std::endl;
        linesToRead *= 2;  // padvigubiname nuskaitomu eiluciu kieki
    }
}

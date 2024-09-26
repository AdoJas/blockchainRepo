//
// Created by adoma on 9/19/2024.
//

#include <array>
#include "common.h"

void fileChoice() {
    std::cout << "Pasirinkite, kaip noretumete ivesti duomenis: 1 - Ranka, 2 - is failo, 3 - testai" << std::endl;
    std::string s;
    std::string manualInput;

    while (true) {
        std::cin >> s;
        try {
            int choice = std::stoi(s);
            if (choice == 1 || choice == 2) {
                switch (choice) {
                    case 1:
                        manualHash();
                        return;
                    case 2:
                        std::cout << "Iveskite failo pavadinima: ";
                        std::cin >> manualInput;
                        readingFromFile(manualInput);
                        return;
                }
            } else {
                std::cout << "Neteisinga ivestis. Pasirinkite skaiciu nuo 1 iki 2.\n";
            }
        } catch (const std::invalid_argument&) {
            std::cout << "Neteisinga ivestis. Pasirinkite skaiciu nuo 1 iki 2.\n";
        }
    }
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
    if (!fd1.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    int totalLinesRead = 0;
    int linesToRead = 1;

    while (true) {
        std::array<uint8_t, HASH_SIZE> hashArray = {0};
        unsigned int previousY = 1;
        long long totalHashTime = 0;

        fd1.seekg(0);
        std::cout << "Reading " << linesToRead << " lines:" << std::endl;

        int linesReadThisIteration = 0;
        for (int i = 0; i < linesToRead && std::getline(fd1, zodziai); ++i) {
            ++linesReadThisIteration;
            ++totalLinesRead;

            auto start = std::chrono::high_resolution_clock::now();

            for (char c : zodziai) {
                unsigned int decimalValue = static_cast<unsigned int>(c);

                computeHashFunction(decimalValue, hashArray, previousY);
            }

            std::string finalHash = toHexString(hashArray);

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            totalHashTime += duration.count();

            previousY = 1;
        }

        std::cout << "Total time taken for " << linesReadThisIteration << " lines: " << totalHashTime << " ns" << std::endl;

        if (linesReadThisIteration == 0) {
            break;
        }
        linesToRead *= 2;
    }
}

void manualHash(){
    std::string zodziai;
    std::cout << "Iveskite zodzius: ";
    std::cin >> zodziai;
    std::array<uint8_t, HASH_SIZE> hashArray = {0};

    unsigned int previousY = 1;

    for (char c : zodziai) {
        unsigned int decimalValue = static_cast<unsigned int>(c);
        computeHashFunction(decimalValue, hashArray, previousY);
    }
    std::string finalHash = toHexString(hashArray);
    std::cout << "Hash: " << finalHash << std::endl;
}

void readingFromFile(std::string filename){
    std::ifstream fd1(filename);
    if (!fd1.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    std::string zodziai;
    std::string allText;

    while (std::getline(fd1, zodziai)) {
        allText += zodziai + "\n";
    }
    fd1.close();

    std::string defaultValue = "'m,ad/Kez**gqnI< sU 4esd;cx1GNwkF>}M,F_eJvTU)kw-yEt!:3}IC+e*J]YNC&L";
    std::array<uint8_t, HASH_SIZE> hashArray = {0};

    unsigned int previousY = 1;

    if(allText.empty()){
        for (char c : defaultValue) {
            unsigned int decimalValue = static_cast<unsigned int>(c);
            computeHashFunction(decimalValue, hashArray, previousY);
        }
    }else{
        for (char c : allText) {
            unsigned int decimalValue = static_cast<unsigned int>(c);
            computeHashFunction(decimalValue, hashArray, previousY);
        }
    }

    std::string finalHash = toHexString(hashArray);
    std::cout << "Final hash: " << finalHash << std::endl;
}

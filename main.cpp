#include <iostream>
#include "common.h"
#include <fstream>
#include <chrono>

int main() {
    std::string zodziai; // Ivedimo string
    //void fileChoice(std::string& s);

    std::ifstream fd1("konstitucija.txt");
    //std::cout << "Enter a string: ";
    //std::cin >> zodziai;


    int totalLinesRead = 0;
    int linesToRead = 1;

    while (!fd1.eof()) {
        std::array<uint8_t, HASH_SIZE> hashArray = {0}; // sukuriame nulinio dydzio arrayu, kuriame laikysime hash reiksmes
        unsigned int previousY = 1; // pradine reiksme

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

            std::cout << "Final computed hash (hex): " << finalHash <<  " " << "Time taken: " << duration.count() << " ns" << std::endl; // isvedame gauta hasha
            previousY = 1; // Resetuojame previousY reiksme, kad naujai skaiciuojant hash funkcija butu pradeda nuo 1
        }


        linesToRead *= 2;  // padvigubiname nuskaitomu eiluciu kieki
    }
    return 0;
}

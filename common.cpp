//
// Created by adoma on 9/19/2024.
//


#include "common.h"
#include "differenceTesting.h"

void fileChoice() {
    while (true) {
        std::cout << "Pasirinkite, kaip noretumete ivesti duomenis: 1 - Ranka, 2 - is failo, 3 - Konstitucija Testas, 4 - koliziju patikra, 5- bitu ir hex skirtumu skaiciavimas" << std::endl;
        std::string s;
        std::string manualInput;
        std::cin >> s;
        try {
            int choice = std::stoi(s);
            if (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5) {
                switch (choice) {
                    case 1:
                        manualHash();
                        continue;
                    case 2:
                        std::cout << "Iveskite failo pavadinima: ";
                        std::cin >> manualInput;
                        readingFromFile(manualInput);
                        continue;
                    case 3:
                        konstitucijosTestas(manualInput);
                        continue;
                    case 4:
                        checkForCollisions("random_pairs.txt");
                        continue;
                    case 5:
                        testHashDifferencesFromFile("testavimas6uzd.txt");
                        continue;
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
    unsigned int p1 = 2654435761; // Knuth's dauginimo konstanta
    unsigned int p2 = 1597334677; // random prime
    unsigned int p3 = 2246822519; // random prime
    unsigned int p4 = 3266489917; // random prime

    for (int i = 0; i < HASH_SIZE; i++) {
        unsigned int uniqueInput = (x ^ previousY) + (i * p1); // XOR su ankstesniu rezultatu; naudojamas kaip unikalus ivesties kintamasis kartu su i * p1

        uniqueInput = (uniqueInput << (i % 16)) | (uniqueInput >> (16 - (i % 16))); // (uniqueInput << (i % 16)) bitus paslenka (i % 16) poziciju i kairę, (uniqueInput >> (16 - (i % 16))) bitus paslenka (16 - (i % 16)) poziciju i desinę, ir rezultatas yra "sudedamas" OR operacijos
        unsigned int y = (13 * uniqueInput + 17 * (uniqueInput * uniqueInput)
                          + 5 * (previousY * previousY)
                          + ((previousY ^ uniqueInput) * p3)
                          + ((uniqueInput * uniqueInput * uniqueInput) % 31) * p4);

        y = (y ^ (y << 13)) ^ ((y >> 11) | (previousY << (i % 8)));

        previousY = ((previousY * p1) ^ (y + i * p2) + (previousY << 5)) % 1048576;

        hashArray[i] ^= (y & 0xFF);
        hashArray[i] ^= ((y >> 8) & 0xFF);
        hashArray[i] ^= ((y >> 16) & 0xFF);  // Didesne difuzina
        hashArray[i] ^= ((y >> 24) & 0xFF);  // Mixinam tolesnius bitus
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
    std::ofstream fd2("konstitucija_hash.txt");
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

void manualHash() {
    std::string zodziai;
    std::cout << "Iveskite zodzius: ";
    std::cin.ignore();
    std::getline(std::cin, zodziai);

    if (zodziai.empty()) {
        zodziai = "'m,ad/Kez**gqnI< sU 4esd;cx1GNwkF>}M,F_eJvTU)kw-yEt!:3}IC+e*J]YNC&L";
    }

    std::array<uint8_t, HASH_SIZE> hashArray = {0};
    unsigned int previousY = 1;

    for (wchar_t wc : zodziai) {
        unsigned int decimalValue = static_cast<unsigned int>(wc);
        computeHashFunction(decimalValue, hashArray, previousY);
    }

    std::string finalHash = toHexString(hashArray);
    std::cout << "\nHash: " << finalHash << std::endl;
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
        allText += zodziai;
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

void checkForCollisions(const std::string &filename) {
    std::ifstream infile(filename);
    std::unordered_map<std::string, std::pair<int, std::string>> hashCounts;
    int collisionCount = 0;
    std::string line;
    int lineNumber = 0;

    while (std::getline(infile, line)) {
        lineNumber++;
        size_t commaPos = line.find(',');
        if (commaPos != std::string::npos) {
            std::string str1 = line.substr(0, commaPos);
            std::string str2 = line.substr(commaPos + 1);
            std::string concatenated = str1 + str2;

            std::array<uint8_t, HASH_SIZE> hashArray = {0};
            unsigned int previousY = 1;

            for (char c : concatenated) {
                unsigned int decimalValue = static_cast<unsigned int>(c);
                computeHashFunction(decimalValue, hashArray, previousY);
            }

            std::string hashValue = toHexString(hashArray);

            if (hashCounts.find(hashValue) != hashCounts.end()) {
                collisionCount++;
                std::cout << "Collision detected!" << std::endl;
                std::cout << "Hash Value: " << hashValue << std::endl;
                std::cout << "Previous string (Line " << hashCounts[hashValue].first << "): " << hashCounts[hashValue].second << std::endl;
                std::cout << "Current string (Line " << lineNumber << "): " << concatenated << std::endl;
            } else {
                hashCounts[hashValue] = {lineNumber, concatenated};
            }
        }
    }

    std::cout << "Number of collisions: " << collisionCount << std::endl;
    std::cout << "Total unique hashes: " << hashCounts.size() << std::endl;
}


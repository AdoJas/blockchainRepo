//
// Created by adoma on 9/27/2024.
//


#include "differenceTesting.h"


double calculateBitDifference(const std::array<uint8_t, HASH_SIZE>& hash1, const std::array<uint8_t, HASH_SIZE>& hash2) {
    int differentBits = 0;
    int totalBits = HASH_SIZE * 8;

    for (size_t i = 0; i < HASH_SIZE; ++i) {
        differentBits += __builtin_popcount(hash1[i] ^ hash2[i]);
    }

    return (static_cast<double>(differentBits) / totalBits) * 100;
}

double calculateHexDifference(const std::string& hex1, const std::string& hex2) {
    int differentHexDigits = 0;
    int totalHexDigits = hex1.size();

    for (size_t i = 0; i < totalHexDigits; ++i) {
        if (hex1[i] != hex2[i]) {
            differentHexDigits++;
        }
    }

    return (static_cast<double>(differentHexDigits) / totalHexDigits) * 100;
}

void analyzeHashDifferences(const std::vector<std::array<uint8_t, HASH_SIZE>>& hashValues) {
    std::vector<double> bitDifferences;
    std::vector<double> hexDifferences;

    for (size_t i = 0; i < hashValues.size(); ++i) {
        for (size_t j = i + 1; j < hashValues.size(); ++j) {
            double bitDiff = calculateBitDifference(hashValues[i], hashValues[j]);
            bitDifferences.push_back(bitDiff);

            std::string hex1 = toHexString(hashValues[i]);
            std::string hex2 = toHexString(hashValues[j]);
            double hexDiff = calculateHexDifference(hex1, hex2);
            hexDifferences.push_back(hexDiff);
        }
    }

    double avgBitDiff = std::accumulate(bitDifferences.begin(), bitDifferences.end(), 0.0) / bitDifferences.size();
    double minBitDiff = *std::min_element(bitDifferences.begin(), bitDifferences.end());
    double maxBitDiff = *std::max_element(bitDifferences.begin(), bitDifferences.end());

    double avgHexDiff = std::accumulate(hexDifferences.begin(), hexDifferences.end(), 0.0) / hexDifferences.size();
    double minHexDiff = *std::min_element(hexDifferences.begin(), hexDifferences.end());
    double maxHexDiff = *std::max_element(hexDifferences.begin(), hexDifferences.end());

    std::cout << "Bit Differences:\n";
    std::cout << "  Average: " << avgBitDiff << "%\n";
    std::cout << "  Minimum: " << minBitDiff << "%\n";
    std::cout << "  Maximum: " << maxBitDiff << "%\n";

    std::cout << "Hex Differences:\n";
    std::cout << "  Average: " << avgHexDiff << "%\n";
    std::cout << "  Minimum: " << minHexDiff << "%\n";
    std::cout << "  Maximum: " << maxHexDiff << "%\n";
}

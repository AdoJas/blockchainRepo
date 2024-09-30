//
// Created by adoma on 9/27/2024.
//

#ifndef BLOCKCHAINREPO_DIFFERENCETESTING_H
#define BLOCKCHAINREPO_DIFFERENCETESTING_H
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <array>
#include <algorithm>
#include <numeric>
#include "common.h"

double calculateBitDifference(const std::array<uint8_t, HASH_SIZE>& hash1, const std::array<uint8_t, HASH_SIZE>& hash2);
double calculateHexDifference(const std::string& hex1, const std::string& hex2);
void analyzeHashDifferences(const std::vector<std::array<uint8_t, HASH_SIZE>>& hashValues);
#endif //BLOCKCHAINREPO_DIFFERENCETESTING_H

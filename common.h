//
// Created by adoma on 9/19/2024.
//

#ifndef BLOCKCHAINREPO_COMMON_H
#define BLOCKCHAINREPO_COMMON_H

#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
#include <cmath>
#include <string>
#include <array>
#include <iomanip>

const size_t HASH_SIZE = 32; // dydis baitais
static unsigned int previousY = 1; // pradine reiksme

void fileChoice(std::string& s);
void computeHashFunction(unsigned int x, std::array<uint8_t, HASH_SIZE>& hashArray, unsigned int& previousY);
std::string toHexString(const std::array<uint8_t, HASH_SIZE>& hashArray);


#endif //BLOCKCHAINREPO_COMMON_H

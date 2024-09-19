//
// Created by adoma on 9/19/2024.
//

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
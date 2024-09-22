#ifndef INTERRUPTS_HPP_
#define INTERRUPTS_HPP_

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<random>
#include<utility>
#include<sstream>
#include<iomanip>

#include<stdio.h>

#define ADDR_BASE   0
#define VECTOR_SIZE 2

// LCG Class Definition
class SimpleLCG {
public:
    SimpleLCG(uint32_t seed) : current(seed) {}

    // Generate a random number using LCG
    uint32_t generate() {
        current = (current * 1664525 + 1013904223) % (1u << 31);
        return current;
    }

    // Get a number in the desired range
    int get_random(int min, int max) {
        return min + (generate() % (max - min + 1));
    }

private:
    uint32_t current;
};

// Following function was taken from stackoverflow; helper function for splitting strings
std::vector<std::string> split_delim(std::string input, std::string delim) {
    std::vector<std::string> tokens;
    std::size_t pos = 0;
    std::string token;
    while ((pos = input.find(delim)) != std::string::npos) {
        token = input.substr(0, pos);
        tokens.push_back(token);
        input.erase(0, pos + delim.length());
    }
    tokens.push_back(input);

    return tokens;
}

//Default interrupt boilerplate
std::pair<std::string, int> intr_boilerplate(int current_time, int intr_num, int context_save_time, std::vector<std::string> vectors) {

    std::string execution = "";

    execution += std::to_string(current_time) + ", " + std::to_string(1) + ", switch to kernel mode\n";
    current_time++;

    execution += std::to_string(current_time) + ", " + std::to_string(context_save_time) + ", context saved\n";
    current_time += context_save_time;
    
    char vector_address_c[10];
    sprintf(vector_address_c, "0x%04X", (ADDR_BASE + (intr_num * VECTOR_SIZE)));
    std::string vector_address(vector_address_c);

    execution += std::to_string(current_time) + ", " + std::to_string(1) + ", find vector " + std::to_string(intr_num) 
                    + " in memory position " + vector_address + "\n";
    current_time++;

    execution += std::to_string(current_time) + ", " + std::to_string(1) + ", load address " + vectors.at(intr_num) + " into the PC\n";
    current_time++;

    return std::make_pair(execution, current_time);
}

#endif
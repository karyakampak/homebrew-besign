#ifndef CALCULATE_HASH_H
#define CALCULATE_HASH_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>

// Class declaration
class CalculateHash {

public:
    // Constructor
    CalculateHash();

    // Member function declaration
    std::unordered_map<std::string, std::string> calculate_hash(std::vector<uint8_t>& placedHolder);
};

#endif
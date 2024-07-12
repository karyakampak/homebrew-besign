#ifndef FIND_STRING_POSITION_H
#define FIND_STRING_POSITION_H
#include <iostream>
#include <vector>
#include <cstdint>

// Class declaration
class FindStringPosition {

public:
    // Constructor
    FindStringPosition();

    // Member function declaration
    size_t findStringPosition(const std::vector<uint8_t>& data, const std::string& query, size_t start);
};

#endif
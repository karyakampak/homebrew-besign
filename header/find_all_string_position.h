#ifndef FIND_ALL_STRING_POSITION_H
#define FIND_ALL_STRING_POSITION_H
#include <iostream>
#include <vector>
#include <cstdint>

// Class declaration
class FindAllStringPosition {

public:
    // Constructor
    FindAllStringPosition();

    // Member function declaration
    std::vector<size_t> findAllStringPosition(const std::vector<uint8_t>& data, const std::string& query);
};

#endif
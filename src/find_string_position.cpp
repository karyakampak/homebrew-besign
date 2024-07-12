#include "../header/find_string_position.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

FindStringPosition::FindStringPosition() {
    // Initialize private member variables or perform any necessary setup
}

// Function to search for a sequence of bytes within a vector
std::vector<uint8_t>::iterator findSequence(std::vector<uint8_t>& vec, const std::string& str, size_t start_position) {
    return std::search(vec.begin(), vec.end(), str.begin(), str.end());
}

size_t FindStringPosition::findStringPosition(const std::vector<uint8_t>& data, const std::string& query, size_t start) {
    size_t dataSize = data.size();
    size_t querySize = query.size();

    // Ensure start is within bounds
    if (start >= dataSize) {
        return std::string::npos;
    }

    for (size_t i = start; i <= dataSize - querySize; ++i) {
        if (std::equal(data.begin() + i, data.begin() + i + querySize, query.begin())) {
            return i; // Return the index where the query string is found
        }
    }

    return std::string::npos; // Return npos if not found
}
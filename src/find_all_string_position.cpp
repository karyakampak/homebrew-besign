#include "../header/find_all_string_position.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint> // For uint8_t

FindAllStringPosition::FindAllStringPosition() {
    // Initialize private member variables or perform any necessary setup
}

std::vector<size_t> FindAllStringPosition::findAllStringPosition(const std::vector<uint8_t>& data, const std::string& query) {
    std::vector<size_t> indices;
    size_t querySize = query.size();
    size_t dataSize = data.size();

    for (size_t i = 0; i <= dataSize - querySize; ++i) {
        if (std::equal(data.begin() + i, data.begin() + i + querySize, query.begin())) {
            indices.push_back(i);
        }
    }

    return indices;
}
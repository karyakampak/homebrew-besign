#include "../header/get_added_index.h"
#include "../header/addons.h"
#include <regex>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

GetAddedIndex::GetAddedIndex() {
    // Initialize private member variables or perform any necessary setup
}

// Convert a string representation to a single size_t value
size_t string_to_size_t(const std::string& str) {
    std::istringstream iss(str);
    size_t value;
    iss >> value;
    return value;
}

// Convert a string to a vector of size_t values
std::vector<size_t> string_to_vector(const std::string& input) {
    std::vector<size_t> output;
    std::istringstream iss(input);
    std::string token;
    while (std::getline(iss, token, ' ')) {
        if (!token.empty()) {
            output.push_back(string_to_size_t(token));
        }
    }
    return output;
}

std::vector<size_t> GetAddedIndex::get_index_after_add_dict(std::vector<uint8_t>& added_index) {
    std::string added_index_string(added_index.begin(), added_index.end());

    std::vector<size_t> x_list = string_to_vector(added_index_string);

    return x_list;
}
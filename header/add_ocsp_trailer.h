#ifndef ADD_OCSP_TRAILER_H
#define ADD_OCSP_TRAILER_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Class declaration
class AddOCSPTrailer {

public:
    // Constructor
    AddOCSPTrailer();

    // Member function declaration
    std::vector<uint8_t> add_ocsp_trailer(const std::vector<uint8_t>& pdf, const std::vector<uint8_t>& ocsp_dict_added, const std::vector<size_t>& added_index, const std::unordered_map<std::string, std::string>& placeholderData);
};

#endif
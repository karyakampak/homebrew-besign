#ifndef ADD_OCSP_DICT_H
#define ADD_OCSP_DICT_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Class declaration
class AddOCSPDict {

public:
    // Constructor
    AddOCSPDict();

    // Member function declaration
    std::unordered_map<std::string, std::vector<uint8_t> > add_ocsp_dict(std::string max_index_string, std::unordered_map<std::string, std::vector<uint8_t> > update_byterange, std::vector<uint8_t> catalog_dict, const std::vector<uint8_t>& vec_ocsp);
};

#endif
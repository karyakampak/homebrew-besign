#ifndef ADDONS_H
#define ADDONS_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint> // For uint8_t
#include <unordered_map>
#include <stdexcept>
#include <cctype>
#include <regex>
#include <openssl/x509.h>
#include <openssl/x509v3.h> // Include this header for STACK_OF(X509_ATTRIBUTE)
#include <cstdint>

// Class declaration
class Addons {

public:
    // Constructor
    Addons();

    // Member function declaration
    std::vector<uint8_t> getSubVector(const std::vector<uint8_t>& vec, size_t x, size_t y);
    std::string vectorToString(const std::vector<uint8_t>& vec);
    std::vector<uint8_t> stringToVector(const std::string& str);
    std::vector<uint8_t> get_value(std::vector<uint8_t> vec_data, const std::vector<uint8_t>& key);
    std::vector<uint8_t> get_value_id(std::vector<uint8_t> vec_data, const std::vector<uint8_t>& key);
    std::vector<size_t> findAllStringPosition(const std::vector<uint8_t>& data, const std::string& query);
    size_t findStringPosition(const std::vector<uint8_t>& data, const std::string& query, size_t start);
    std::vector<std::string> extractPatterns(const std::string& input);
    std::string generateUUID2();
    std::vector<uint8_t> decodePNG(const char* filename, int& width, int& height, int& channels);
    std::string vectorToHexString(const std::vector<uint8_t>& vec);
    std::string base64_encode(const unsigned char* input, int length);
    std::vector<uint8_t> base64_decode(const std::string& base64Data);
    STACK_OF(X509)* vec_uint8_to_stack_of_x509(const std::vector<uint8_t>& vec);
    std::vector<std::vector<uint8_t> > stack_of_x509_to_vec_list(STACK_OF(X509)* stack);
    size_t get_offset(size_t pdf_awal_len, const std::vector<uint8_t>& pdf_akhir, const std::string& object_number);
    std::vector<uint8_t> sha256(const std::vector<uint8_t>& data);
    std::vector<uint8_t> hexStringToBytes(const std::string& hex);
};

#endif
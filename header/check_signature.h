#ifndef CHECK_SIGNATURE_H
#define CHECK_SIGNATURE_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Class declaration
class CheckSignature {

public:
    // Constructor
    CheckSignature();

    // Member function declaration
    std::unordered_map<std::string, std::string> check_signature(std::vector<uint8_t> pdf_content);
};

#endif
#ifndef SIGN_WITH_P12_H
#define SIGN_WITH_P12_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

// Class declaration
class SignWithP12 {

public:
    // Constructor
    SignWithP12();

    // Member function declaration
    std::vector<uint8_t> sign(std::vector<uint8_t>& placedHolder, const char* p12Path, const char* passphrase);
};

#endif
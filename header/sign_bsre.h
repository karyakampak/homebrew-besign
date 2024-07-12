#ifndef SIGN_BSRE_H
#define SIGN_BSRE_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Class declaration
class SignBSrE {

public:
    // Constructor
    SignBSrE();

    // Member function declaration
    std::unordered_map<std::string, std::vector<uint8_t> > sign(std::vector<uint8_t>& placedHolder, const char* nik, const char* passphrase, const char* id, const char* secret);
};

#endif
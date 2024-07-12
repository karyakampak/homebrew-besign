#ifndef PLACE_SIGNATURE_H
#define PLACE_SIGNATURE_H
#include <iostream>
#include <vector>
#include <cstdint>

// Class declaration
class PlaceSignature {

public:
    // Constructor
    PlaceSignature();

    // Member function declaration
    std::vector<uint8_t> place_signature(std::vector<uint8_t>& placedHolder, std::string signature_hex, int byteRange0, int byteRange1, int byteRange2, int byteRange3);
};

#endif
#include "../header/place_signature.h"
#include "../header/addons.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <array>


PlaceSignature::PlaceSignature() {
    // Initialize private member variables or perform any necessary setup
}

std::vector<uint8_t> PlaceSignature::place_signature(std::vector<uint8_t>& placedHolder, std::string signature_hex, int byteRange0, int byteRange1, int byteRange2, int byteRange3) {
    Addons adns;
    std::vector<uint8_t> data_to_sign = adns.getSubVector(placedHolder, 0, byteRange1-1);
    std::vector<uint8_t> digest2 = adns.getSubVector(placedHolder, byteRange2, (byteRange2 + byteRange3-1));
    data_to_sign.reserve(data_to_sign.size() + digest2.size());
    std::copy(digest2.begin(), digest2.end(), std::back_inserter(data_to_sign));


    std::string repeated = "<" + signature_hex + std::string(35000 - signature_hex.length(), '0') + ">";
    std::vector<uint8_t> tte(repeated.begin(), repeated.end());

    std::vector<uint8_t> concatenatedBuffer;
    concatenatedBuffer.reserve(data_to_sign.size() + tte.size());
    concatenatedBuffer.insert(concatenatedBuffer.end(), data_to_sign.begin(), data_to_sign.begin() + byteRange1);
    concatenatedBuffer.insert(concatenatedBuffer.end(), tte.begin(), tte.end());
    concatenatedBuffer.insert(concatenatedBuffer.end(), data_to_sign.begin() + byteRange1, data_to_sign.end());


    return concatenatedBuffer;
}
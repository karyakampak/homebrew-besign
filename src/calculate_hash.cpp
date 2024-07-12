#include "../header/calculate_hash.h"
#include "../header/cms.h"
#include "../header/detached_cms.h"
#include "../header/addons.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <openssl/sha.h>
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/cms.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <array>


CalculateHash::CalculateHash() {
    // Initialize private member variables or perform any necessary setup
}

std::unordered_map<std::string, std::string> CalculateHash::calculate_hash(std::vector<uint8_t>& placedHolder) {

    Addons adns;
    // Sample query string
    const char query_byterange[] = "/ByteRange [0 /********** /********** /**********]";
    size_t byteRangePos = 0;
    size_t index = adns.findStringPosition(placedHolder, query_byterange, 0);
    if (index != -1) {
        byteRangePos = index;
    }

    size_t byteRangeEnd = byteRangePos + sizeof(query_byterange);

    // Find the index where the query occurs in the buffer
    size_t contentsTagPos = 0;
    index = adns.findStringPosition(placedHolder, "/Contents", byteRangeEnd);
    if (index != -1) {
        contentsTagPos = index;
    }

    size_t placeholderPos = 0;
    index = adns.findStringPosition(placedHolder, "<", contentsTagPos);
    if (index != -1) {
        placeholderPos = index;
    }


    size_t placeholderEnd = 0;
    index = adns.findStringPosition(placedHolder, ">", placeholderPos);
    if (index != -1) {
        placeholderEnd = index;
    }


    size_t placeholderLengthWithBrackets = placeholderEnd + 1 - placeholderPos;
    size_t placeholderLength = placeholderLengthWithBrackets - 2;

    
    std::array<int, 4> byteRange = {0, 0, 0, 0};
    byteRange[1] = placeholderPos;
    byteRange[2] = byteRange[1] + placeholderLengthWithBrackets;
    byteRange[3] = placedHolder.size() - byteRange[2];
    std::string actualByteRange = "ByteRange [" + std::to_string(byteRange[0]) + " " + std::to_string(byteRange[1]) + " " + std::to_string(byteRange[2]) + " " + std::to_string(byteRange[3]) + "]";
    actualByteRange += std::string((sizeof(query_byterange) - 2) - actualByteRange.size(), ' ');
    std::vector<uint8_t> updated_pdf = adns.getSubVector(placedHolder, 0, byteRangePos);

    std::vector<uint8_t> part3 = adns.getSubVector(placedHolder, byteRangeEnd-1, placedHolder.size()-1);
    updated_pdf.reserve(updated_pdf.size() + actualByteRange.size());
    std::copy(actualByteRange.begin(), actualByteRange.end(), std::back_inserter(updated_pdf));
    updated_pdf.reserve(updated_pdf.size() + part3.size());
    std::copy(part3.begin(), part3.end(), std::back_inserter(updated_pdf));
    
    std::vector<uint8_t> data_to_sign = adns.getSubVector(updated_pdf, 0, byteRange[1]-1);
    std::vector<uint8_t> digest2 = adns.getSubVector(updated_pdf, byteRange[2], (byteRange[2] + byteRange[3]-1));
    data_to_sign.reserve(data_to_sign.size() + digest2.size());
    std::copy(digest2.begin(), digest2.end(), std::back_inserter(data_to_sign));


    const unsigned char* data = data_to_sign.data();
    unsigned char sha256_digest[SHA256_DIGEST_LENGTH];
    SHA256(data, data_to_sign.size(), sha256_digest);

    std::string bsHash = adns.base64_encode(sha256_digest, SHA256_DIGEST_LENGTH);


    std::string repeated = "<" + std::string(35000, '0') + ">";
    std::vector<uint8_t> tte(repeated.begin(), repeated.end());

    std::vector<uint8_t> concatenatedBuffer;
    concatenatedBuffer.reserve(data_to_sign.size() + tte.size());
    concatenatedBuffer.insert(concatenatedBuffer.end(), data_to_sign.begin(), data_to_sign.begin() + byteRange[1]);
    concatenatedBuffer.insert(concatenatedBuffer.end(), tte.begin(), tte.end());
    concatenatedBuffer.insert(concatenatedBuffer.end(), data_to_sign.begin() + byteRange[1], data_to_sign.end());


    std::unordered_map<std::string, std::string> rslt;
    
    rslt["pdf"] = adns.vectorToString(concatenatedBuffer);
    rslt["hash"] = bsHash;
    rslt["byterange0"] = std::to_string(byteRange[0]);
    rslt["byterange1"] = std::to_string(byteRange[1]);
    rslt["byterange2"] = std::to_string(byteRange[2]);
    rslt["byterange3"] = std::to_string(byteRange[3]);


    return rslt;
}
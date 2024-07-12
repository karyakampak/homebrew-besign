#ifndef GET_OCSP_RESPONSE_H
#define GET_OCSP_RESPONSE_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Class declaration
class GetOCSPResponse {

public:
    // Constructor
    GetOCSPResponse();

    // Member function declaration
    std::vector<uint8_t> get_ocsp(const std::vector<uint8_t>& stack_certs);
};

#endif
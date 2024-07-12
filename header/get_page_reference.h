#ifndef GET_PAGE_REFERENCE_H
#define GET_PAGE_REFERENCE_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>


// Class declaration
class GetPageReference {

public:
    // Constructor
    GetPageReference();

    // Member function declaration
    std::vector<std::string> getPage(std::vector<uint8_t> pdf_buffer, std::unordered_map<std::string, std::string> read_pdf);
};


#endif
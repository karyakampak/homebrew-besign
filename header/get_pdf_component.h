#ifndef GET_PDF_COMPONENT_H
#define GET_PDF_COMPONENT_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Class declaration
class GetPdfComponent {

public:
    // Constructor
    GetPdfComponent();

    // Member function declaration
    std::unordered_map<std::string, std::string> getPdfComponent(std::vector<uint8_t> pdf_buffer);
};

#endif
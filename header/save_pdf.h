#ifndef SAVE_PDF_H
#define SAVE_PDF_H
#include <iostream>
#include <vector>
#include <cstdint>

// Class declaration
class SavePdf {

public:
    // Constructor
    SavePdf();

    // Member function declaration
    void savePDF(const std::vector<uint8_t>& pdfBuffer, const std::string& filename);
};

#endif
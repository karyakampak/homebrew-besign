#include "../header/save_pdf.h"
#include <iostream>
#include <fstream>
#include <vector>

SavePdf::SavePdf() {
    // Initialize private member variables or perform any necessary setup
}

void SavePdf::savePDF(const std::vector<uint8_t>& pdfBuffer, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::out | std::ios::binary);
    if (!outFile) {
        std::cerr << "\nError opening file: " << filename << std::endl;
        return;
    }

    // Write the PDF buffer to the file
    outFile.write(reinterpret_cast<const char*>(pdfBuffer.data()), pdfBuffer.size());

    // Check if writing was successful
    if (!outFile) {
        std::cerr << "\nError writing to file: " << filename << std::endl;
    } else {
        std::cout << "\nPDF saved to: " << filename << std::endl;
    }

    // Close the file
    outFile.close();
}
#include "../header/open_file.h"
#include <iostream>
#include <fstream>
#include <vector>

OpenFile::OpenFile() {
    // Initialize private member variables or perform any necessary setup
}

std::vector<uint8_t> OpenFile::open(const char* path) {
        // Open file stream
        std::ifstream fileStream(path, std::ios::binary);
        if (!fileStream.is_open()) {
            std::cerr << "Error opening file: " << path << std::endl;
        }

        // Get file size
        fileStream.seekg(0, std::ios::end);
        std::streampos fileSize = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);

        // Read file content into vector
        std::vector<uint8_t> fileContent(fileSize);
        if (!fileStream.read(reinterpret_cast<char*>(fileContent.data()), fileSize)) {
            std::cerr << "Error reading file: " << path << std::endl;
        }

        // Close file stream
        fileStream.close();

        return fileContent;
}
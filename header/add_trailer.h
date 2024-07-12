#ifndef ADD_TRAILER_H
#define ADD_TRAILER_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Class declaration
class AddTrailer {

public:
    // Constructor
    AddTrailer();

    // Member function declaration
    std::unordered_map<std::string, std::string> add_trailer(const std::vector<uint8_t>& pdf_content, const std::vector<uint8_t>& pdf_buffer, const std::vector<size_t>& object_id, const std::unordered_map<std::string, std::string>& read_pdf);
};

#endif
#ifndef ADD_SEAL_DICT_H
#define ADD_SEAL_DICT_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Class declaration
class AddSealDict {

public:
    // Constructor
    AddSealDict();

    // Member function declaration
    std::unordered_map<std::string, std::vector<uint8_t> > add_sealdict(std::vector<unsigned char> pdf_content, const char* input_image_path_string, int page, int visibility, float x, float y, float width, float height, std::vector<std::string> object_page_ref, std::unordered_map<std::string, std::string> read_pdf, std::unordered_map<std::string, std::string> check_sign);
};

#endif
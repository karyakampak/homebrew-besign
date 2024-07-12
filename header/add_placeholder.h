#ifndef ADD_PLACEHOLDER_H
#define ADD_PLACEHOLDER_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Class declaration
class AddPlaceHolder {

public:
    // Constructor
    AddPlaceHolder();

    // Member function declaration
    std::unordered_map<std::string, std::string> addPlaceholder(const char* pdf_path, const char* image_path, int page, int visibility, float x, float y, float width, float height, int isSeal);
};

#endif
#ifndef ADD_VISUALIZATION_H
#define ADD_VISUALIZATION_H
#include <Python.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <random>
#include <algorithm>

// Class declaration
class Visualization {

public:
    // Constructor
    Visualization();

    // Member function declaration
    std::vector<uint8_t> visualization(std::string &pdf_path, int page_num, std::string &target_char, std::string &image_path, std::string &url, int start_object_number, int x, int y, int width, int height);
};

#endif
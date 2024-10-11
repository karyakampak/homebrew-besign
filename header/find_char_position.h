#ifndef FIND_CHAR_POSITION_H
#define FIND_CHAR_POSITION_H
#include <iostream>
#include <vector>
#include <vector>
#include <map>

// Class declaration
class FindCharPosition {

public:
    // Constructor
    FindCharPosition();

    // Member function declaration
    std::vector<std::map<std::string, double>> findCharPosition(const std::string& pdfPath, char character, int pageNumber = -1);
};

#endif
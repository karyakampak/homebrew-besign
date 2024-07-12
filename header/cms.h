#ifndef CMS_H
#define CMS_H
#include <iostream>
#include <cstdint>

// Class declaration
class CMS {

public:
    // Constructor
    CMS();

    // Member function declaration
    std::string generateCMS(const std::string& pkcs12Path, const std::string& password, const std::string& data);
};

#endif
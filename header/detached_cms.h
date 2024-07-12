#ifndef DETACHED_CMS_H
#define DETACHED_CMS_H
#include <iostream>
#include <vector>
#include <cstdint>

// Class declaration
class DetachedCMS {

public:
    // Constructor
    DetachedCMS();

    // Member function declaration
    std::string generateCMS(const std::string& pkcs12Path, const std::string& password, const std::vector<uint8_t>& signed_attrs);
};

#endif
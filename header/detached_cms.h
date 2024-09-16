#ifndef DETACHED_CMS_H
#define DETACHED_CMS_H
#include <openssl/pkcs12.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/cms.h>
#include <openssl/obj_mac.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/x509v3.h> // Include this header for STACK_OF(X509_ATTRIBUTE)
#include <openssl/asn1.h>
#include <openssl/objects.h>
#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdint> // For uint8_t

// Class declaration
class DetachedCMS {

public:
    // Constructor
    DetachedCMS();

    // Member function declaration
    std::string detached_cms(const std::string& hash, const std::string& data, const char* p12Path, const char* passphrase);
};

#endif
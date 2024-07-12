#include "../header/detached_cms.h"
#include <openssl/pkcs12.h>
#include <openssl/pem.h>
#include <openssl/cms.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/obj_mac.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>


DetachedCMS::DetachedCMS() {
    // Initialize private member variables or perform any necessary setup
}

// Function to load PKCS#12 file and extract certificate, private key, and the chain of certificates
bool loadPKCS12_2(const std::string& pkcs12Path, const std::string& password, EVP_PKEY*& pkey, X509*& cert, STACK_OF(X509)*& ca) {
    FILE* fp = fopen(pkcs12Path.c_str(), "rb");
    if (!fp) {
        std::cerr << "Unable to open PKCS#12 file" << std::endl;
        return false;
    }

    PKCS12* p12 = d2i_PKCS12_fp(fp, nullptr);
    fclose(fp);

    if (!p12) {
        std::cerr << "Unable to parse PKCS#12 file" << std::endl;
        return false;
    }

    if (!PKCS12_parse(p12, password.c_str(), &pkey, &cert, &ca)) {
        std::cerr << "Unable to parse PKCS#12 structure" << std::endl;
        PKCS12_free(p12);
        return false;
    }

    PKCS12_free(p12);
    return true;
}

// Function to convert binary data to hex string
std::string binaryToHex_2(const unsigned char* data, size_t length) {
    std::ostringstream oss;
    for (size_t i = 0; i < length; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    return oss.str();
}

// Function to print OpenSSL errors
void print_openssl_error() {
    BIO *bio = BIO_new_fp(stderr, BIO_NOCLOSE);
    ERR_print_errors(bio);
    BIO_free(bio);
}

// Function to compute the SHA-256 digest of input data
std::string computeSHA256Digest(const std::string& data) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_sha256();

    if (!EVP_DigestInit_ex(mdctx, md, nullptr)) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Digest initialization failed");
    }

    if (!EVP_DigestUpdate(mdctx, data.c_str(), data.size())) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Digest update failed");
    }

    if (!EVP_DigestFinal_ex(mdctx, hash, &hashLen)) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Digest finalization failed");
    }

    EVP_MD_CTX_free(mdctx);

    return std::string(reinterpret_cast<char*>(hash), hashLen);
}

// Function to encode data in base64
std::string base64Encode(const std::string& data) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, data.c_str(), data.size());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);

    std::string encodedData(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);

    return encodedData;
}

// Example function to convert unsigned char* to vector<uint8_t>
std::vector<uint8_t> uchar_to_vector(const unsigned char* data, size_t length) {
    return std::vector<uint8_t>(data, data + length);
}

// Function to compute SHA-256 hash
std::vector<uint8_t> sha256_hash(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> hash(SHA256_DIGEST_LENGTH);
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.data(), data.size());
    SHA256_Final(hash.data(), &sha256);
    return hash;
}

// Helper function to convert hash to hexadecimal string
std::string to_hex_string(const std::vector<uint8_t>& hash) {
    std::ostringstream oss;
    for (uint8_t byte : hash) {
        oss << std::setw(2) << std::setfill('0') << std::hex << (int)byte;
    }
    return oss.str();
}

std::string stringToHex(const std::string& input) {
    std::stringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    for (unsigned char c : input) {
        hexStream << std::setw(2) << static_cast<int>(c);
    }
    return hexStream.str();
}

// Function to create a detached CMS signature and save it as a hex string
std::string createDetachedCMS(const std::vector<uint8_t>& signed_attrs, EVP_PKEY* pkey, X509* cert, STACK_OF(X509)* ca) {
    try {
        // Compute the digest of the signed attributes
        std::string hash(signed_attrs.begin(), signed_attrs.end());
        std::string digest = computeSHA256Digest(hash);

        // Encode the digest in base64
        std::string base64Digest = base64Encode(digest);
        std::string hexData = stringToHex(digest);

        // Create BIO for the message digest
        BIO* bio_in = BIO_new_mem_buf(digest.data(), digest.size());
        if (!bio_in) {
            std::cerr << "Error creating BIO for message digest." << std::endl;
            print_openssl_error();
            return nullptr;
        }

        // Create CMS structure
        CMS_ContentInfo* cms = CMS_sign(nullptr, nullptr, ca, bio_in,  CMS_DETACHED | CMS_BINARY | CMS_CADES | CMS_PARTIAL);
        if (!cms) {
            std::cerr << "Error creating CMS structure." << std::endl;
            BIO_free(bio_in);
            print_openssl_error();
            return nullptr;
        }

        // Add signer info to the CMS structure
        if (!CMS_add1_signer(cms, cert, pkey, EVP_sha256(), CMS_BINARY)) {
            std::cerr << "Error adding signer info." << std::endl;
            CMS_ContentInfo_free(cms);
            BIO_free(bio_in);
            print_openssl_error();
            return nullptr;
        }

        // Finalize the CMS structure
        if (!CMS_final(cms, bio_in, nullptr,  CMS_DETACHED | CMS_BINARY)) {
            std::cerr << "Error finalizing CMS structure." << std::endl;
            print_openssl_error();
            CMS_ContentInfo_free(cms);
            BIO_free(bio_in);
            return nullptr;
        }

        // Write CMS to memory BIO
        BIO* bio_out = BIO_new(BIO_s_mem());
        if (!bio_out) {
            std::cerr << "Error creating BIO for output." << std::endl;
            print_openssl_error();
            CMS_ContentInfo_free(cms);
            BIO_free(bio_in);
            return nullptr;
        }

        if (!i2d_CMS_bio(bio_out, cms)) {
            std::cerr << "Error writing CMS to BIO." << std::endl;
            print_openssl_error();
            CMS_ContentInfo_free(cms);
            BIO_free(bio_in);
            BIO_free(bio_out);
            return nullptr;
        }

        // Read the output from BIO
        BUF_MEM* bptr;
        BIO_get_mem_ptr(bio_out, &bptr);
        std::string hexStr = binaryToHex_2((const unsigned char*)bptr->data, bptr->length);

        // Clean up
        CMS_ContentInfo_free(cms);
        BIO_free(bio_in);
        BIO_free(bio_out);
        ERR_free_strings();

        return hexStr;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return nullptr;
    }
}

std::string DetachedCMS::generateCMS(const std::string& pkcs12Path, const std::string& password, const std::vector<uint8_t>& signed_attrs) {
    EVP_PKEY* pkey = nullptr;
    X509* cert = nullptr;
    STACK_OF(X509)* ca = nullptr;

    if (!loadPKCS12_2(pkcs12Path, password, pkey, cert, ca)) {
        std::cerr << "Failed to load PKCS#12 file" << std::endl;
        return "";
    }

    std::string signature = createDetachedCMS(signed_attrs, pkey, cert, ca);

    EVP_PKEY_free(pkey);
    X509_free(cert);
    sk_X509_pop_free(ca, X509_free);

    std::cout << "CMS signature created successfully" << std::endl;
    return signature;
}

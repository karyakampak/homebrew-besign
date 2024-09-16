#include "../header/addons.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <stdexcept>
#include <cctype>
#include <regex>
#include <uuid/uuid.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../header/stb_image.h"
#include <sstream>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h> // Include this header for STACK_OF(X509_ATTRIBUTE)
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/pkcs12.h>
#include <iomanip>
#include <iomanip>
#include <zlib.h>

Addons::Addons() {
    // Initialize private member variables or perform any necessary setup
}


// Function to search for a sequence of bytes within a vector
std::vector<uint8_t>::iterator findSequence(std::vector<uint8_t>& vec, const std::string& str, size_t start_position) {
    return std::search(vec.begin(), vec.end(), str.begin(), str.end());
}


std::vector<uint8_t> Addons::getSubVector(const std::vector<uint8_t>& vec, size_t x, size_t y) {
    if (x >= vec.size() || y >= vec.size() || x > y) {
        // Invalid indices
        return std::vector<uint8_t>(); // Return an empty vector
    }
    return std::vector<uint8_t>(vec.begin() + x, vec.begin() + y + 1);
}

std::string Addons::vectorToString(const std::vector<uint8_t>& vec) {
    return std::string(vec.begin(), vec.end());
}

std::vector<uint8_t> Addons::stringToVector(const std::string& str) {
    std::vector<uint8_t> vec;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        vec.push_back(static_cast<uint8_t>(*it));
    }
    return vec;
}

std::vector<uint8_t> Addons::get_value(std::vector<uint8_t> vec_data, const std::vector<uint8_t>& key) {
    
    std::vector<uint8_t>::iterator index_result = std::search(vec_data.begin(), vec_data.end(), key.begin(), key.end());
    if (index_result != vec_data.end()) {
        vec_data = std::vector<uint8_t>(index_result, vec_data.end());
        std::string pdf_stringxxx(vec_data.begin(), vec_data.end());
    }
    std::vector<uint8_t> query_slice;
    query_slice.push_back(' ');
    query_slice.push_back('R');
    std::vector<uint8_t>::iterator index = std::search(vec_data.begin(), vec_data.end(), query_slice.begin(), query_slice.end());
    if (index != vec_data.end()) {
        vec_data = std::vector<uint8_t>(vec_data.begin() + key.size() + 1, index + 2);
    }
    return vec_data;
}

std::vector<uint8_t> Addons::get_value_id(std::vector<uint8_t> vec_data, const std::vector<uint8_t>& key) {
    
    std::vector<uint8_t>::iterator index_result = std::search(vec_data.begin(), vec_data.end(), key.begin(), key.end());
    if (index_result != vec_data.end()) {
        vec_data = std::vector<uint8_t>(index_result, vec_data.end());
        std::string pdf_stringxxx(vec_data.begin(), vec_data.end());
    } else {
        return std::vector<uint8_t>();
    }
    std::vector<uint8_t> query_slice;
    query_slice.push_back('>');
    // query_slice.push_back(']');
    std::vector<uint8_t>::iterator index = std::search(vec_data.begin(), vec_data.end(), query_slice.begin(), query_slice.end());
    if (index != vec_data.end()) {
        vec_data = std::vector<uint8_t>(vec_data.begin() + key.size() + 1, index + 2);
    }
    return vec_data;
}

std::vector<size_t> Addons::findAllStringPosition(const std::vector<uint8_t>& data, const std::string& query) {
    std::vector<size_t> indices;
    size_t querySize = query.size();
    size_t dataSize = data.size();

    for (size_t i = 0; i <= dataSize - querySize; ++i) {
        if (std::equal(data.begin() + i, data.begin() + i + querySize, query.begin())) {
            indices.push_back(i);
        }
    }

    return indices;
}

size_t Addons::findStringPosition(const std::vector<uint8_t>& data, const std::string& query, size_t start) {
    size_t dataSize = data.size();
    size_t querySize = query.size();

    // Ensure start is within bounds
    if (start >= dataSize) {
        return std::string::npos;
    }

    for (size_t i = start; i <= dataSize - querySize; ++i) {
        if (std::equal(data.begin() + i, data.begin() + i + querySize, query.begin())) {
            return i; // Return the index where the query string is found
        }
    }

    return std::string::npos; // Return npos if not found
}


std::vector<std::string> Addons::extractPatterns(const std::string& input) {
    std::vector<std::string> result;
    std::regex pattern("\\d+ 0 R");
    std::smatch match;

    std::string::const_iterator it = input.begin();
    while (std::regex_search(it, input.end(), match, pattern)) {
        result.push_back(match[0]);
        it = match.suffix().first; // Move iterator to the end of the match
    }

    return result;
}


std::string Addons::generateUUID2() {
    uuid_t uuid;
    uuid_generate(uuid);

    char uuidStr[37]; // 36 characters + null terminator
    uuid_unparse(uuid, uuidStr);

    std::string uuidString(uuidStr);
    // Remove hyphens from the UUID string
    uuidString.erase(std::remove(uuidString.begin(), uuidString.end(), '-'), uuidString.end());

    return uuidString;
}

std::vector<uint8_t> Addons::decodePNG(const char* filename, int& width, int& height, int& channels) {
    std::vector<uint8_t> imageData;

    // Load the image with desired settings for better quality
    stbi_set_flip_vertically_on_load(true); // Flip the image vertically if needed
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_default);
    if (!data) {
        throw std::runtime_error("Failed to load image.");
    }

    // Determine the loaded image format
    int format = STBI_default;
    if (channels == STBI_rgb_alpha) {
        format = STBI_rgb_alpha;
    } else if (channels == STBI_rgb) {
        format = STBI_rgb;
    }

    // // Compress the image data using zlib
    // uLongf compressedSize = compressBound(width * height * channels);
    // imageData.resize(compressedSize);

    // int ret = compress(reinterpret_cast<Bytef*>(imageData.data()), &compressedSize,
    //                    reinterpret_cast<const Bytef*>(data), width * height * channels);
    // stbi_image_free(data);

    // if (ret != Z_OK) {
    //     throw std::runtime_error("Compression failed.");
    // }

    // // Resize vector to actual compressed size
    // imageData.resize(compressedSize);

    // return imageData;

    // Copy the pixel data into a vector
    imageData.assign(data, data + width * height * channels);

    // Free the loaded image data
    stbi_image_free(data);

    return imageData;
}


std::string Addons::base64_encode(const unsigned char* input, int length) {
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bio = BIO_new(BIO_s_mem());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(b64, bio);
    BIO_write(b64, input, length);
    BIO_flush(b64);

    BUF_MEM* bufferPtr;
    BIO_get_mem_ptr(b64, &bufferPtr);

    std::string base64_output(bufferPtr->data, bufferPtr->length); // Exclude null terminator

    BIO_free_all(b64);

    return base64_output;
}


std::vector<uint8_t> Addons::base64_decode(const std::string& base64Data) {
    BIO *bio, *b64;
    std::vector<uint8_t> decodedData;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_new_mem_buf((void*)base64Data.c_str(), base64Data.length());
    bio = BIO_push(b64, bio);

    // Allocate enough memory for decoding
    decodedData.resize(base64Data.length());

    // Perform decoding
    int len = BIO_read(bio, decodedData.data(), decodedData.size());
    if (len > 0) {
        // Resize vector to actual decoded length
        decodedData.resize(len);
    } else {
        // Handle decoding error
        std::cerr << "Error decoding Base64 data" << std::endl;
        decodedData.clear();
    }

    // Free BIOs
    BIO_free_all(bio);

    return decodedData;
}

std::string Addons::vectorToHexString(const std::vector<uint8_t>& vec) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    for (size_t i = 0; i < vec.size(); ++i) {
        uint8_t byte = vec[i];
        ss << std::setw(2) << static_cast<int>(byte);
    }
    
    return ss.str();
}

// Function to convert Vec<uint8_t> to STACK_OF(X509)*
STACK_OF(X509)* Addons::vec_uint8_to_stack_of_x509(const std::vector<uint8_t>& vec) {
    STACK_OF(X509)* stack = sk_X509_new_null();
    if (!stack) {
        std::cerr << "Error creating new STACK_OF(X509)." << std::endl;
        return nullptr;
    }

    const unsigned char* p = vec.data();
    const unsigned char* end = p + vec.size();

    while (p < end) {
        X509* cert = d2i_X509(nullptr, &p, end - p);
        if (!cert) {
            std::cerr << "Error decoding X509 certificate: "
                      << ERR_reason_error_string(ERR_get_error()) << std::endl;
            sk_X509_pop_free(stack, X509_free);
            return nullptr;
        }

        if (!sk_X509_push(stack, cert)) {
            std::cerr << "Error adding X509 certificate to stack." << std::endl;
            X509_free(cert);
            sk_X509_pop_free(stack, X509_free);
            return nullptr;
        }
    }

    return stack;
}

// Function to convert STACK_OF(X509)* to a list of Vec<uint8_t>
std::vector<std::vector<uint8_t> > Addons::stack_of_x509_to_vec_list(STACK_OF(X509)* stack) {
    std::vector<std::vector<uint8_t> > result;

    for (int i = 0; i < sk_X509_num(stack); ++i) {
        X509* cert = sk_X509_value(stack, i);

        int len = i2d_X509(cert, nullptr);
        if (len < 0) {
            std::cerr << "Error converting X509 certificate to DER format: " 
                      << ERR_reason_error_string(ERR_get_error()) << std::endl;
            continue;
        }

        std::vector<uint8_t> der_cert(len);
        unsigned char* p = der_cert.data();
        len = i2d_X509(cert, &p);
        if (len < 0) {
            std::cerr << "Error converting X509 certificate to DER format: " 
                      << ERR_reason_error_string(ERR_get_error()) << std::endl;
            continue;
        }

        result.push_back(der_cert);
    }

    return result;
}

size_t Addons::get_offset(size_t pdf_awal_len, const std::vector<uint8_t>& pdf_akhir, const std::string& object_number) {
    Addons adns;
    std::string query = "\n" + object_number + " 0 obj";
    size_t offset_start = 0;
    size_t index = adns.findStringPosition(pdf_akhir, query, pdf_awal_len);
    if (index != std::string::npos) {
        offset_start = index;
    }
    
    return offset_start;
}

std::vector<uint8_t> Addons::sha256(const std::vector<uint8_t>& data) {
    // Create a context for the hash operation
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) {
        throw std::runtime_error("EVP_MD_CTX_new failed");
    }

    // Initialize the context for SHA-256
    if (1 != EVP_DigestInit_ex(context, EVP_sha256(), nullptr)) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("EVP_DigestInit_ex failed");
    }

    // Update the context with the data
    if (1 != EVP_DigestUpdate(context, data.data(), data.size())) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("EVP_DigestUpdate failed");
    }

    // Finalize the hash and retrieve the result
    std::vector<uint8_t> hash(EVP_MD_size(EVP_sha256()));
    unsigned int length = 0;
    if (1 != EVP_DigestFinal_ex(context, hash.data(), &length)) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("EVP_DigestFinal_ex failed");
    }

    // Clean up the context
    EVP_MD_CTX_free(context);

    return hash;
}



std::vector<uint8_t> Addons::hexStringToBytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
        bytes.push_back(byte);
    }
    
    return bytes;
}

std::string Addons::base64_encode_2(const unsigned char* input, int length) {
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bio = BIO_new(BIO_s_mem());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(b64, bio);
    BIO_write(b64, input, length);
    BIO_flush(b64);

    BUF_MEM* bufferPtr;
    BIO_get_mem_ptr(b64, &bufferPtr);

    std::string base64_output(bufferPtr->data, bufferPtr->length); // Exclude null terminator

    BIO_free_all(b64);

    return base64_output;
}


std::vector<uint8_t> Addons::base64_decode_2(const std::string& base64Data) {
    BIO *bio, *b64;
    std::vector<uint8_t> decodedData;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_new_mem_buf((void*)base64Data.c_str(), base64Data.length());
    bio = BIO_push(b64, bio);

    // Allocate enough memory for decoding
    decodedData.resize(base64Data.length());

    // Perform decoding
    int len = BIO_read(bio, decodedData.data(), decodedData.size());
    if (len > 0) {
        // Resize vector to actual decoded length
        decodedData.resize(len);
    } else {
        // Handle decoding error
        std::cerr << "Error decoding Base64 data" << std::endl;
        decodedData.clear();
    }

    // Free BIOs
    BIO_free_all(bio);

    return decodedData;
}

// Function to convert binary data to hex string
std::string Addons::binaryToHex_2(const unsigned char* data, size_t length) {
    std::ostringstream oss;
    for (size_t i = 0; i < length; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    return oss.str();
}

// Print the OID of the X509_ATTRIBUTE
void Addons::print_attribute_oid(const X509_ATTRIBUTE* attr) {
    const ASN1_OBJECT* obj = X509_ATTRIBUTE_get0_object(const_cast<X509_ATTRIBUTE*>(attr));
    char buffer[128];
    OBJ_obj2txt(buffer, sizeof(buffer), obj, 1);
    std::cout << "OID: " << buffer << std::endl;
}

// Function to load PKCS#12 file and extract certificate, private key, and the chain of certificates
bool Addons::loadPKCS12_2(const std::string& pkcs12Path, const std::string& password, EVP_PKEY*& pkey, X509*& cert, STACK_OF(X509)*& ca) {
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

void Addons::handleErrors() {
    // Print OpenSSL errors to stderr
    ERR_print_errors_fp(stderr);
    
    // Optionally, throw an exception to allow for graceful error handling
    throw std::runtime_error("An error occurred in OpenSSL. Check stderr for details.");
}

std::vector<unsigned char> Addons::readData(std::string filePath) {
    std::ifstream ifs(filePath, std::ios::binary);
    if (!ifs) {
        throw std::runtime_error("Failed to open data file.");
    }

    // Read the entire file into the vector
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    return data;
}

std::string Addons::digest(std::vector<unsigned char> data) {
    // Create a SHA256 hash
    unsigned char sha256_digest[SHA256_DIGEST_LENGTH];
    SHA256(data.data(), data.size(), sha256_digest);

    // Encode the hash as a base64 string
    std::string hash = base64_encode_2(sha256_digest, SHA256_DIGEST_LENGTH);

    return hash;
}

// Function to convert std::vector<uint8_t> to an octet string (hexadecimal representation)
std::string Addons::vectorToStringHex_2(const std::vector<uint8_t>& vec) {
    std::ostringstream oss;
    
    // Iterate over each byte in the vector
    for (auto byte : vec) {
        // Print each byte as two-digit hexadecimal, uppercase
        oss << std::hex << std::nouppercase << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        // Optionally add a space between bytes for readability
        // oss << ' ';
    }
    
    return oss.str();
}

// Function to convert ASN1_STRING to a hexadecimal std::string
std::string Addons::asn1_string_to_hex_string(const ASN1_STRING *asn1_string) {
    const unsigned char *data = ASN1_STRING_get0_data(asn1_string);
    int length = ASN1_STRING_length(asn1_string);

    std::ostringstream hex_stream;

    // Convert each byte to a two-digit hexadecimal value
    for (int i = 0; i < length; i++) {
        hex_stream << std::hex << std::nouppercase << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }

    return hex_stream.str();
}

// Function to convert ASN1_STRING to a hexadecimal std::string
std::string Addons::get_digest_hex_from_signer_info(const CMS_SignerInfo* signer_info) {
    // Define the NID for messageDigest attribute
    int nid_message_digest = NID_pkcs9_messageDigest;
    // Get the count of signed attributes
    int attr_count = CMS_signed_get_attr_count(signer_info);
    if (attr_count < 0) {
        fprintf(stderr, "Error getting attribute count\n");
        return "";
    }
    // Try to find the digest attribute by NID
    int attr_pos = CMS_signed_get_attr_by_NID(signer_info, nid_message_digest, -1);
    if (attr_pos < 0) {
        fprintf(stderr, "Digest attribute not found\n");
        return "";
    }
    // Get the attribute at the found position
    X509_ATTRIBUTE *attr = CMS_signed_get_attr(signer_info, attr_pos);
    if (!attr) {
        fprintf(stderr, "Error getting attribute\n");
        return "";
    }
    // Extract and print the digest data
    const ASN1_TYPE *attr_value = X509_ATTRIBUTE_get0_type(attr, 0);
    if (attr_value) {
        if (attr_value->type == V_ASN1_OCTET_STRING) {
            ASN1_STRING *datal = attr_value->value.asn1_string;
            const unsigned char *datan = ASN1_STRING_get0_data(datal);
            int length = ASN1_STRING_length(datal);
            std::ostringstream hex_stream;
            for (int i = 0; i < length; i++) {
                hex_stream << std::hex << std::nouppercase << std::setw(2) << std::setfill('0') << static_cast<int>(datan[i]);
            }
            return hex_stream.str();
        } else {
            fprintf(stderr, "Attribute value is not an ASN1_OCTET_STRING\n");
        return "";
        }
    } else {
        fprintf(stderr, "Error extracting attribute value\n");
        return "";
    }
}


void Addons::find_and_replace(std::string &str, const std::string &to_find, const std::string &replace_with) {
    std::string::size_type pos = 0;

    // Loop to find and replace all occurrences
    while ((pos = str.find(to_find, pos)) != std::string::npos) {
        str.replace(pos, to_find.length(), replace_with);
        pos += replace_with.length(); // Move past the replaced part
    }
}
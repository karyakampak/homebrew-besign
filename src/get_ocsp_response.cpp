#include "../header/get_ocsp_response.h"
#include "../header/addons.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/buffer.h>
#include <openssl/bio.h>
#include <openssl/ocsp.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/pkcs7.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h> // Include this header for STACK_OF(X509_ATTRIBUTE)
#include <sstream>
#include <chrono>

GetOCSPResponse::GetOCSPResponse() {
    // Initialize private member variables or perform any necessary setup
}

size_t writeCallbackOcsp(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t totalSize = size * nmemb;
    std::vector<char> *responseBuffer = (std::vector<char> *)userdata;
    responseBuffer->insert(responseBuffer->end(), (char *)ptr, (char *)ptr + totalSize);
    return totalSize;
}

std::vector<uint8_t> get_ocsp_response(const char* ocsp_request_file) {

    // OCSP Responder URL
    const char* ocsp_url = "http://cvs-bsre.bssn.go.id/ocsp";

    // Read the OCSP request file into a buffer
    std::ifstream file(ocsp_request_file, std::ios::binary | std::ios::ate);
    std::streamsize file_size = file.tellg();
    file.seekg(0);
    std::vector<char> ocsp_data(file_size);
    if (!file.read(ocsp_data.data(), file_size)) {
        std::cerr << "Failed to read OCSP request file." << std::endl;
        return std::vector<uint8_t>();
    }

    // Initialize cURL session
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize libcurl." << std::endl;
        return std::vector<uint8_t>();
    }

    // Set cURL options
    curl_easy_setopt(curl, CURLOPT_URL, ocsp_url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(file_size));
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ocsp_data.data());

    // Set the correct content-type header
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/ocsp-request");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set up the callback function to receive the response
    std::vector<char> response_data;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallbackOcsp);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    // Execute the cURL request
    CURLcode res = curl_easy_perform(curl);

    // Check if the file exists before attempting to remove it
    if (std::remove(ocsp_request_file) != 0) {
        std::cerr << "Error removing the file." << std::endl;
    }

    std::vector<uint8_t> OCSPResp;
    if (res != CURLE_OK) {
        std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        return std::vector<uint8_t>();
    } else {
        for (char c : response_data) {
            OCSPResp.push_back(static_cast<uint8_t>(c));
        }
        
    }

    // Clean up
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return OCSPResp;
}

std::vector<uint8_t> get_ocsp_request(X509* issuer_cert, X509* cert, const char* ocsp_der_file) {

    // Initialize OpenSSL
    // SSL_load_error_strings();
    // ERR_load_BIO_strings();
    // ERR_load_crypto_strings();
    // SSL_library_init();
    // OpenSSL_add_all_algorithms();

    // Create OCSP request
    OCSP_REQUEST* req = OCSP_REQUEST_new();
    if (!req) {
        fprintf(stderr, "Error creating OCSP request\n");
        ERR_print_errors_fp(stderr);
        X509_free(issuer_cert);
        X509_free(cert);
        return std::vector<uint8_t>();
    }
    int ret = OCSP_request_add1_nonce(req, nullptr, -1);
    if (ret != 1) {
        fprintf(stderr, "Error adding nonce to OCSP request\n");
        ERR_print_errors_fp(stderr);
        OCSP_REQUEST_free(req);
        X509_free(issuer_cert);
        X509_free(cert);
        return std::vector<uint8_t>();
    }
    OCSP_CERTID* id = OCSP_cert_to_id(nullptr, cert, issuer_cert);
    if (!id) {
        fprintf(stderr, "Error creating OCSP certificate ID\n");
        ERR_print_errors_fp(stderr);
        OCSP_REQUEST_free(req);
        X509_free(issuer_cert);
        X509_free(cert);
        return std::vector<uint8_t>();
    }

    if (!OCSP_request_add0_id(req, id)) {
        fprintf(stderr, "Error adding certificate ID to OCSP request\n");
        ERR_print_errors_fp(stderr);
        OCSP_CERTID_free(id);
        OCSP_REQUEST_free(req);
        X509_free(issuer_cert);
        X509_free(cert);
        return std::vector<uint8_t>();
    }

    // Create a BIO to store the DER-encoded OCSP request data
    BIO* req_bio = BIO_new(BIO_s_mem());
    if (!req_bio) {
        fprintf(stderr, "Error creating BIO\n");
        ERR_print_errors_fp(stderr);
        OCSP_CERTID_free(id);
        OCSP_REQUEST_free(req);
        X509_free(issuer_cert);
        X509_free(cert);
        return std::vector<uint8_t>();
    }

    // Write the DER-encoded OCSP request data to the BIO
    if (i2d_OCSP_REQUEST_bio(req_bio, req) <= 0) {
        fprintf(stderr, "Error writing OCSP request data to BIO\n");
        ERR_print_errors_fp(stderr);
        BIO_free(req_bio);
        OCSP_CERTID_free(id);
        OCSP_REQUEST_free(req);
        X509_free(issuer_cert);
        X509_free(cert);
        return std::vector<uint8_t>();
    }


    // Get the request data from the BIO as a binary string
    char* req_data = nullptr;
    long req_data_len = BIO_get_mem_data(req_bio, &req_data);

    // Write the binary string to the file
    FILE* fp = fopen(ocsp_der_file, "wb");
    if (!fp) {
        fprintf(stderr, "Error opening file for writing\n");
        ERR_print_errors_fp(stderr);
        BIO_free(req_bio);
        OCSP_CERTID_free(id);
        OCSP_REQUEST_free(req);
        X509_free(issuer_cert);
        X509_free(cert);
        return std::vector<uint8_t>();
    }
    fwrite(req_data, 1, req_data_len, fp);

    fclose(fp);

    std::vector<uint8_t> OCSPResp = get_ocsp_response(ocsp_der_file);

    // std::string ocspR(OCSPResp.begin(), OCSPResp.end());
    // std::cout << ocspR << std::endl;


    // Clean up
    BIO_free(req_bio);
    OCSP_CERTID_free(id);
    // OCSP_REQUEST_free(req);
    X509_free(issuer_cert);
    X509_free(cert);
    ERR_free_strings();
    EVP_cleanup();


    return OCSPResp;
}


std::vector<uint8_t> GetOCSPResponse::get_ocsp(const std::vector<uint8_t>& stack_certs) {

    Addons adns;

    // Convert Vec<uint8_t> back to STACK_OF(X509)*
    STACK_OF(X509)* new_stack = adns.vec_uint8_to_stack_of_x509(stack_certs);
    if (!new_stack) {
        std::cerr << "Error converting Vec<uint8_t> back to STACK_OF(X509)*." << std::endl;
        return std::vector<uint8_t>();
    }

    std::chrono::system_clock::time_point utc_now = std::chrono::system_clock::now();
    std::chrono::seconds formatted_timestamp = std::chrono::duration_cast<std::chrono::seconds>(utc_now.time_since_epoch());
    std::time_t timestamp_time_t = formatted_timestamp.count();
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&timestamp_time_t), "%Y%m%d%H%M%S");
    std::string ocsp_name = ss.str() + ".der";

    const char* ocsp_der_file = ocsp_name.c_str();

    std::vector<uint8_t> OCSPResponse = get_ocsp_request(sk_X509_value(new_stack, 0), sk_X509_value(new_stack, 1), ocsp_der_file);

    sk_X509_pop_free(new_stack, X509_free);


    return OCSPResponse;
}
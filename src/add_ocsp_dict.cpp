#include "../header/add_ocsp_dict.h"
#include "../header/addons.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <uuid/uuid.h>
#include <iostream>
#include <openssl/x509.h>
#include <openssl/err.h>


AddOCSPDict::AddOCSPDict() {
    // Initialize private member variables or perform any necessary setup
}


// Function to convert Vec<uint8_t> to STACK_OF(X509)*
STACK_OF(X509)* vec_uint8_to_stack_of_x509(const std::vector<uint8_t>& vec) {
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

size_t extract_integer_2(const std::string& input) {
    // Find the position of the first non-whitespace character
    size_t start_pos = input.find_first_not_of(" \t\n\v\f\r");
    if (start_pos == std::string::npos) {
        // No non-whitespace character found
        throw std::invalid_argument("No integer found in the input string.");
    }

    // Find the position of the first non-digit character after the start position
    size_t pos = start_pos;
    while (pos < input.size() && std::isdigit(input[pos])) {
        ++pos;
    }

    // Convert the substring containing the integer part to size_t
    return std::stoul(input.substr(start_pos, pos - start_pos));
}

// Convert a single size_t value to its string representation
std::string size_t_to_string_2(size_t value) {
    return std::to_string(value);
}

// Convert a vector of size_t values to a string
std::string vector_to_string_2(const std::vector<size_t>& input) {
    std::string output;
    for (size_t i = 0; i < input.size(); ++i) {
        output += size_t_to_string_2(input[i]);
        if (i < input.size() - 1) {
            output += " "; // Use space as delimiter for all except the last value
        }
    }
    return output;
}

std::unordered_map<std::string, std::vector<uint8_t> > AddOCSPDict::add_ocsp_dict(std::string max_index_string, std::unordered_map<std::string, std::vector<uint8_t> > update_byterange, std::vector<uint8_t> catalog_dict, const std::vector<uint8_t>& vec_ocsp){

    Addons adns;

    std::vector<uint8_t> null;
    size_t max_index = 0;
    try {
        size_t number = std::stoull(max_index_string);
        if (number <= std::numeric_limits<size_t>::max()) {
            max_index = number;
        } else {
            std::cout << "Parsed number is too large to fit into size_t" << std::endl;
        }
    } catch (const std::exception& e) {
            std::cout << "Failed to parse the string as a number" << std::endl;
    }


    // Convert Vec<uint8_t> back to STACK_OF(X509)*
    STACK_OF(X509)* new_stack = adns.vec_uint8_to_stack_of_x509(update_byterange.at("stack_cert"));
    if (!new_stack) {
        std::cerr << "Error converting Vec<uint8_t> back to STACK_OF(X509)*." << std::endl;
    }

    // Convert STACK_OF(X509)* to a list of Vec<uint8_t>
    std::vector<std::vector<uint8_t> > cert_list = adns.stack_of_x509_to_vec_list(new_stack);

    // Output the size of each std::vector<uint8_t> in the list
    // for (std::vector<std::vector<uint8_t> >::iterator it = cert_list.begin(); it != cert_list.end(); ++it) {
    //     std::cout << "Size of std::vector<uint8_t>: " << it->size() << std::endl;
    // }

    sk_X509_pop_free(new_stack, X509_free);

    std::string dssObj = "\n/DSS " + std::to_string(max_index+1) + " 0 R";


    // Convert the string characters to uint8_t values and insert into uintVector
    for (size_t i = 0; i < dssObj.size(); ++i) {
        catalog_dict.insert(catalog_dict.begin() + catalog_dict.size() - 10, static_cast<uint8_t>(dssObj[i]));
    }

    // for (size_t i = 0; i < catalog_dict.size(); ++i) {
    //     std::cout << static_cast<char>(catalog_dict[i]) << "";
    // }
    // std::cout << std::endl;

    std::string object__start = "\n\n" + std::to_string(max_index + 1) + " 0 obj\n";
    std::string object__end = "\nendobj";
    std::string _dict = object__start + "<<\n/VRI "+ std::to_string(max_index + 2) + " 0 R" + "\n/OCSPs ["+ std::to_string(max_index + 3) + " 0 R]" + "\n/Certs ["+ std::to_string(max_index + 5) + " 0 R " + std::to_string(max_index + 6) + " 0 R]\n>>" + object__end;

    std::string uniqueID = adns.generateUUID2();

    std::string object_2_start = "\n\n" + std::to_string(max_index + 2) + " 0 obj\n";
    std::string object_2_end = "\nendobj";
    std::string _dict2 = object_2_start + "<<\n/"+ uniqueID + " <<\n/OCSP ["+ std::to_string(max_index + 3) + " 0 R]\n/Cert ["+ std::to_string(max_index + 5) + " 0 R " + std::to_string(max_index + 6) + " 0 R]\n>>\n>>" + object_2_end;

    std::string object_ocsp_start = "\n\n" + std::to_string(max_index + 3) + " 0 obj\n";
    std::string object_ocsp_end = "\nendobj";
    std::string ocsp(vec_ocsp.begin(), vec_ocsp.end());
    std::string _dictocsp = object_ocsp_start + "<<\n/Length "+ std::to_string(vec_ocsp.size()) + "\n>>\nstream\n" + ocsp + "\nendstream" + object_ocsp_end;

    std::string object_cert1_start = "\n\n" + std::to_string(max_index + 5) + " 0 obj\n";
    std::string object_cert1_end = "\nendobj";
    std::string cert0(cert_list[0].begin(), cert_list[0].end());
    std::string _dictcert1 = object_cert1_start + "<<\n/Length "+ std::to_string(cert_list[0].size()) + "\n>>\nstream\n" + cert0 +"\nendstream" + object_cert1_end;

    std::string object_cert2_start = "\n\n" + std::to_string(max_index + 6) + " 0 obj\n";
    std::string object_cert2_end = "\nendobj";
    std::string cert1(cert_list[1].begin(), cert_list[1].end());
    std::string _dictcert2 = object_cert2_start + "<<\n/Length "+ std::to_string(cert_list[1].size()) + "\n>>\nstream\n" + cert1 +"\nendstream" + object_cert2_end;

    std::string catalog_string(catalog_dict.begin(), catalog_dict.end());
    size_t catalogIndex = extract_integer_2(catalog_string.substr(0, 10));

    std::vector<size_t> x_list;

    std::vector<uint8_t> signed_pdf = update_byterange.at("signed");
    signed_pdf.insert(signed_pdf.end(), catalog_dict.begin(), catalog_dict.end());
    x_list.push_back(catalogIndex);
    signed_pdf.insert(signed_pdf.end(), _dict.begin(), _dict.end());
    x_list.push_back(max_index + 1);
    signed_pdf.insert(signed_pdf.end(), _dict2.begin(), _dict2.end());
    x_list.push_back(max_index + 2);
    signed_pdf.insert(signed_pdf.end(), _dictocsp.begin(), _dictocsp.end());
    x_list.push_back(max_index + 3);
    signed_pdf.insert(signed_pdf.end(), _dictcert1.begin(), _dictcert1.end());
    x_list.push_back(max_index + 5);
    signed_pdf.insert(signed_pdf.end(), _dictcert2.begin(), _dictcert2.end());
    x_list.push_back(max_index + 6);



    std::sort(x_list.begin(), x_list.end());
    std::string added_index_string = vector_to_string_2(x_list);
    std::vector<uint8_t> added_index = adns.stringToVector(added_index_string);


    std::unordered_map<std::string, std::vector<uint8_t> > myMap;
    myMap["signed_pdf"] = signed_pdf;
    myMap["added_index"] = added_index;
    
    return myMap;
}
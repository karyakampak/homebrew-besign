#include "../header/add_ocsp_trailer.h"
#include "../header/addons.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <openssl/err.h>
#include <random>
#include <sstream>
#include <array>

AddOCSPTrailer::AddOCSPTrailer() {
    // Initialize private member variables or perform any necessary setup
}

std::vector<uint8_t> create_ocsp_buffer_trailer(const std::vector<uint8_t>& pdf, const std::vector<uint8_t>& ocsp_dict_added, const std::vector<size_t>& added_index, const std::unordered_map<std::string, std::string>& placeholderData) {
    Addons adns;
    std::string trailer;
    trailer += "\n\nxref\n0 1\n0000000000 65535 f \n";
    size_t pdf_awal_len = pdf.size();
    for (size_t i = 0; i < added_index.size(); ++i) {
        std::vector<uint8_t> pdf_to_query = ocsp_dict_added; // Assuming get_offset is a function you have defined elsewhere
        size_t offset = adns.get_offset(pdf_awal_len, pdf_to_query, std::to_string(added_index[i])); // This needs to be defined or replaced with equivalent logic
        std::string padded_offset = std::string(10 - std::to_string(offset + 1).length(), '0') + std::to_string(offset + 1);
        trailer += std::to_string(added_index[i]) + " 1\n" + padded_offset + " 00000 n \n";
    }
    trailer += "trailer\n<<";
    trailer += "\n/Size " + std::to_string(added_index.back() + 1);
    trailer += "\n/Root " + placeholderData.at("pdf_component_root");
    trailer += "\n/Info " + placeholderData.at("pdf_component_info");\
    if(placeholderData.at("pdf_component_id") != "none"){
        trailer += "\n/ID " + placeholderData.at("pdf_component_id")+" <"+adns.generateUUIDv4()+"> ]";
        // trailer += "\n/ID " + placeholderData.at("pdf_component_id");
    }
    trailer += "\n/Prev " + placeholderData.at("pdf_component_prev");
    trailer += "\n>>\nstartxref\n" + std::to_string(ocsp_dict_added.size() + 1) + "\n%%EOF";
    return std::vector<uint8_t>(trailer.begin(), trailer.end());
}

std::vector<uint8_t> AddOCSPTrailer::add_ocsp_trailer(const std::vector<uint8_t>& pdf, const std::vector<uint8_t>& ocsp_dict_added, const std::vector<size_t>& added_index, const std::unordered_map<std::string, std::string>& placeholderData) {
    std::vector<uint8_t> result;
    std::vector<uint8_t> trailer = create_ocsp_buffer_trailer(pdf, ocsp_dict_added, added_index, placeholderData);
    result.insert(result.end(), ocsp_dict_added.begin(), ocsp_dict_added.end());
    result.insert(result.end(), trailer.begin(), trailer.end());
    return result;
}
#include "../header/get_page_reference.h"
#include "../header/addons.h"
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cstdint> // For uint8_t

GetPageReference::GetPageReference() {
    // Initialize private member variables or perform any necessary setup
}

std::vector<std::string> GetPageReference::getPage(std::vector<uint8_t> pdf_buffer, std::unordered_map<std::string, std::string> read_pdf) {

    Addons adns; //Fungsi tambahan

    std::vector<std::string> object_page_ref;
    // std::string raw_string_doc(pdf_buffer.begin(), pdf_buffer.end());
    std::string root_ref = read_pdf.at("rootRef");
    std::string query = "\n" + root_ref.substr(0, root_ref.length() - 2) + " obj";
    size_t offset_start = 0;
    

    if (size_t index = adns.findStringPosition(pdf_buffer, query, 0)) {
        offset_start = index;
        // std::cout << "Offset Start: " << offset_start<< std::endl;
    }
    size_t offset_pages_start = 0;
    if (size_t index = adns.findStringPosition(pdf_buffer, "/Pages", offset_start)) {
        offset_pages_start = index;
        // std::cout <<  "Offset Page Start: " << offset_pages_start << std::endl;
    }
    size_t offset_pages_end = 0;
    if (size_t index = adns.findStringPosition(pdf_buffer, ">>", offset_pages_start)) {
        offset_pages_end = index;
        // std::cout <<  "Offset Page End: " << offset_pages_end << std::endl;
    }

    // Get subvector from index 2 to index 5
    std::vector<uint8_t> subvector = adns.getSubVector(pdf_buffer, offset_pages_start, offset_pages_end);
    std::string raw_string = adns.vectorToString(subvector);
    std::regex pages_ref_regex("/Pages\\s+(\\d+\\s+\\d+\\s+R)");
    std::smatch match_result;
    if (std::regex_search(raw_string, match_result, pages_ref_regex)) {
        std::string match_str = match_result.str();
        // std::cout <<  "Regex 1 : " << match_str << std::endl;
        std::regex regex("/Pages\\s+(\\d+)\\s+0\\s+R");
        if (std::regex_search(match_str, match_result, regex)) {
            std::string number_match = match_result[1].str();
            // std::cout <<  "Regex 2 : " << number_match << std::endl;
            try {
                size_t number = std::stoi(number_match);
                std::string pages_object = "\n" + std::to_string(number) + " 0 obj";
                // std::vector<char> query_pages_obj(pages_object.begin(), pages_object.end());
                size_t offset_pages_obj_start = 0;
                size_t index = adns.findStringPosition(pdf_buffer, pages_object, 0);
                if (index != -1) {
                    offset_pages_obj_start = index;
                }

                size_t offset_pages_obj_end = 0;
                index = adns.findStringPosition(pdf_buffer, "endobj", offset_pages_obj_start);
                if (index != -1) {
                    offset_pages_obj_end = index;
                }


                std::vector<uint8_t> binding = adns.getSubVector(pdf_buffer, offset_pages_obj_start, offset_pages_obj_end);

                // std::string binding_str = vectorToString(binding);
                // std::cout << "Binding 1 " << binding_str << std::endl;

                size_t offset_kids_obj = 0;
                index = adns.findStringPosition(binding, "/Kids", 0);
                if (index != -1) {
                    offset_kids_obj = index;
                }
                size_t offset_kids_obj_open = 0;
                index = adns.findStringPosition(binding, "[", offset_kids_obj);
                if (index != -1) {
                    offset_kids_obj_open = index;
                }
                size_t offset_kids_obj_close = 0;
                index = adns.findStringPosition(binding, "]", offset_kids_obj_open);
                if (index != -1) {
                    offset_kids_obj_close = index;
                }


                std::vector<uint8_t> binding2 = adns.getSubVector(binding, offset_kids_obj_open, offset_kids_obj_close);
                std::string raw_string_kids = adns.vectorToString(binding2);
                // std::cout << "Binding 2 " << raw_string_kids << std::endl;
                
                object_page_ref = adns.extractPatterns(raw_string_kids);
            } catch (const std::exception& e) {
                std::cout << "Failed to parse the extracted number." << std::endl;
            }
        } else {
            std::cout << "Pattern not found in the input string." << std::endl;
        }
    } else {
        std::cout << "Failed to find the pages descriptor." << std::endl;
    }


    return object_page_ref; // Assuming error_code is used to represent ErrorStack
}

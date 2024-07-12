#include "../header/check_signature.h"
#include "../header/addons.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <openssl/err.h>

CheckSignature::CheckSignature() {
    // Initialize private member variables or perform any necessary setup
}


std::unordered_map<std::string, std::string> CheckSignature::check_signature(std::vector<uint8_t> pdf_content) {

    Addons adns;

    // Extract the trailer dictionary.
    std::vector<size_t> positions_byte_range = adns.findAllStringPosition(pdf_content, "/ByteRange");
    std::unordered_map<std::string, std::string> result;
    std::string check = "none";
    std::string raw_acroform_id = "";
    std::string raw_annots_id = "";
    std::string acroform_id_before = "";
    std::string acroform_last_start_string = "";
    std::string acroform_last_end_string = "";
    if (positions_byte_range.size() > 0) {
        check = "exist";
        std::vector<size_t> positions_annots = adns.findAllStringPosition(pdf_content, "/Annots");

        size_t offset_start = 0;
        size_t index = adns.findStringPosition(pdf_content, "[", positions_annots.back());
        if (index != -1) {
            offset_start = index;
        }


        size_t offset_start2 = 0;
        index = adns.findStringPosition(pdf_content, "]", offset_start);
        if (index != -1) {
            offset_start2 = index;
        }

        std::vector<uint8_t> binding = adns.getSubVector(pdf_content, offset_start, offset_start2);
        std::string raw_annots_id_pre = adns.vectorToString(binding);
        raw_annots_id = raw_annots_id_pre.substr(raw_annots_id_pre.find_first_of('[') + 1, raw_annots_id_pre.find_last_of(']') - raw_annots_id_pre.find_first_of('[') - 1);



        std::vector<size_t> positions_acroform = adns.findAllStringPosition(pdf_content, "\n/AcroForm");

        size_t offset_start3 = 0;
        index = adns.findStringPosition(pdf_content, " ", positions_acroform.back());
        if (index != -1) {
            offset_start3 = index;
        }


        size_t offset_start4 = 0;
        index = adns.findStringPosition(pdf_content, "R", positions_acroform.back());
        if (index != -1) {
            offset_start4 = index;
        }


        std::vector<uint8_t> binding_acroform = adns.getSubVector(pdf_content, offset_start3+1, offset_start4+1);
        std::string raw_acroform_string_result = adns.vectorToString(binding_acroform);
        std::string object_signature_start = raw_acroform_string_result.substr(0, raw_acroform_string_result.length() - 2) + "obj";
        // std::cout << "object_signature_start " << object_signature_start << std::endl;


        std::vector<size_t> positions_acroform_last = adns.findAllStringPosition(pdf_content, object_signature_start);

        size_t offset_acroform_end = 0;
        index = adns.findStringPosition(pdf_content, "endobj", positions_acroform_last.back());
        if (index != -1) {
            offset_acroform_end = index+6;
        }


        std::vector<uint8_t> acroform_last = adns.getSubVector(pdf_content, positions_acroform_last.back(), offset_acroform_end);

        size_t acroform_last_start = 0;
        index = adns.findStringPosition(acroform_last, "/Fields", 0);
        if (index != -1) {
            acroform_last_start = index;
        }

        size_t acroform_last_end = 0;
        index = adns.findStringPosition(acroform_last, "]", acroform_last_start);
        if (index != -1) {
            acroform_last_end = index;
            std::string first_part =  adns.vectorToString(acroform_last).substr(0, index);
            std::string second_part =  adns.vectorToString(acroform_last).substr(index);

            acroform_last_start_string = first_part;
            acroform_last_end_string = second_part;
        }

        acroform_id_before = object_signature_start;
    }

    result["check"] = check;
    result["raw_annots_id"] = raw_annots_id;
    result["acroform_before"] = acroform_id_before;
    result["acroform_last_start"] = acroform_last_start_string;
    result["acroform_last_end"] = acroform_last_end_string;


    return result;
}
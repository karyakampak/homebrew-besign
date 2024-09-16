#include "../header/add_signature_dict.h"
#include "../header/addons.h"
#include "../header/visualization.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdio>  // For std::remove
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <zlib.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../header/stb_image_resize2.h"
#include <opencv2/opencv.hpp>
#include <qrencode.h>

#define PIXEL_SIZE 10  // Scale of each QR code pixel block

AddSignatureDict::AddSignatureDict() {
    // Initialize private member variables or perform any necessary setup
}


void saveQRCodeAsJPG(const std::string &text, const std::string &filename) {
    // Create the QR code
    QRcode *qr = QRcode_encodeString(text.c_str(), 0, QR_ECLEVEL_H, QR_MODE_8, 1);
    if (qr == nullptr) {
        std::cerr << "Failed to generate QR code" << std::endl;
        return;
    }

    // Set the image size (QR code size * pixel scale)
    int imageSize = qr->width * PIXEL_SIZE;
    cv::Mat qrImage = cv::Mat::zeros(imageSize, imageSize, CV_8UC3);

    // Fill the image with the QR code data
    for (int y = 0; y < qr->width; y++) {
        for (int x = 0; x < qr->width; x++) {
            cv::Rect rect(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
            if (qr->data[y * qr->width + x] & 1) {
                cv::rectangle(qrImage, rect, cv::Scalar(0, 0, 0), cv::FILLED);  // Black for QR code modules
            } else {
                cv::rectangle(qrImage, rect, cv::Scalar(255, 255, 255), cv::FILLED);  // White background
            }
        }
    }

    // Save the image as JPG
    if (cv::imwrite(filename, qrImage)) {
        // std::cout << "QR code saved as " << filename << std::endl;
    } else {
        std::cerr << "Failed to save the QR code as JPG" << std::endl;
    }

    // Clean up QR code
    QRcode_free(qr);
}


std::string get_page(const std::vector<uint8_t>& pdf_content, const std::string& object_page_ref) {
    
    Addons adns;

    std::vector<uint8_t> object_page;
    std::vector<uint8_t> pdf = pdf_content;
    std::string raw_string_doc(pdf_content.begin(), pdf_content.end());
    std::string query = "\n" + object_page_ref.substr(0, object_page_ref.length() - 2) + " obj";
    std::vector<uint8_t> query_offset(query.begin(), query.end());


    std::vector<size_t> positions = adns.findAllStringPosition(pdf_content, query);


    size_t endobjPosition = 0;
    if (size_t index = adns.findStringPosition(pdf_content, "endobj", positions.back())) {
        endobjPosition = index;
    }

    std::vector<uint8_t> binding(pdf.begin() + positions.back(), pdf.begin() + endobjPosition);
    std::string raw_string_result(binding.begin(), binding.end());
    // std::cout << "Page Result 2: " << raw_string_result << std::endl;
    return raw_string_result;
}



bool containsAnnots(const std::vector<uint8_t>& pdfData) {
    std::string annotsString = "/Annots";
    size_t annotsPos = 0;

    // Search for "/Annots" in the PDF data
    for (size_t i = 0; i < pdfData.size(); ++i) {
        if (pdfData[i] == annotsString[annotsPos]) {
            ++annotsPos;
            if (annotsPos == annotsString.size()) {
                return true; // Found "/Annots" in the PDF data
            }
        } else {
            annotsPos = 0; // Reset position if mismatch
        }
    }

    return false; // "/Annots" not found in the PDF data
}

std::unordered_map<std::string, std::string> checkAnnotOnPage(std::string page_dict) {

    Addons adns;

    std::unordered_map<std::string, std::string> result;
    std::string page_before;
    std::string page_after;

    std::vector<uint8_t> page_vec(page_dict.begin(), page_dict.end());

    // Check if the PDF data contains "/Annots"
    if (containsAnnots(page_vec)) {
        size_t annotPosition = 0;
        if (size_t index_1 = adns.findStringPosition(page_vec, "/Annots", 0)) {
            if (size_t index_2 = adns.findStringPosition(page_vec, "]", index_1)) {
                annotPosition = index_2;
                std::vector<uint8_t> page_before_vec(page_vec.begin(), page_vec.begin() + index_2);
                std::string page_beforex(page_before_vec.begin(), page_before_vec.end());
                page_before = page_beforex;

                std::vector<uint8_t> page_after_vec(page_vec.begin() + index_2, page_vec.end());
                std::string page_afterx(page_after_vec.begin(), page_after_vec.end());
                page_after = page_afterx;
            }
        }
    } else {
        result["status"] = "null";
        result["page_before"] = "null";
        result["page_after"] = "null";
        return result;
    }

    
    result["status"] = "exist";
    result["page_before"] = page_before;
    result["page_after"] = page_after;

    // std::unordered_map<std::string, std::string>::iterator it;
    // for (it = result.begin(); it != result.end(); ++it) {
    //     std::cout << "Key ReadPdf: " << it->first << ", Value: " << it->second << std::endl;
    // }

    return result;

}

std::string get_catalog(const std::vector<uint8_t>& pdf_content, const std::unordered_map<std::string, std::string>& read_pdf) {
    std::vector<uint8_t> object_page;
    std::vector<uint8_t> pdf = pdf_content;
    std::string raw_string_doc(pdf_content.begin(), pdf_content.end());
    std::string rootRef = read_pdf.at("rootRef");
    std::string query = "\n" + rootRef.substr(0, rootRef.length() - 2) + " obj";
    std::vector<uint8_t> query_offset(query.begin(), query.end());
    size_t offset_start = 0;
    std::vector<uint8_t>::iterator it = std::search(pdf.begin(), pdf.end(), query_offset.begin(), query_offset.end());
    if (it != pdf.end()) {
        offset_start = std::distance(pdf.begin(), it);
    }
    std::string query_offset2 = "endobj";
    std::vector<uint8_t> query_offset_vec2(query_offset2.begin(), query_offset2.end());
    size_t offset_start2 = 0;
    it = std::search(pdf.begin() + offset_start, pdf.end(), query_offset_vec2.begin(), query_offset_vec2.end());
    if (it != pdf.end()) {
        offset_start2 = std::distance(pdf.begin(), it);
    }
    std::vector<uint8_t> binding(pdf.begin() + offset_start, pdf.begin() + offset_start2);
    std::string raw_string_result(binding.begin(), binding.end());
    return raw_string_result;
}

size_t extract_integer(const std::string& input) {
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
std::string size_t_to_string(size_t value) {
    return std::to_string(value);
}

// Convert a vector of size_t values to a string
std::string vector_to_string(const std::vector<size_t>& input) {
    std::string output;
    for (size_t i = 0; i < input.size(); ++i) {
        output += size_t_to_string(input[i]);
        if (i < input.size() - 1) {
            output += " "; // Use space as delimiter for all except the last value
        }
    }
    return output;
}

std::unordered_map<std::string, std::vector<uint8_t> > AddSignatureDict::add_signaturedict(std::vector<unsigned char> pdf_content, const char* input_image_path_string, int page, int visibility, float x, float y, float width, float height, std::vector<std::string> object_page_ref, std::unordered_map<std::string, std::string> read_pdf, std::unordered_map<std::string, std::string> check_sign) {

    Addons adns;

    size_t max_index = 0;
    if (read_pdf.count("maxIndex") > 0) {
        try {
            size_t number = std::stoull(read_pdf.at("maxIndex"));
            if (number <= std::numeric_limits<size_t>::max()) {
                max_index = number;
            } else {
                std::cout << "Parsed number is too large to fit into size_t" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Failed to parse the string as a number" << std::endl;
        }
    }
    std::chrono::system_clock::time_point utc_now = std::chrono::system_clock::now();
    std::chrono::seconds formatted_timestamp = std::chrono::duration_cast<std::chrono::seconds>(utc_now.time_since_epoch());
    std::time_t timestamp_time_t = formatted_timestamp.count();
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&timestamp_time_t), "%Y%m%d%H%M%S");
    std::string formatted_timestamp_with_z = ss.str() + "Z";
    std::vector<unsigned char> result;
    std::string object_signature_start = "\n" + std::to_string(max_index + 1) + " 0 obj\n<<\n";
    std::string contents = "/Contents <" + std::string(35000, '0') + ">";
    std::string object_signature_end = "\n>>\nendobj";
    // std::string signature_dict = object_signature_start + "/Type /Sig\n/Filter /Adobe.PPKLite\n/SubFilter /adbe.pkcs7.detached\n/ByteRange [0 /********** /********** /**********]\n" + contents + "\n/Reason (Signed Certificate.)\n/M (D:" + formatted_timestamp_with_z + ")\n/ContactInfo (sign@example.com)\n/Name (Example)\n/Location (Jakarta)" + object_signature_end;
    std::string signature_dict = object_signature_start + "/Type /Sig\n/Filter /Adobe.PPKLite\n/SubFilter /ETSI.CAdES.detached\n/ByteRange [0 /********** /********** /**********]\n" + contents + "\n/Reason (Signed Certificate.)\n/M (D:" + formatted_timestamp_with_z + ")\n/ContactInfo (sign@example.com)\n/Name (Example)\n/Location (Jakarta)" + object_signature_end;
    std::string object_annot_start = "\n\n" + std::to_string(max_index + 2) + " 0 obj\n<<\n";
    std::string object_annot_end = "\n>>\nendobj";
    std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
    std::chrono::system_clock::duration duration_since_epoch = current_time.time_since_epoch();
    std::chrono::milliseconds::rep milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch).count();
    std::string object_page_ref_annot = object_page_ref[page - 1];
    std::string annot_dict;
    std::vector<unsigned char> img_dict_signature;
    if (visibility == 1) {


        // std::string text = "https://bsre-bssn.go.id";
        // std::string filename = std::to_string(milliseconds)+".jpg";
        // saveQRCodeAsJPG(text, filename);

        std::ifstream image_file(input_image_path_string, std::ios::binary);
        // std::ifstream image_file(filename.c_str(), std::ios::binary);
        if (image_file) {


            std::vector<unsigned char> image_data((std::istreambuf_iterator<char>(image_file)),
                                                std::istreambuf_iterator<char>());

            int widthImage, heightImage, channels;
            // std::vector<uint8_t> imageData = adns.decodePNG(filename.c_str(), widthImage, heightImage, channels);
            std::vector<uint8_t> imageData = adns.decodePNG(input_image_path_string, widthImage, heightImage, channels);



            Visualization vzsl;
            // std::vector<double> positions = vzsl.get_position("/Users/pusopskamsinas/Documents/Pribadi/Cpp/dksign_v2/input/Doc1.pdf", 0, "#");

            std::vector<uint8_t> visualization = vzsl.get_image(image_data);
            std::string visualization_string(visualization.begin(), visualization.end());
            // std::cout << visualization_string << std::endl;

            // Jika menggunnakan tanda #
            // x = positions[0];
            // y = positions[1];

            float rectWidth;
            float rectHeight;
            if (width/height > widthImage/heightImage)
            {
                rectHeight = height;
                rectWidth = rectHeight/heightImage*widthImage;
                x = x + ((width-rectWidth)/2);
            } else{
                rectWidth = width;
                rectHeight = (rectWidth/widthImage)*heightImage;
                y = y + ((height-rectHeight)/2);
            }

            // Jika menggunnakan tanda #
            // x = positions[0] - (rectWidth/2);
            // y = positions[1] - (rectHeight/2);
            

            annot_dict = object_annot_start + "/Type /Annot\n/Subtype /Widget\n/FT /Sig\n/Rect [" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(x + rectWidth) + " " + std::to_string(y+rectHeight) +  "]\n/AP <<\n/N " + std::to_string(max_index + 5) + " 0 R\n>>\n/V " + std::to_string(max_index + 1) + " 0 R\n/T (Signature_" + std::to_string(milliseconds) + ")\n/F 4\n/P " + object_page_ref_annot + object_annot_end;
            
            std::stringstream outputStream;
            outputStream.write(reinterpret_cast<const char*>(imageData.data()), imageData.size());
            std::string str = outputStream.str();

            float widthSign = static_cast<float>(widthImage) / (widthImage/rectWidth); // Convert numerator to float before division
            float heightSign = static_cast<float>(heightImage) / (heightImage/rectHeight); // Convert numerator to float before division

            // Copy the contents into a vector
            std::vector<unsigned char> vec(visualization_string.begin(), visualization_string.end());

            std::string img_obj_0_start = "\n\n" + std::to_string(max_index + 5) + " 0 obj\n";
            std::string img_obj_0_end = "\nendobj";
            std::string stream_obj_0 = "q\nq\n"+std::to_string(widthSign)+" 0 0 "+std::to_string(heightSign)+" 0 0 cm\n/X" + std::to_string(milliseconds) + " Do\nQ\n1 0 0 1 "+std::to_string(widthSign)+" "+std::to_string(heightSign)+" cm\nQ\n";
            // std::string stream_obj_0 = "q\nq\n"+std::to_string(widthSign)+" 0 0 -"+std::to_string(heightSign)+" 0 "+std::to_string(heightSign)+" cm\n/X" + std::to_string(milliseconds) + " Do\nQ\n1 0 0 1 "+std::to_string(widthSign)+" "+std::to_string(heightSign)+" cm\nQ\n";
            std::string img_obj_0_dict = img_obj_0_start + "<<\n/Length " + std::to_string(stream_obj_0.size()) + "\n/Type /XObject\n/Subtype /Form\n/Resources <<\n/XObject <<\n/X" + std::to_string(milliseconds) + " " + std::to_string(max_index + 6) + " 0 R\n>>\n>>\n/FormType 1\n/BBox [0.0 0.0 "+std::to_string(widthSign)+" "+std::to_string(heightSign)+"]\n/Matrix [1 0 0 1 0 0]\n>>\nstream\n" + stream_obj_0 + "\nendstream" + img_obj_0_end;
            std::string img_obj_1_start = "\n\n" + std::to_string(max_index + 6) + " 0 obj\n<<\n";
            std::string img_obj_1_end = "\nendobj";
            std::string img_obj_1_dict_1 = img_obj_1_start + "/Type /XObject\n/Subtype /Image\n/Filter /DCTDecode\n/Width " + std::to_string(widthImage) + "\n/Height " + std::to_string(heightImage) + "\n/ColorSpace /DeviceRGB\n/BitsPerComponent 8\n/Length " + std::to_string(imageData.size()) + "\n>>\nstream\n";
            // std::string img_obj_1_dict_1 = img_obj_1_start + "/Type /XObject\n/Subtype /Image\n/Width " + std::to_string(widthImage) + "\n/Height " + std::to_string(heightImage) + "\n/ColorSpace /DeviceRGB\n/BitsPerComponent 8\n/Length " + std::to_string(imageData.size()) + "\n>>\nstream\n";
            std::string img_obj_1_dict_2 = "\nendstream" + img_obj_1_end;
            std::vector<unsigned char> img_obj_final_vec;
            img_obj_final_vec.insert(img_obj_final_vec.end(), img_obj_0_dict.begin(), img_obj_0_dict.end());
            img_obj_final_vec.insert(img_obj_final_vec.end(), img_obj_1_dict_1.begin(), img_obj_1_dict_1.end());
            img_obj_final_vec.insert(img_obj_final_vec.end(), vec.begin(), vec.end());
            img_obj_final_vec.insert(img_obj_final_vec.end(), img_obj_1_dict_2.begin(), img_obj_1_dict_2.end());

            img_dict_signature = img_obj_final_vec;

        } else {
            std::cerr << "Error reading from the input image file" << std::endl;
        }


        // if (std::remove(filename.c_str()) != 0) {
        //     std::perror("Error deleting file");
        // } else {
        //     std::cout << "File successfully deleted\n";
        // }
        
    } else {
        annot_dict = object_annot_start + "/Type /Annot\n/Subtype /Widget\n/FT /Sig\n/Rect [0 0 0 0]\n/V " + std::to_string(max_index + 1) + " 0 R\n/T (Signature_" + std::to_string(milliseconds) + ")\n/F 4\n/P " + object_page_ref_annot + object_annot_end;
    }
    std::string object_acroform_start = "\n\n" + std::to_string(max_index + 3) + " 0 obj\n<<\n";
    std::string object_acroform_end = "\n>>\nendobj";
    std::string acroform_dict = object_acroform_start + "/Type /AcroForm\n/SigFlags 3\n/Fields [" + std::to_string(max_index + 2) + " 0 R]" + object_acroform_end;
    if (check_sign.count("check") > 0 && check_sign.at("check") == "exist") {
        acroform_dict = "\n\n" + check_sign.at("acroform_last_start") + " " + std::to_string(max_index + 2) + " 0 R" + check_sign.at("acroform_last_end");
    }
    std::string object__start = "\n\n" + std::to_string(max_index + 4) + " 0 obj\n";
    std::string object__end = "\nendobj";
    std::string _dict = object__start + "undefined" + object__end;
    std::vector<unsigned char> pdf_content_page_dict = pdf_content;

    std::string page_dict = get_page(pdf_content_page_dict, object_page_ref_annot);
    std::unordered_map<std::string, std::string> annots_on_page = checkAnnotOnPage(page_dict);
    std::vector<unsigned char> pdf_content_catalog_dict = pdf_content;
    std::unordered_map<std::string, std::string> read_pdf_dict = read_pdf;
    std::string catalog_dict = get_catalog(pdf_content_catalog_dict, read_pdf_dict);
    std::string root_obj = "\n" + catalog_dict.substr(0, catalog_dict.length() - 4) + "\n/AcroForm " + std::to_string(max_index + 3) + " 0 R\n>>\nendobj";
    std::vector<unsigned char> root_obj_vec(root_obj.begin(), root_obj.end());


    std::string page_obj = "\n" + page_dict.substr(0, page_dict.length() - 4) + "/Annots [" + std::to_string(max_index + 2) + " 0 R]\n\n>>\nendobj";
    if (annots_on_page.at("status") == "exist") {
        page_obj = "\n" + annots_on_page.at("page_before") + " " + std::to_string(max_index + 2) + " 0 R" + annots_on_page.at("page_after") + "\nendobj";
    }


    std::vector<unsigned char> page_obj_vec(page_obj.begin(), page_obj.end());


    size_t rootIndex = extract_integer(read_pdf.at("rootRef"));
    size_t pageIndex = extract_integer(page_dict.substr(0, 10));

    std::vector<size_t> x_list;
    if (visibility == 1) {
        result.insert(result.end(), pdf_content.begin(), pdf_content.end());
        result.insert(result.end(), signature_dict.begin(), signature_dict.end());
        x_list.push_back(max_index + 1);
        result.insert(result.end(), annot_dict.begin(), annot_dict.end());
        x_list.push_back(max_index + 2);
        result.insert(result.end(), acroform_dict.begin(), acroform_dict.end());
        x_list.push_back(max_index + 3);
        result.insert(result.end(), _dict.begin(), _dict.end());
        x_list.push_back(max_index + 4);
        result.insert(result.end(), img_dict_signature.begin(), img_dict_signature.end());
        x_list.push_back(max_index + 5);
        x_list.push_back(max_index + 6);  // XObject
        if (check_sign.count("check") > 0 && check_sign.at("check") == "exist") {
            result.insert(result.end(), page_obj_vec.begin(), page_obj_vec.end());
            x_list.push_back(pageIndex);
        } else {
            result.insert(result.end(), root_obj_vec.begin(), root_obj_vec.end());
            x_list.push_back(rootIndex);
            result.insert(result.end(), page_obj_vec.begin(), page_obj_vec.end());
            x_list.push_back(pageIndex);
        }
    } else {
        result.insert(result.end(), pdf_content.begin(), pdf_content.end());
        result.insert(result.end(), signature_dict.begin(), signature_dict.end());
        x_list.push_back(max_index + 1);
        result.insert(result.end(), annot_dict.begin(), annot_dict.end());
        x_list.push_back(max_index + 2);
        result.insert(result.end(), acroform_dict.begin(), acroform_dict.end());
        x_list.push_back(max_index + 3);
        result.insert(result.end(), _dict.begin(), _dict.end());
        x_list.push_back(max_index + 4);
        if (check_sign.count("check") > 0 && check_sign.at("check") == "exist") {
            result.insert(result.end(), page_obj_vec.begin(), page_obj_vec.end());
            x_list.push_back(pageIndex);
        } else {
            result.insert(result.end(), root_obj_vec.begin(), root_obj_vec.end());
            x_list.push_back(rootIndex);
            result.insert(result.end(), page_obj_vec.begin(), page_obj_vec.end());
            x_list.push_back(pageIndex);
        }
    }

    std::sort(x_list.begin(), x_list.end());
    std::string added_index_string = vector_to_string(x_list);
    std::vector<uint8_t> added_index = adns.stringToVector(added_index_string);

    // AddImage adimg;
    // std::string imageStream = adimg.addImage(1);
    // std::cout << imageStream << std::endl;

    // Populate the map with data
    std::unordered_map<std::string, std::vector<uint8_t> > myMap;
    myMap["added_dict"] = result;
    myMap["catalog_dict"] = root_obj_vec;
    myMap["added_index"] = added_index;

    return myMap;
}
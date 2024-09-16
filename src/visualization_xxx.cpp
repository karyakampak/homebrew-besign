#include <Python.h>
#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <stdexcept>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include "../header/addons.h"
#include "../header/visualization.h"
#include <random>
#include <algorithm>

Visualization::Visualization() {
    // Initialize private member variables or perform any necessary setup
}

// Function to generate a random string of given length
std::string generate_random_string(size_t length) {
    // Define the characters that can be used in the random string
    const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    const size_t num_characters = characters.size();
    
    // Create a random number generator
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 generator(rd());  // Seed the generator
    std::uniform_int_distribution<> distribution(0, num_characters - 1);  // Define the range

    // Generate the random string
    std::string random_string;
    for (size_t i = 0; i < length; ++i) {
        random_string += characters[distribution(generator)];
    }

    return random_string;
}

// Function to decode a Base64-encoded string using OpenSSL
std::string base64_decode(const std::string& encoded) {
    std::string decoded;

    // Create a BIO for Base64 decoding
    BIO *bio = BIO_new(BIO_f_base64());
    BIO *bio_mem = BIO_new_mem_buf(encoded.data(), encoded.size());
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // Disable newline handling

    // Chain the memory BIO with the Base64 BIO
    bio = BIO_push(bio, bio_mem);

    // Decode
    char buffer[1024];
    int bytes_read;
    while ((bytes_read = BIO_read(bio, buffer, sizeof(buffer))) > 0) {
        decoded.append(buffer, bytes_read);
    }

    // Clean up
    BIO_free_all(bio);

    return decoded;
}

std::vector<uint8_t> process_image(const std::string& image_path) {
    Py_Initialize();

    // Import the Cython module
    PyObject* pName = PyUnicode_DecodeFSDefault("helper");
    PyObject* pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule != nullptr) {
        // Get reference to the function
        PyObject* pFunc = PyObject_GetAttrString(pModule, "process_image");
        if (pFunc && PyCallable_Check(pFunc)) {
            // Prepare arguments for the Python function
            PyObject* pArgs = PyTuple_New(1);  // 7 arguments for add_qr_to_char
            PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(image_path.c_str()));

            // Call the function
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            if (pValue != nullptr) {
                // Process the result
                std::string result = PyUnicode_AsUTF8(pValue);
                std::string decoded_data = base64_decode(result);
                std::vector<uint8_t> pdf(decoded_data.begin(), decoded_data.end());
                // std::cout << decoded_data << std::endl;
                Py_DECREF(pValue);
                return pdf;
            } else {
                PyErr_Print();
            }
            Py_XDECREF(pArgs);
        } else {
            PyErr_Print();
        }
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
    } else {
        PyErr_Print();
    }

    Py_Finalize();
}

std::vector<uint8_t> process_qr(const std::string& url) {
    Py_Initialize();

    // Import the Cython module
    PyObject* pName = PyUnicode_DecodeFSDefault("helper");
    PyObject* pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule != nullptr) {
        // Get reference to the function
        PyObject* pFunc = PyObject_GetAttrString(pModule, "process_qr");
        if (pFunc && PyCallable_Check(pFunc)) {
            // Prepare arguments for the Python function
            PyObject* pArgs = PyTuple_New(1);  // 7 arguments for add_qr_to_char
            PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(url.c_str()));

            // Call the function
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            if (pValue != nullptr) {
                // Process the result
                std::string result = PyUnicode_AsUTF8(pValue);
                std::string decoded_data = base64_decode(result);
                std::vector<uint8_t> pdf(decoded_data.begin(), decoded_data.end());
                // std::cout << decoded_data << std::endl;
                Py_DECREF(pValue);
                return pdf;
            } else {
                PyErr_Print();
            }
            Py_XDECREF(pArgs);
        } else {
            PyErr_Print();
        }
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
    } else {
        PyErr_Print();
    }

    Py_Finalize();
}

std::vector<uint8_t> add_image_to_pdf(const std::string& image_path, int x, int y, int width, int height, const std::string& base64_pdf) {
    Py_Initialize();

    // Import the Cython module
    PyObject* pName = PyUnicode_DecodeFSDefault("helper");
    PyObject* pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule != nullptr) {
        // Get reference to the function
        PyObject* pFunc = PyObject_GetAttrString(pModule, "add_image_to_pdf");
        if (pFunc && PyCallable_Check(pFunc)) {
            // Prepare arguments for the Python function
            PyObject* pArgs = PyTuple_New(6);  // 7 arguments for add_qr_to_char
            PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(image_path.c_str()));
            PyTuple_SetItem(pArgs, 1, PyLong_FromLong(x));
            PyTuple_SetItem(pArgs, 2, PyLong_FromLong(y));
            PyTuple_SetItem(pArgs, 3, PyLong_FromLong(width));
            PyTuple_SetItem(pArgs, 4, PyLong_FromLong(height));
            PyTuple_SetItem(pArgs, 5, PyUnicode_FromString(base64_pdf.c_str()));

            // Call the function
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            if (pValue != nullptr) {
                // Process the result
                std::string result = PyUnicode_AsUTF8(pValue);
                std::string decoded_data = base64_decode(result);
                std::vector<uint8_t> pdf(decoded_data.begin(), decoded_data.end());
                // std::cout << decoded_data << std::endl;
                Py_DECREF(pValue);
                return pdf;
            } else {
                PyErr_Print();
            }
            Py_XDECREF(pArgs);
        } else {
            PyErr_Print();
        }
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
    } else {
        PyErr_Print();
    }

    Py_Finalize();
}

std::vector<uint8_t> add_image_to_char(const std::string& pdf_path, int page_num, const std::string& target_char, 
                                const std::string& image_path, int width, int height, const std::string& base64_pdf) {
    Py_Initialize();

    // Import the Cython module
    PyObject* pName = PyUnicode_DecodeFSDefault("helper");
    PyObject* pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule != nullptr) {
        // Get reference to the function
        PyObject* pFunc = PyObject_GetAttrString(pModule, "add_image_to_char");
        if (pFunc && PyCallable_Check(pFunc)) {
            // Prepare arguments for the Python function
            PyObject* pArgs = PyTuple_New(7);  // 7 arguments for add_qr_to_char
            PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(pdf_path.c_str()));
            PyTuple_SetItem(pArgs, 1, PyLong_FromLong(page_num));
            PyTuple_SetItem(pArgs, 2, PyUnicode_FromString(target_char.c_str()));
            PyTuple_SetItem(pArgs, 3, PyUnicode_FromString(image_path.c_str()));
            PyTuple_SetItem(pArgs, 4, PyLong_FromLong(width));
            PyTuple_SetItem(pArgs, 5, PyLong_FromLong(height));
            PyTuple_SetItem(pArgs, 6, PyUnicode_FromString(base64_pdf.c_str()));

            // Call the function
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            if (pValue != nullptr) {
                // Process the result
                std::string result = PyUnicode_AsUTF8(pValue);
                std::string decoded_data = base64_decode(result);
                std::vector<uint8_t> pdf(decoded_data.begin(), decoded_data.end());
                // std::cout << decoded_data << std::endl;
                Py_DECREF(pValue);
                return pdf;
            } else {
                PyErr_Print();
            }
            Py_XDECREF(pArgs);
        } else {
            PyErr_Print();
        }
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
    } else {
        PyErr_Print();
    }

    Py_Finalize();
}

std::vector<uint8_t> add_qr_to_char(const std::string& pdf_path, int page_num, const std::string& target_char, 
                                const std::string& url, int width, int height, const std::string& base64_pdf) {
    Py_Initialize();

    // Import the Cython module
    PyObject* pName = PyUnicode_DecodeFSDefault("helper");
    PyObject* pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule != nullptr) {
        // Get reference to the function
        PyObject* pFunc = PyObject_GetAttrString(pModule, "add_qr_to_char");
        if (pFunc && PyCallable_Check(pFunc)) {
            // Prepare arguments for the Python function
            PyObject* pArgs = PyTuple_New(7);  // 7 arguments for add_qr_to_char
            PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(pdf_path.c_str()));
            PyTuple_SetItem(pArgs, 1, PyLong_FromLong(page_num));
            PyTuple_SetItem(pArgs, 2, PyUnicode_FromString(target_char.c_str()));
            PyTuple_SetItem(pArgs, 3, PyUnicode_FromString(url.c_str()));
            PyTuple_SetItem(pArgs, 4, PyLong_FromLong(width));
            PyTuple_SetItem(pArgs, 5, PyLong_FromLong(height));
            PyTuple_SetItem(pArgs, 6, PyUnicode_FromString(base64_pdf.c_str()));

            // Call the function
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            if (pValue != nullptr) {
                // Process the result
                std::string result = PyUnicode_AsUTF8(pValue);
                std::string decoded_data = base64_decode(result);
                std::vector<uint8_t> pdf(decoded_data.begin(), decoded_data.end());
                // std::cout << decoded_data << std::endl;
                Py_DECREF(pValue);
                return pdf;
            } else {
                PyErr_Print();
            }
            Py_XDECREF(pArgs);
        } else {
            PyErr_Print();
        }
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
    } else {
        PyErr_Print();
    }

    Py_Finalize();
}


// Function to extract all numbers before " 0 obj"
std::vector<int> extract_numbers(const std::string& input) {
    std::vector<int> numbers;
    size_t pos = 0;
    
    while ((pos = input.find(" 0 obj", pos)) != std::string::npos) {
        // Walk backward to find the start of the number
        size_t start = pos;
        while (start > 0 && isdigit(input[start - 1])) {
            --start;
        }
        
        if (start != pos) {
            int number = std::stoi(input.substr(start, pos - start));
            numbers.push_back(number);
        }
        
        pos += 6; // Move past " 0 obj"
    }

    return numbers;
}

// Function to replace all "x 0 obj" with "x+y 0 obj"
std::string replace_objects(std::string input, int y) {
    size_t pos = 0;
    
    while ((pos = input.find(" 0 obj", pos)) != std::string::npos) {
        // Walk backward to find the start of the number
        size_t start = pos;
        while (start > 0 && isdigit(input[start - 1])) {
            --start;
        }
        
        if (start != pos) {
            int number = std::stoi(input.substr(start, pos - start));
            // Create the replacement string
            std::string replacement = "" + std::to_string(number + y) + " 0 obj";
            input.replace(start, pos - start + 6, replacement);
            pos = start + replacement.length(); // Move past the replacement
        } else {
            pos += 6; // Move past " 0 obj" if no number was found
        }
    }

    return input;
}

std::string replace_references(std::string input, int y) {
    size_t pos = 0;

    while ((pos = input.find(" 0 R", pos)) != std::string::npos) {
        // Walk backward to find the start of the number
        size_t start = pos;
        while (start > 0 && std::isdigit(input[start - 1])) {
            --start;
        }

        // If we found a valid number before " 0 R"
        if (start != pos) {
            int number = std::stoi(input.substr(start, pos - start));

            // Create the replacement string "(number + y) 0 R"
            std::string replacement = std::to_string(number + y) + " 0 R";

            // Replace the original "x 0 R" with "(x + y) 0 R"
            input.replace(start, pos - start + 5, replacement);

            // Move past the replacement
            pos = start + replacement.length();
        } else {
            // Move past " 0 R" if no number was found
            pos += 5;
        }
    }

    return input;
}

// // Function to replace all "x 0 obj" with "(x+y) 0 obj"
// std::string replace_references(std::string input, int y) {
//     size_t pos = 0;
    
//     while ((pos = input.find(" 0 R", pos)) != std::string::npos) {
//         // Walk backward to find the start of the number
//         size_t start = pos;
//         while (start > 0 && isdigit(input[start - 1])) {
//             --start;
//         }
        
//         if (start != pos) {
//             int number = std::stoi(input.substr(start, pos - start));
//             // Create the replacement string
//             std::string replacement = "" + std::to_string(number + y) + " 0 R";
//             input.replace(start, pos - start + 4, replacement);
//             pos = start + replacement.length(); // Move past the replacement
//         } else {
//             pos += 4; // Move past " 0 obj" if no number was found
//         }
//     }

//     return input;
// }


void replaceAll(std::string &str, const std::string &from, const std::string &to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();  // Move past the replaced part to avoid infinite loop
    }
}


std::vector<uint8_t> getImageObject(std::vector<uint8_t> pdf_buffer, int width, int height, int start_object_number){
    Addons adns;
    int y = start_object_number; // Example value for y

    size_t start_obj = 0;
    if (size_t index = adns.findStringPosition(pdf_buffer, "5 0 obj", 0)) {
        start_obj = index;
    }

    size_t end_obj = start_obj+1;
    if (size_t index = adns.findStringPosition(pdf_buffer, "xref", start_obj)) {
        end_obj = index;
    }

    std::vector<uint8_t> obj = std::vector<uint8_t>(pdf_buffer.begin() + start_obj, pdf_buffer.begin() + end_obj);
    std::string obj_string(obj.begin(), obj.end());
    // std::cout << obj_string << std::endl;






    size_t start_image_name = 0;
    if (size_t index = adns.findStringPosition(pdf_buffer, "/XObject<</", 0)) {
        start_image_name = index;
    }
    size_t end_image_name = 0;
    if (size_t index = adns.findStringPosition(pdf_buffer, " ", start_image_name)) {
        end_image_name = index;
    }
    std::vector<uint8_t> image_name = std::vector<uint8_t>(pdf_buffer.begin() + start_image_name + 11, pdf_buffer.begin() + end_image_name);
    std::string image_name_string(image_name.begin(), image_name.end());
    // std::cout << image_name_string << std::endl;


    size_t length = image_name_string.size();  // Desired length of the random string
    std::string random_string = generate_random_string(length);
    // Print the random string
    // std::cout << "Random String: " << random_string << std::endl;
    


    // size_t xxx = 0;
    // if (size_t index = adns.findStringPosition(obj, "6 0 obj", 0)) {
    //     xxx = index;
    // }
    // size_t yyy = 0;
    // if (size_t index = adns.findStringPosition(obj, "<<", xxx)) {
    //     yyy = index+2;
    // }
    // std::string img_obj_0_dict = "\n/Type /XObject\n/Subtype /Form\n/Resources <<\n/XObject <<\n/" + random_string + " " + std::to_string(5) + " 0 R\n>>\n>>\n/FormType 1\n/BBox [0.0 0.0 "+std::to_string(width)+" "+std::to_string(height)+"]\n/Matrix [1 0 0 1 0 0]\n";
    // std::vector<uint8_t> img_obj_0_dict_vec(img_obj_0_dict.begin(), img_obj_0_dict.end());

    // std::vector<uint8_t> concatenatedBuffer;
    // concatenatedBuffer.reserve(obj.size() + img_obj_0_dict_vec.size());
    // concatenatedBuffer.insert(concatenatedBuffer.end(), obj.begin(), obj.begin() + yyy);
    // concatenatedBuffer.insert(concatenatedBuffer.end(), img_obj_0_dict_vec.begin(), img_obj_0_dict_vec.end());
    // concatenatedBuffer.insert(concatenatedBuffer.end(), obj.begin() + yyy, obj.end());

    // std::string bbb(concatenatedBuffer.begin(), concatenatedBuffer.end());
    // std::cout << bbb << std::endl;



    std::vector<int> numbers = extract_numbers(obj_string);
    // Print the extracted numbers
    // for (int num : numbers) {
    //     std::cout << num << std::endl;
    // }

    std::string output = replace_objects(obj_string, y);
    std::string output2 = replace_references(output, y);


    replaceAll(output2, image_name_string, random_string);
    // Print the modified string
    // std::cout << output2 << std::endl;
    std::vector<uint8_t> obj_vec(output2.begin(), output2.end());

    return obj_vec;
}

std::vector<uint8_t> Visualization::visualization(std::string &pdf_path, int page_num, std::string &target_char, std::string &image_path, std::string &url, int start_object_number, int x, int y, int width, int height){
    // Define your input parameters
    // std::string pdf_path = "/Users/pusopskamsinas/Documents/Pribadi/Cpp/AddImage/Doc1.pdf";
    // std::string image_path = "/Users/pusopskamsinas/Documents/Pribadi/Cpp/AddImage/logo.png";
    // int page_num = 0;
    // std::string target_char = "#";
    // std::string url = "https://example.com";
    std::vector<uint8_t> obj = process_image(image_path);
    // std::vector<uint8_t> obj = process_qr(url);
    // std::vector<uint8_t> pdf = add_image_to_pdf(image_path, x, y, width, height, "JVBERi0xLjcKJcK1wrYKCjEgMCBvYmoKPDwvVHlwZS9DYXRhbG9nL1BhZ2VzIDIgMCBSPj4KZW5kb2JqCgoyIDAgb2JqCjw8L1R5cGUvUGFnZXMvQ291bnQgMS9LaWRzWzQgMCBSXT4+CmVuZG9iagoKMyAwIG9iago8PD4+CmVuZG9iagoKNCAwIG9iago8PC9UeXBlL1BhZ2UvTWVkaWFCb3hbMCAwIDU5NSA4NDJdL1JvdGF0ZSAwL1Jlc291cmNlcyAzIDAgUi9QYXJlbnQgMiAwIFI+PgplbmRvYmoKCnhyZWYKMCA1CjAwMDAwMDAwMDAgMDAwMDEgZiAKMDAwMDAwMDAxNiAwMDAwMCBuIAowMDAwMDAwMDYyIDAwMDAwIG4gCjAwMDAwMDAxMTQgMDAwMDAgbiAKMDAwMDAwMDEzNSAwMDAwMCBuIAoKdHJhaWxlcgo8PC9TaXplIDUvUm9vdCAxIDAgUi9JRFs8MDUwQkMzQjk1NEMzOTdDM0FENDhDMkExMzE1RjE3NUU+PERBOEM3Nzk4MTk3NEM4MzA3MUU4RTlCNzBBNDQwQTMxPl0+PgpzdGFydHhyZWYKMjI2CiUlRU9GCg==");
    // std::vector<uint8_t> pdf = add_image_to_char(pdf_path, page_num, target_char, image_path, 200, 300, "JVBERi0xLjcKJcK1wrYKCjEgMCBvYmoKPDwvVHlwZS9DYXRhbG9nL1BhZ2VzIDIgMCBSPj4KZW5kb2JqCgoyIDAgb2JqCjw8L1R5cGUvUGFnZXMvQ291bnQgMS9LaWRzWzQgMCBSXT4+CmVuZG9iagoKMyAwIG9iago8PD4+CmVuZG9iagoKNCAwIG9iago8PC9UeXBlL1BhZ2UvTWVkaWFCb3hbMCAwIDU5NSA4NDJdL1JvdGF0ZSAwL1Jlc291cmNlcyAzIDAgUi9QYXJlbnQgMiAwIFI+PgplbmRvYmoKCnhyZWYKMCA1CjAwMDAwMDAwMDAgMDAwMDEgZiAKMDAwMDAwMDAxNiAwMDAwMCBuIAowMDAwMDAwMDYyIDAwMDAwIG4gCjAwMDAwMDAxMTQgMDAwMDAgbiAKMDAwMDAwMDEzNSAwMDAwMCBuIAoKdHJhaWxlcgo8PC9TaXplIDUvUm9vdCAxIDAgUi9JRFs8MDUwQkMzQjk1NEMzOTdDM0FENDhDMkExMzE1RjE3NUU+PERBOEM3Nzk4MTk3NEM4MzA3MUU4RTlCNzBBNDQwQTMxPl0+PgpzdGFydHhyZWYKMjI2CiUlRU9GCg==");
    // std::vector<uint8_t> pdf = add_qr_to_char(pdf_path, page_num, target_char, url, width, height, "JVBERi0xLjcKJcK1wrYKCjEgMCBvYmoKPDwvVHlwZS9DYXRhbG9nL1BhZ2VzIDIgMCBSPj4KZW5kb2JqCgoyIDAgb2JqCjw8L1R5cGUvUGFnZXMvQ291bnQgMS9LaWRzWzQgMCBSXT4+CmVuZG9iagoKMyAwIG9iago8PD4+CmVuZG9iagoKNCAwIG9iago8PC9UeXBlL1BhZ2UvTWVkaWFCb3hbMCAwIDU5NSA4NDJdL1JvdGF0ZSAwL1Jlc291cmNlcyAzIDAgUi9QYXJlbnQgMiAwIFI+PgplbmRvYmoKCnhyZWYKMCA1CjAwMDAwMDAwMDAgMDAwMDEgZiAKMDAwMDAwMDAxNiAwMDAwMCBuIAowMDAwMDAwMDYyIDAwMDAwIG4gCjAwMDAwMDAxMTQgMDAwMDAgbiAKMDAwMDAwMDEzNSAwMDAwMCBuIAoKdHJhaWxlcgo8PC9TaXplIDUvUm9vdCAxIDAgUi9JRFs8MDUwQkMzQjk1NEMzOTdDM0FENDhDMkExMzE1RjE3NUU+PERBOEM3Nzk4MTk3NEM4MzA3MUU4RTlCNzBBNDQwQTMxPl0+PgpzdGFydHhyZWYKMjI2CiUlRU9GCg==");
    

    // std::vector<uint8_t> obj = getImageObject(pdf, width, height, start_object_number);
    
    return obj;
}

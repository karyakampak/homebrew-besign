#include "../header/add_placeholder.h"
#include "../header/calculate_hash.h"
#include "../header/sign_with_p12.h"
#include "../header/sign_bsre.h"
#include "../header/save_pdf.h"
#include "../header/get_ocsp_response.h"
#include "../header/add_ocsp_dict.h"
#include "../header/get_added_index.h"
#include "../header/add_ocsp_trailer.h"
#include "../header/addons.h"
#include "../header/place_signature.h"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <sstream>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>

using json = nlohmann::json;


std::string base64_encodexxx(const std::string& binaryData) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    // Create a base64 filter/sink
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    // Create a memory buffer source
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    // Write binary data to the BIO
    BIO_write(bio, binaryData.c_str(), binaryData.length());
    BIO_flush(bio);

    // Retrieve the encoded data from the BIO
    BIO_get_mem_ptr(bio, &bufferPtr);
    std::string encodedData(bufferPtr->data, bufferPtr->length);

    // Cleanup
    BIO_free_all(bio);

    return encodedData;
}

extern "C" {

    char* placeHolder(const char* pdf_path, const char* image_path, int page, int visibility, float x, float y, float width, float height, int isSeal){

        AddPlaceHolder addPlaceHolder;
        std::unordered_map<std::string, std::string> placeholderData = addPlaceHolder.addPlaceholder(pdf_path, image_path, page, visibility, x, y, width, height, isSeal);
        std::string placedHolder = placeholderData.at("placedHolder");
        std::string catalogDict = placeholderData.at("catalog_dict");


        CalculateHash clhash;
        std::vector<uint8_t> holder = std::vector<uint8_t>(placedHolder.begin(), placedHolder.end());
        std::unordered_map<std::string, std::string> signed_pdf = clhash.calculate_hash(holder);

        Addons adns;
    
        // Allocate a buffer large enough to hold the string
        std::string pdfs = adns.vectorToHexString(adns.stringToVector(signed_pdf.at("pdf")));
        std::string catalog = adns.vectorToHexString(adns.stringToVector(catalogDict));


        json j_obj;
        j_obj["pdfs"] = pdfs;
        j_obj["hash"] = signed_pdf.at("hash");
        j_obj["byterange0"] = signed_pdf.at("byterange0");
        j_obj["byterange1"] = signed_pdf.at("byterange1");
        j_obj["byterange2"] = signed_pdf.at("byterange2");
        j_obj["byterange3"] = signed_pdf.at("byterange3");
        j_obj["catalog"] = catalog;


        // Convert json object to string
        std::string json_str = j_obj.dump();

        char* cstr = new char[json_str.size() + 1];
        std::strcpy(cstr, json_str.c_str());

        // std::cout << signed_pdf.at("pdf") << std::endl;
        // std::cout << encodedData << std::endl;
        
        // SavePdf svpdf;
        // svpdf.savePDF(adns.stringToVector(signed_pdf.at("pdf")), "./xxx.pdf");
        

        return cstr;
    }

    char* place(const char* pdf_char, const char* signature_char, const char* catalog_char, const char* byteRange0_char, const char* byteRange1_char, const char* byteRange2_char, const char* byteRange3_char, const char* outputPath) {
        std::string pdf(pdf_char);
        std::string signature(signature_char);
        std::string catalog(catalog_char);
        int byteRange0 = std::stoi(byteRange0_char);
        int byteRange1 = std::stoi(byteRange1_char);
        int byteRange2 = std::stoi(byteRange2_char);
        int byteRange3 = std::stoi(byteRange3_char);

        Addons adns;

        std::vector<uint8_t> pkcs7Binary = adns.base64_decode(signature);
        std::string signature_hex = adns.vectorToHexString(pkcs7Binary);

        std::vector<uint8_t> holder_vec = adns.hexStringToBytes(pdf);

        PlaceSignature plcSig;
        std::vector<uint8_t> pdf_vec = plcSig.place_signature(holder_vec, signature_hex, byteRange0, byteRange1, byteRange2, byteRange3);
        std::string pdf_str = adns.vectorToHexString(pdf_vec);

        char* cstr = new char[pdf_str.size() + 1];
        std::strcpy(cstr, pdf_str.c_str());
        
        SavePdf svpdf;
        svpdf.savePDF(pdf_vec, outputPath);
        

        return cstr;
    }
    
    // Function to free the allocated memory
    void free_string(const char* buffer) {
        free((void*)buffer);
    }

    void signWithP12(const char* pdf_path, const char* image_path, const char* output_path, int page, int visibility, float x, float y, float width, float height, const char* p12Path, const char* passphrase, int isSeal){

        AddPlaceHolder addPlaceHolder;
        std::unordered_map<std::string, std::string> placeholderData = addPlaceHolder.addPlaceholder(pdf_path, image_path, page, visibility, x, y, width, height, isSeal);
        std::string placedHolder = placeholderData.at("placedHolder");
        std::string catalogDict = placeholderData.at("catalog_dict");

        SignWithP12 snp12;
        std::vector<uint8_t> holder = std::vector<uint8_t>(placedHolder.begin(), placedHolder.end());
        std::vector<uint8_t> signed_pdf = snp12.sign(holder, p12Path, passphrase);

        SavePdf svpdf;
        svpdf.savePDF(signed_pdf, output_path);
    }

    void signBSrE(const char* pdf_path, const char* image_path, const char* output_path, int page, int visibility, float x, float y, float width, float height, const char* nik, const char* passphrase, const char* id, const char* secret, int isLTV, int isSeal){

        AddPlaceHolder addPlaceHolder;
        std::unordered_map<std::string, std::string> placeholderData = addPlaceHolder.addPlaceholder(pdf_path, image_path, page, visibility, x, y, width, height, isSeal);
        std::string placedHolder = placeholderData.at("placedHolder");
        std::string catalogDict = placeholderData.at("catalog_dict");


        SignBSrE sBSrEnp12;
        std::vector<uint8_t> holder = std::vector<uint8_t>(placedHolder.begin(), placedHolder.end());
        std::unordered_map<std::string, std::vector<uint8_t> > signedData = sBSrEnp12.sign(holder, nik, passphrase, id, secret);
        std::vector<uint8_t> signed_pdf = signedData.at("signed");
        std::vector<uint8_t> stack_cert = signedData.at("stack_cert");

        if(isLTV == 0){
            SavePdf svpdf;
            svpdf.savePDF(signed_pdf, output_path);
        } else {
            GetOCSPResponse getOCSP;
            std::vector<uint8_t> ocsp = getOCSP.get_ocsp(stack_cert);

            // std::string ocspResponse(ocsp.begin(), ocsp.end());
            // std::cout << ocspResponse << std::endl;

            AddOCSPDict adocspdict;
            std::vector<uint8_t> catalog_dict = std::vector<uint8_t>(catalogDict.begin(), catalogDict.end());
            std::unordered_map<std::string, std::vector<uint8_t> > ocspDictAdded = adocspdict.add_ocsp_dict(placeholderData.at("pdf_component_size"), signedData, catalog_dict, ocsp);

            // std::string ocspResponse(ocspDictAdded.begin(), ocspDictAdded.end());
            // std::cout << ocspResponse << std::endl;
            GetAddedIndex getaddin;
            std::vector<size_t> added_index = getaddin.get_index_after_add_dict(ocspDictAdded.at("added_index"));
            // for (std::vector<size_t>::const_iterator it = added_index.begin(); it != added_index.end(); ++it) {
            //     std::cout << "New Object Index " << *it << std::endl;
            // }

            AddOCSPTrailer addOcspTrailer;
            std::vector<uint8_t> pdf_ltv_signature = addOcspTrailer.add_ocsp_trailer(signed_pdf, ocspDictAdded.at("signed_pdf"), added_index, placeholderData);
            
            SavePdf svpdf;
            svpdf.savePDF(pdf_ltv_signature, output_path);
        }
    }

}


int main(int argc, char* argv[]) {
    // Check if the number of arguments is correct
    // if (argc < 5 || argc > 13) {
    //     std::cerr << "Usage: " << argv[0] << " <pdf_path> [<image_path>] <output_path> [<page>] [<visibility>] [<x>] [<y>] [<width>] [<height>] <p12Path> <passphrase>\n";
    //     return 1;
    // }

    // Extract mandatory parameters from command line arguments
    char* pdf_path;
    char* image_path;
    char* output_path;
    int page = 1;
    int visibility = 0;
    float x = 100.0;
    float y = 100.0;
    float width = 128.0;
    float height = 45.374;
    char* p12Path;
    char* nik;
    char* passphrase;
    char* id;
    char* secret;
    int isSeal = 0;

    const char* type = argv[1];
    std::string typeOption = "--type=";
    std::string inputPDFOption = "--input-pdf=";
    std::string outputPDFOption = "--output-pdf=";
    std::string inputImageOption = "--input-image=";
    std::string visibilityOption = "--visibility=";
    std::string p12Option = "--p12=";
    std::string nikOption = "--nik=";
    std::string passphraseOption = "--passphrase=";
    std::string idOption = "--id=";
    std::string secretOption = "--secret=";
    std::string tipe;


    std::string constId = "L3IqxuKvTRHKUvHim2YPXBEj7U0a";
    id = new char[constId.length() + 1]; // +1 for null terminator
    std::strcpy(id, constId.c_str());

    std::string constSecret = "oAH8f9DGsWd4XJLZZiKGhlaRk2ca";
    secret = new char[constSecret.length() + 1]; // +1 for null terminator
    std::strcpy(secret, constSecret.c_str());

    // Iterate through command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], typeOption.c_str(), typeOption.length()) == 0) {
            std::string typeVariable = argv[i] + typeOption.length();
            tipe = typeVariable;
        }
        if (strncmp(argv[i], inputPDFOption.c_str(), inputPDFOption.length()) == 0) {
            std::string inputPDFVariable = argv[i] + inputPDFOption.length();
            pdf_path = new char[inputPDFVariable.length() + 1]; // +1 for null terminator
            std::strcpy(pdf_path, inputPDFVariable.c_str());
        }
        if (strncmp(argv[i], outputPDFOption.c_str(), outputPDFOption.length()) == 0) {
            std::string outputPDFVariable = argv[i] + outputPDFOption.length();
            output_path = new char[outputPDFVariable.length() + 1]; // +1 for null terminator
            std::strcpy(output_path, outputPDFVariable.c_str());
        }
        if (strncmp(argv[i], p12Option.c_str(), p12Option.length()) == 0) {
            std::string p12Variable = argv[i] + p12Option.length();
            p12Path = new char[p12Variable.length() + 1]; // +1 for null terminator
            std::strcpy(p12Path, p12Variable.c_str());
        }
        if (strncmp(argv[i], passphraseOption.c_str(), passphraseOption.length()) == 0) {
            std::string passphraseVariable = argv[i] + passphraseOption.length();
            passphrase = new char[passphraseVariable.length() + 1]; // +1 for null terminator
            std::strcpy(passphrase, passphraseVariable.c_str());
        }


        if (strncmp(argv[i], nikOption.c_str(), nikOption.length()) == 0) {
            std::string nikVariable = argv[i] + nikOption.length();
            nik = new char[nikVariable.length() + 1]; // +1 for null terminator
            std::strcpy(nik, nikVariable.c_str());
        }
        if (strncmp(argv[i], nikOption.c_str(), nikOption.length()) == 0) {
            std::string nikVariable = argv[i] + nikOption.length();
            nik = new char[nikVariable.length() + 1]; // +1 for null terminator
            std::strcpy(nik, nikVariable.c_str());
        }
        if (strncmp(argv[i], visibilityOption.c_str(), visibilityOption.length()) == 0) {
            std::string visibilityVariable = argv[i] + visibilityOption.length();
            if (visibilityVariable == "visible")
            {
                for (int j = 1; j < argc; ++j) {
                    if (strncmp(argv[j], inputImageOption.c_str(), inputImageOption.length()) == 0) {
                        std::string inputImageVariable = argv[j] + inputImageOption.length();
                        image_path = new char[inputImageVariable.length() + 1]; // +1 for null terminator
                        std::strcpy(image_path, inputImageVariable.c_str());
                    }
                }
                if(image_path == nullptr){
                    std::cerr << "Image path must be added" << std::endl;
                    return 1;
                }
                visibility = 1;
            }
        }
    }


    if (tipe == "p12")
    {
        std::cout << "Tipe tandatangan P12 " << std::endl;

        AddPlaceHolder addPlaceHolder;
        std::unordered_map<std::string, std::string> placeholderData = addPlaceHolder.addPlaceholder(pdf_path, image_path, page, visibility, x, y, width, height, isSeal);
        // std::cout << pdf_path << std::endl;
        std::string placedHolder = placeholderData.at("placedHolder");
        std::string catalogDict = placeholderData.at("catalog_dict");

        SignWithP12 snp12;
        std::vector<uint8_t> holder = std::vector<uint8_t>(placedHolder.begin(), placedHolder.end());
        std::vector<uint8_t> signed_pdf = snp12.sign(holder, p12Path, passphrase);

        SavePdf svpdf;
        svpdf.savePDF(signed_pdf, output_path);

        return 0;
    }

    if (tipe == "bsre")
    {
        std::cout << "Tipe tandatangan BSrE " << std::endl;

        AddPlaceHolder addPlaceHolder;
        std::unordered_map<std::string, std::string> placeholderData = addPlaceHolder.addPlaceholder(pdf_path, image_path, page, visibility, x, y, width, height, isSeal);
        std::string placedHolder = placeholderData.at("placedHolder");
        std::string catalogDict = placeholderData.at("catalog_dict");


        SignBSrE sBSrEnp12;
        std::vector<uint8_t> holder = std::vector<uint8_t>(placedHolder.begin(), placedHolder.end());
        std::unordered_map<std::string, std::vector<uint8_t> > signedData = sBSrEnp12.sign(holder, nik, passphrase, id, secret);
        std::vector<uint8_t> signed_pdf = signedData.at("signed");
        std::vector<uint8_t> stack_cert = signedData.at("stack_cert");
        SavePdf svpdf;
        svpdf.savePDF(signed_pdf, output_path);

        return 0;
    }
    
    if (tipe == "bsre-ocsp")
    {
        std::cout << "Tipe tandatangan BSrE with OCSP " << std::endl;

        AddPlaceHolder addPlaceHolder;
        std::unordered_map<std::string, std::string> placeholderData = addPlaceHolder.addPlaceholder(pdf_path, image_path, page, visibility, x, y, width, height, isSeal);
        std::string placedHolder = placeholderData.at("placedHolder");
        std::string catalogDict = placeholderData.at("catalog_dict");


        SignBSrE sBSrEnp12;
        std::vector<uint8_t> holder = std::vector<uint8_t>(placedHolder.begin(), placedHolder.end());
        std::unordered_map<std::string, std::vector<uint8_t> > signedData = sBSrEnp12.sign(holder, nik, passphrase, id, secret);
        std::vector<uint8_t> signed_pdf = signedData.at("signed");
        std::vector<uint8_t> stack_cert = signedData.at("stack_cert");

            GetOCSPResponse getOCSP;
            std::vector<uint8_t> ocsp = getOCSP.get_ocsp(stack_cert);

            AddOCSPDict adocspdict;
            std::vector<uint8_t> catalog_dict = std::vector<uint8_t>(catalogDict.begin(), catalogDict.end());
            std::unordered_map<std::string, std::vector<uint8_t> > ocspDictAdded = adocspdict.add_ocsp_dict(placeholderData.at("pdf_component_size"), signedData, catalog_dict, ocsp);

            
            GetAddedIndex getaddin;
            std::vector<size_t> added_index = getaddin.get_index_after_add_dict(ocspDictAdded.at("added_index"));


            AddOCSPTrailer addOcspTrailer;
            std::vector<uint8_t> pdf_ltv_signature = addOcspTrailer.add_ocsp_trailer(signed_pdf, ocspDictAdded.at("signed_pdf"), added_index, placeholderData);

        SavePdf svpdf;
        svpdf.savePDF(pdf_ltv_signature, output_path);
        return 0;
    }

    std::cerr << "Type of signature must be definition" << std::endl;
    return 1;
}
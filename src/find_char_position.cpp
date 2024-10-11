#include "../header/find_char_position.h"
#include <iostream>
#include <poppler-document.h>
#include <poppler-page.h>
#include <poppler-page-renderer.h>
#include <vector>
#include <map>

FindCharPosition::FindCharPosition() {
    // Initialize private member variables or perform any necessary setup
}

std::vector<std::map<std::string, double>> FindCharPosition::findCharPosition(const std::string& pdfPath, char character, int pageNumber) {

    // Open the PDF document
    auto document = poppler::document::load_from_file(pdfPath);
    if (!document) {
        throw std::runtime_error("Failed to open PDF file.");
    }

    std::vector<std::map<std::string, double>> positions;

    // Get the total number of pages
    int totalPages = document->pages();
    if (pageNumber != -1 && (pageNumber < 1 || pageNumber > totalPages)) {
        throw std::out_of_range("Invalid page number: " + std::to_string(pageNumber));
    }

    // Define the range of pages to iterate over
    int startPage = (pageNumber != -1) ? pageNumber - 1 : 0; // 0-based index
    int endPage = (pageNumber != -1) ? pageNumber - 1 : totalPages - 1;

    // Iterate through the specified pages
    for (int pageNum = startPage; pageNum <= endPage; ++pageNum) {
        std::unique_ptr<poppler::page> page(document->create_page(pageNum));
        if (!page) {
            throw std::runtime_error("Failed to load page.");
        }

        // Get the page height for adjusting y-axis
        double pageHeight = page->page_rect().height();
        double pageWidth = page->page_rect().width();

        std::cout << "PageWidth : " << pageWidth << " PageHeight : " << pageHeight << std::endl;

        // Extract text from the page
        std::vector<poppler::text_box> textBoxes = page->text_list();

        for (const auto& textBox : textBoxes) {
            std::vector<char> utf8_text = textBox.text().to_utf8();
            std::string text(utf8_text.begin(), utf8_text.end()); // Convert vector<char> to std::string
            poppler::rectf bbox = textBox.bbox(); // Get bounding box of the text

            double left = bbox.x();
            double top = bbox.y();
            double width = bbox.width();
            double height = bbox.height();

            // Find the character within the text and calculate its position
            for (size_t i = 0; i < text.length(); ++i) {
                if (text[i] == character) {
                    // Calculate character position within the word bounds
                    double charWidth = width / text.length();
                    double xPosition = left + (i * charWidth);
                    double yPosition = pageHeight - top; // Invert y-axis

                    // Store the real position of the character
                    positions.push_back({
                        {"x", xPosition},
                        {"y", yPosition},
                        {"width", charWidth},
                        {"height", height}
                    });
                }
            }
        }
    }

    return positions;
}
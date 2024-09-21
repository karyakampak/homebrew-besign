# addimage.pyx
import base64
import fitz  # PyMuPDF
import pikepdf
from io import BytesIO
import subprocess
import datetime
import os

def decrypt_pdf_from_bytes(pdf_content_bytes, password):
    # Convert bytes to a BytesIO stream
    pdf_stream = BytesIO(pdf_content_bytes)
    
    # Open the encrypted PDF and decrypt it
    with pikepdf.open(pdf_stream, password=password) as pdf:
        # Create another BytesIO buffer for the decrypted content
        decrypted_pdf_bytes = BytesIO()
        
        # Save the decrypted PDF to the buffer
        pdf.save(decrypted_pdf_bytes)
        
        # Return the decrypted PDF content as bytes
        return decrypted_pdf_bytes.getvalue()

def find_char_position(pdf_data, page_num, target_char):
    # Convert the raw PDF data (bytes) to a BytesIO object
    pdf_stream = BytesIO(pdf_data)
    
    # Open the PDF from the BytesIO stream
    pdf_document = fitz.open(stream=pdf_stream, filetype="pdf")
    page = pdf_document[page_num]

    # Get the page height (to transform the coordinates)
    page_height = page.rect.height

    # Search for the target character
    text_instances = page.search_for(target_char)

    if not text_instances:
        print(f"Character '{target_char}' not found.")
        return None

    # Get the first occurrence (or modify for multiple instances)
    rect = text_instances[0]

    # Transform the coordinates (0,0) -> bottom-left
    x0, y0 = rect.x0, page_height - rect.y1  # bottom-left of the character
    x1, y1 = rect.x1, page_height - rect.y0  # top-right of the character

    # Calculate the center of the character
    center_x = (x0 + x1) / 2
    center_y = (y0 + y1) / 2

    # print(f"Center of character '{target_char}': (x={center_x}, y={center_y})")

    return center_x, center_y
# addimage.pyx
import base64
import fitz  # PyMuPDF
from io import BytesIO
import qrcode
from PIL import Image
import subprocess
import datetime
import os

def get_temp_filename(prefix, suffix):
    """Generate a unique temporary file name with a timestamp."""
    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    return f"/tmp/{prefix}_{timestamp}.{suffix}"

def convert_pdf_to_version_1_3(pdf_stream):

    try:
        # Open the PDF using PyMuPDF to verify it's a valid PDF
        pdf_document = fitz.open(stream=pdf_stream, filetype="pdf")
    except Exception as e:
        print(f"Error opening PDF: {e}")
        return None

    # Save the PDF to a temporary file
    input_pdf_path = get_temp_filename("input", "pdf")
    pdf_document.save(input_pdf_path)

    # Create output path for the PDF version 1.3
    output_pdf_path = get_temp_filename("output_1_3", "pdf")

    # Use Ghostscript to convert the PDF to version 1.3
    try:
        subprocess.run([
            "gs", 
            "-dCompatibilityLevel=1.3",  # Specify PDF version 1.3
            "-dNOPAUSE", 
            "-dBATCH", 
            "-sDEVICE=pdfwrite", 
            f"-sOutputFile={output_pdf_path}", 
            input_pdf_path
        ], check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except subprocess.CalledProcessError as e:
        print(f"Error running Ghostscript: {e}")
        os.remove(input_pdf_path)
        return None

    # Read the output PDF 1.3 file into BytesIO
    with open(output_pdf_path, "rb") as f:
        pdf_1_3_bytes = BytesIO(f.read())

    # Clean up temporary files
    os.remove(input_pdf_path)
    os.remove(output_pdf_path)

    # Return the PDF 1.3 data as bytes
    return pdf_1_3_bytes

def decrypt_pdf_to_bytes(pdf_data, password):
    # Convert raw PDF data (bytes) to a BytesIO object
    pdf_stream = BytesIO(pdf_data)

    # Try to open the encrypted PDF from the BytesIO stream
    try:
        pdf_document = fitz.open(stream=pdf_stream, filetype="pdf")
    except Exception as e:
        print(f"Error opening PDF: {e}")
        return None

    # Check if the PDF is encrypted
    if pdf_document.is_encrypted:
        # Try to authenticate using the provided password
        if pdf_document.authenticate(password):
            print("PDF successfully decrypted.")

            # Create a BytesIO object to store the decrypted PDF
            decrypted_pdf_bytes = BytesIO()

            # Write the decrypted PDF into the BytesIO object
            pdf_document.save(decrypted_pdf_bytes)

            # Move the cursor back to the beginning of the BytesIO object
            decrypted_pdf_bytes.seek(0)




            pdf_1_3 = convert_pdf_to_version_1_3(decrypted_pdf_bytes)



            # Return the BytesIO object containing the decrypted PDF data
            return pdf_1_3.getvalue()  # Return the bytes
        else:
            print("Incorrect password. Could not decrypt the PDF.")
            return None
    else:
        print("The PDF is not encrypted.")
        return None

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

def process_image(binary_data):
    # Create a BytesIO object from the binary data
    img_stream = BytesIO(binary_data)

    # Open the image file
    img = Image.open(img_stream)

    # Check if the image has transparency (has an alpha channel)
    if img.mode in ('RGBA', 'LA') or (img.mode == 'P' and 'transparency' in img.info):
        # Create a new image with white background
        white_bg = Image.new("RGB", img.size, (255, 255, 255))
        # Composite the image with white background
        img = Image.alpha_composite(white_bg.convert("RGBA"), img.convert("RGBA")).convert("RGB")
    else:
        # If no alpha channel, just convert to RGB
        img = img.convert('RGB')

    # Rotate the image by 180 degrees
    # img = img.rotate(180, expand=True)

    # Create a BytesIO object for the image stream
    img_stream = BytesIO()
    img.save(img_stream, format='JPEG')  # Save the image in JPEG format to a byte stream
    img_stream.seek(0)  # Move to the beginning of the byte stream

    # Encode image data to Base64
    encoded_string = base64.b64encode(img_stream.read()).decode('utf-8')
    
    return encoded_string
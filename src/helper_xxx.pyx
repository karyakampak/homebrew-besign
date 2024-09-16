# addimage.pyx
import base64
import fitz  # PyMuPDF
from io import BytesIO
import qrcode
from PIL import Image

def process_image(image_path):
    # Open the image file
    img = Image.open(image_path)

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
    img = img.rotate(180, expand=True)

    # Create a BytesIO object for the image stream
    img_stream = BytesIO()
    img.save(img_stream, format='JPEG')  # Save the image in JPEG format to a byte stream
    img_stream.seek(0)  # Move to the beginning of the byte stream

    # Encode image data to Base64
    encoded_string = base64.b64encode(img_stream.read()).decode('utf-8')
    
    return encoded_string

def process_qr(url):
    # Generate QR code
    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_L,
        box_size=10,
        border=4,
    )
    qr.add_data(url)
    qr.make(fit=True)
    
    # Create an image from the QR code instance
    img = qr.make_image(fill='black', back_color='white')

    # save the QR code generated
    img.save('custom_qr.png')

    # Open the image file
    img = Image.open('custom_qr.png')

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
    img = img.rotate(180, expand=True)

    # Create a BytesIO object for the image stream
    img_stream = BytesIO()
    img.save(img_stream, format='JPEG')  # Save the image in JPEG format to a byte stream
    img_stream.seek(0)  # Move to the beginning of the byte stream

    # Encode image data to Base64
    encoded_string = base64.b64encode(img_stream.read()).decode('utf-8')
    
    return encoded_string

def add_image_to_pdf(image_path, x, y_from_bottom, width, height, base64_pdf):
    # Decode the Base64 string
    pdf_data = base64.b64decode(base64_pdf)
    
    # Use BytesIO to handle the PDF data in-memory
    pdf_stream = BytesIO(pdf_data)
    
    # Open the PDF from the BytesIO stream
    pdf_document = fitz.open(stream=pdf_stream, filetype="pdf")
    
    # Select the first page (0-indexed), or adjust for a specific page
    page = pdf_document[0]

    # Get the height of the page
    page_height = page.rect.height

    # Adjust the y coordinate (convert from bottom-left to top-left origin)
    y = page_height - y_from_bottom - height

    # Center the image by shifting the coordinates by half the width and height
    x_centered = x - (width / 2)
    y_centered = page_height - (y_from_bottom + (height / 2))

    # Insert the image on the page at the specified location with scalable size
    # image_rect = fitz.Rect(x, y, x + width, y + height)  # Create a rectangle for image size
    image_rect = fitz.Rect(x_centered, y_centered, x_centered + width, y_centered + height)  # Create a rectangle for image size
    page.insert_image(image_rect, filename=image_path)
    
    # Save the updated PDF to a BytesIO stream
    pdf_output_stream = BytesIO()
    pdf_document.save(pdf_output_stream)
    pdf_output_stream.seek(0)  # Rewind the stream to the beginning

    # Encode the PDF to Base64
    pdf_base64 = base64.b64encode(pdf_output_stream.read()).decode('utf-8')

    # Save the updated PDF
    # pdf_document.save("placed.pdf")
    pdf_document.close()

    return pdf_base64



def add_image_to_char(pdf_path, page_num, target_char, image_path, width, height, base64_pdf):
    # Open the PDF
    pdf_document = fitz.open(pdf_path)
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

    # Decode the Base64 string
    pdf_data = base64.b64decode(base64_pdf)
    
    # Use BytesIO to handle the PDF data in-memory
    pdf_stream = BytesIO(pdf_data)
    
    # Open the PDF from the BytesIO stream
    pdf_document = fitz.open(stream=pdf_stream, filetype="pdf")
    
    # Select the first page (0-indexed), or adjust for a specific page
    page = pdf_document[0]

    # Get the height of the page
    page_height = page.rect.height

    # Adjust the y coordinate (convert from bottom-left to top-left origin)
    y = page_height - center_y - height

    # Center the image by shifting the coordinates by half the width and height
    x_centered = center_x - (width / 2)
    y_centered = page_height - (center_y + (height / 2))

    # Insert the image on the page at the specified location with scalable size
    # image_rect = fitz.Rect(x, y, x + width, y + height)  # Create a rectangle for image size
    image_rect = fitz.Rect(x_centered, y_centered, x_centered + width, y_centered + height)  # Create a rectangle for image size
    page.insert_image(image_rect, filename=image_path)
    
    # Save the updated PDF to a BytesIO stream
    pdf_output_stream = BytesIO()
    pdf_document.save(pdf_output_stream)
    pdf_output_stream.seek(0)  # Rewind the stream to the beginning

    # Encode the PDF to Base64
    pdf_base64 = base64.b64encode(pdf_output_stream.read()).decode('utf-8')

    # # Save the updated PDF
    # pdf_document.save("placed.pdf")
    pdf_document.close()

    return pdf_base64



def add_qr_to_char(pdf_path, page_num, target_char, url, width, height, base64_pdf):
    # Open the PDF
    pdf_document = fitz.open(pdf_path)
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

    # Generate QR code image from URL
    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_L,
        box_size=10,
        border=4,
    )
    qr.add_data(url)
    qr.make(fit=True)

    img = qr.make_image(fill='black', back_color='white')
    
    # Save QR code image to a BytesIO object
    img_stream = BytesIO()
    img.save(img_stream, format='PNG')
    img_stream.seek(0)  # Rewind the stream to the beginning

    # Decode the Base64 string
    try:
        pdf_data = base64.b64decode(base64_pdf)
    except (base64.binascii.Error, ValueError) as e:
        raise ValueError("Failed to decode Base64 PDF") from e

    # Decode the Base64 string
    # pdf_data = base64.b64decode(base64_pdf)
    
    # Use BytesIO to handle the PDF data in-memory
    pdf_stream = BytesIO(pdf_data)
    
    # Open the PDF from the BytesIO stream
    pdf_document = fitz.open(stream=pdf_stream, filetype="pdf")
    
    # Select the first page (0-indexed), or adjust for a specific page
    page = pdf_document[0]

    # Get the height of the page
    page_height = page.rect.height

    # Adjust the y coordinate (convert from bottom-left to top-left origin)
    y = page_height - center_y - height

    # Center the image by shifting the coordinates by half the width and height
    x_centered = center_x - (width / 2)
    y_centered = page_height - (center_y + (height / 2))

    # Insert the image on the page at the specified location with scalable size
    # image_rect = fitz.Rect(x, y, x + width, y + height)  # Create a rectangle for image size
    image_rect = fitz.Rect(x_centered, y_centered, x_centered + width, y_centered + height)  # Create a rectangle for image size
    page.insert_image(image_rect, stream=img_stream)
    
    # Save the updated PDF to a BytesIO stream
    pdf_output_stream = BytesIO()
    pdf_document.save(pdf_output_stream)
    pdf_output_stream.seek(0)  # Rewind the stream to the beginning

    # Encode the PDF to Base64
    pdf_base64 = base64.b64encode(pdf_output_stream.read()).decode('utf-8')

    # # Save the updated PDF
    # pdf_document.save("placed.pdf")
    pdf_document.close()

    return pdf_base64
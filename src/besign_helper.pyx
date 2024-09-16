# addimage.pyx
import base64
import fitz  # PyMuPDF
from io import BytesIO
import qrcode
from PIL import Image

def find_char_position(pdf_path, page_num, target_char):
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
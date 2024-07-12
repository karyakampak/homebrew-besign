cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# Set the target architectures for macOS
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(DKSIGN)

# Define the shared library
add_library(dksign_library SHARED
    src/add_ocsp_trailer.cpp
    src/add_ocsp_dict.cpp
    src/get_ocsp_response.cpp
    src/sign_bsre.cpp
    src/sign_with_p12.cpp
    src/cms.cpp
    src/detached_cms.cpp
    src/place_signature.cpp
    src/add_placeholder.cpp
    src/save_pdf.cpp
    src/add_trailer.cpp
    src/get_added_index.cpp
    src/calculate_hash.cpp
    src/add_signature_dict.cpp
    src/add_seal_dict.cpp
    src/check_signature.cpp
    src/get_page_reference.cpp
    src/get_pdf_component.cpp
    src/open_file.cpp
    src/addons.cpp
    src/main.cpp
)

add_executable(dksign_executable
    src/add_ocsp_trailer.cpp
    src/add_ocsp_dict.cpp
    src/get_ocsp_response.cpp
    src/sign_bsre.cpp
    src/sign_with_p12.cpp
    src/cms.cpp
    src/detached_cms.cpp
    src/place_signature.cpp
    src/add_placeholder.cpp
    src/save_pdf.cpp
    src/add_trailer.cpp
    src/get_added_index.cpp
    src/calculate_hash.cpp
    src/add_signature_dict.cpp
    src/add_seal_dict.cpp
    src/check_signature.cpp
    src/get_page_reference.cpp
    src/get_pdf_component.cpp
    src/open_file.cpp
    src/addons.cpp
    src/main.cpp
)


find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(dksign_executable ${OPENSSL_LIBRARIES})
target_link_libraries(dksign_library ${OPENSSL_LIBRARIES})

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(dksign_executable ${CURL_LIBRARIES})
target_link_libraries(dksign_library ${CURL_LIBRARIES})

find_package(nlohmann_json 3.2.0 REQUIRED)
include_directories(${nlohmann_json_INCLUDE_DIRS})
target_link_libraries(dksign_executable nlohmann_json::nlohmann_json)
target_link_libraries(dksign_library nlohmann_json::nlohmann_json)


find_package(ZLIB REQUIRED)
target_link_libraries(dksign_executable ZLIB::ZLIB)
target_link_libraries(dksign_library ZLIB::ZLIB)

find_package(PNG REQUIRED)
target_link_libraries(dksign_executable PNG::PNG)
target_link_libraries(dksign_library PNG::PNG)

find_package(JPEG REQUIRED)
target_link_libraries(dksign_executable JPEG::JPEG)
target_link_libraries(dksign_library JPEG::JPEG)

# find_package(LibHaru REQUIRED)
# target_link_libraries(dksign_executable QPDF::QPDF)
# target_link_libraries(dksign_executable Podofo::Podofo)

# Try to find the UUID library
find_library(UUID_LIBRARY NAMES uuid)

# Check if the UUID library was found
if(UUID_LIBRARY)
    message(STATUS "UUID library found: ${UUID_LIBRARY}")
    # Add the UUID library to your target
    target_link_libraries(dksign_executable ${UUID_LIBRARY})
    target_link_libraries(dksign_library ${UUID_LIBRARY})
else()
    message(FATAL_ERROR "UUID library not found. Please make sure the library is installed.")
endif()

# Determine platform-specific include and lib directories
if(APPLE)
    set(INCLUDE_DIR "/usr/local/opt/dksign/include")
    set(LIB_DIR "/usr/local/opt/dksign/lib")
    set(BIN_DIR "/usr/local/opt/dksign/bin")
    set(OTHER_DIR "/usr/local/opt/dksign")
elseif(UNIX)
    set(INCLUDE_DIR "/usr/include/dksign")
    set(LIB_DIR "/usr/lib/dksign")
    set(BIN_DIR "/usr/bin/dksign")
    set(OTHER_DIR "/usr/include/dksign")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()


# Optionally, install the executable to the "bin" folder after building
install(TARGETS dksign_executable DESTINATION bin)

install(FILES
    build/bin/dksign
    DESTINATION ${BIN_DIR}
)

install(TARGETS dksign_library
    LIBRARY DESTINATION ${LIB_DIR}
)

install(FILES
    header/add_ocsp_trailer.h
    header/add_ocsp_dict.h
    header/get_ocsp_response.h
    header/sign_bsre.h
    header/sign_with_p12.h
    header/cms.h
    header/detached_cms.h
    header/place_signature.h
    header/add_placeholder.h
    header/save_pdf.h
    header/add_trailer.h
    header/get_added_index.h
    header/calculate_hash.h
    header/add_signature_dict.h
    header/add_seal_dict.h
    header/check_signature.h
    header/get_page_reference.h
    header/get_pdf_component.h
    header/open_file.h
    header/addons.h
    DESTINATION ${INCLUDE_DIR}
)

install(FILES
    LICENSE.txt
    README.md
    AUTHORS.md
    DESTINATION ${OTHER_DIR}
)

set_target_properties(dksign_library PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

set_target_properties(dksign_executable PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(dksign_library dksign_executable PROPERTIES OUTPUT_NAME dksign)

# Set compiler flags if needed
# target_compile_options(besign PRIVATE -Wall -Wextra)

# Set compiler features if needed
# target_compile_features(besign PUBLIC cxx_std_11)
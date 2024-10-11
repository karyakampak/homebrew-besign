cmake_minimum_required(VERSION 3.10)

# Set the toolchain file for Emscripten
if(EMSCRIPTEN)
    set(CMAKE_TOOLCHAIN_FILE "${Emscripten_DIR}/emscripten.cmake")
endif()

# Project setup
project(BESIGN)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Find the Poppler library
find_package(PkgConfig REQUIRED)
pkg_check_modules(POPPLER REQUIRED poppler-cpp)

# Include Poppler headers
include_directories(${POPPLER_INCLUDE_DIRS})

# Add the library search paths (manually specify directories if necessary)
link_directories(${POPPLER_LIBRARY_DIRS})

# Find Dependencies
find_package(OpenCV REQUIRED)
find_library(QRENCODE_LIB qrencode)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(ZLIB REQUIRED)

# Include directories for found dependencies
include_directories(${OpenCV_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIRS} ${nlohmann_json_INCLUDE_DIRS})

# Exclude libraries that are not compatible with WebAssembly
if(EMSCRIPTEN)
    # You might need to handle or stub out certain libraries like OpenSSL, CURL, etc.
    message(STATUS "Building for WebAssembly, some libraries may be omitted or modified.")
endif()

# Define source files
set(SOURCES
    src/add_ocsp_trailer.cpp
    src/add_ocsp_dict.cpp
    src/get_ocsp_response.cpp
    src/sign_bsre.cpp
    src/sign_with_p12.cpp
    src/sign.cpp
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
    src/find_char_position.cpp
    src/addons.cpp
    src/main.cpp
)

# Define the shared library, and executable
add_library(besign_library SHARED ${SOURCES})
add_executable(besign_executable ${SOURCES})

# Link libraries to targets
if(NOT EMSCRIPTEN)
    target_link_libraries(besign_executable ${POPPLER_LIBRARIES} ZLIB::ZLIB nlohmann_json::nlohmann_json ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} ${QRENCODE_LIB} ${OpenCV_LIBS})
    target_link_libraries(besign_library ${POPPLER_LIBRARIES} ZLIB::ZLIB nlohmann_json::nlohmann_json ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} ${QRENCODE_LIB} ${OpenCV_LIBS})
else()
    # For WebAssembly, you might want to link different libraries or handle them differently
    target_link_libraries(besign_executable -s EXPORTED_FUNCTIONS="['_main']")
    target_link_libraries(besign_library)
endif()

# Install setup
install(TARGETS besign_executable DESTINATION bin)
install(TARGETS besign_library LIBRARY DESTINATION lib)
install(FILES
    header/add_ocsp_trailer.h
    header/add_ocsp_dict.h
    header/get_ocsp_response.h
    header/sign_bsre.h
    header/sign_with_p12.h
    header/sign.h
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
    header/find_char_position.h
    header/addons.h
    DESTINATION include
)

# Define platform-specific include and library directories
if(APPLE)
    set(INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
    set(LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
    set(BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
else()
    set(INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
    set(LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
    set(BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
endif()

# Set output directories for the targets
set_target_properties(besign_library PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    INSTALL_RPATH_USE_LINK_PATH TRUE
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${POPPLER_LIBRARY_DIRS}"
)
set_target_properties(besign_executable PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    INSTALL_RPATH_USE_LINK_PATH TRUE
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${POPPLER_LIBRARY_DIRS}"
)
# Set output name
set_target_properties(besign_library besign_executable PROPERTIES OUTPUT_NAME besign)

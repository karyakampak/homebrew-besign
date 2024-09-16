cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# Set the target architectures for macOS
# set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
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
    src/visualization.cpp
    src/check_signature.cpp
    src/get_page_reference.cpp
    src/get_pdf_component.cpp
    src/open_file.cpp
    src/addons.cpp
    src/main.cpp
)

add_library(dksign_object OBJECT
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
    src/visualization.cpp
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
    src/visualization.cpp
    src/check_signature.cpp
    src/get_page_reference.cpp
    src/get_pdf_component.cpp
    src/open_file.cpp
    src/addons.cpp
    src/main.cpp
)



# Find Python
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
include_directories(${Python3_INCLUDE_DIRS})
target_link_libraries(dksign_executable ${Python3_LIBRARIES})
target_link_libraries(dksign_library ${Python3_LIBRARIES})
target_link_libraries(dksign_object ${Python3_LIBRARIES})



find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(dksign_executable ${OpenCV_LIBS})
target_link_libraries(dksign_library ${OpenCV_LIBS})
target_link_libraries(dksign_object ${OpenCV_LIBS})

find_library(QRENCODE_LIB qrencode)

if (QRENCODE_LIB)
    target_link_libraries(dksign_executable ${QRENCODE_LIB})
    target_link_libraries(dksign_library ${QRENCODE_LIB})
    target_link_libraries(dksign_object ${QRENCODE_LIB})
else()
    message(FATAL_ERROR "qrencode library not found")
endif()


find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(dksign_executable ${OPENSSL_LIBRARIES})
target_link_libraries(dksign_library ${OPENSSL_LIBRARIES})
target_link_libraries(dksign_object ${OPENSSL_LIBRARIES})

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(dksign_executable ${CURL_LIBRARIES})
target_link_libraries(dksign_library ${CURL_LIBRARIES})
target_link_libraries(dksign_object ${CURL_LIBRARIES})

find_package(nlohmann_json 3.2.0 REQUIRED)
include_directories(${nlohmann_json_INCLUDE_DIRS})
target_link_libraries(dksign_executable nlohmann_json::nlohmann_json)
target_link_libraries(dksign_library nlohmann_json::nlohmann_json)
target_link_libraries(dksign_object nlohmann_json::nlohmann_json)


# find_package(ZLIB REQUIRED)
# target_link_libraries(dksign_executable ZLIB::ZLIB)
# target_link_libraries(dksign_library ZLIB::ZLIB)
# target_link_libraries(dksign_object ZLIB::ZLIB)

# Try to find the UUID library
find_library(UUID_LIBRARY NAMES uuid)

# Check if the UUID library was found
if(UUID_LIBRARY)
    message(STATUS "UUID library found: ${UUID_LIBRARY}")
    # Add the UUID library to your target
    target_link_libraries(dksign_executable ${UUID_LIBRARY})
    target_link_libraries(dksign_library ${UUID_LIBRARY})
    target_link_libraries(dksign_object ${UUID_LIBRARY})
else()
    message(FATAL_ERROR "UUID library not found. Please make sure the library is installed.")
endif()

# Define custom build directory
set(CUSTOM_CYTHON_BUILD_DIR ${CMAKE_SOURCE_DIR}/cython_build)

# Add custom command to build the Cython extension
add_custom_target(
    cython_module ALL
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/src/setup.py build_ext --inplace
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building Cython extension"
    ENVIRONMENT "CYTHON_BUILD_DIR=${CUSTOM_CYTHON_BUILD_DIR}"
)

# Determine platform-specific include and lib directories
if(APPLE)
    set(INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
    set(LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
    set(BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
else()
    set(INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
    set(LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
    set(BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
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
    header/visualization.h
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
    DESTINATION ${INCLUDE_DIR}
)

set_target_properties(dksign_library PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

set_target_properties(dksign_object PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj
)

set_target_properties(dksign_executable PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(dksign_library dksign_object dksign_executable PROPERTIES OUTPUT_NAME dksign)



# Install all files from the cython directory to the destination directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/cython_build/
        DESTINATION ${LIB_DIR}
        FILES_MATCHING PATTERN "*")


install(CODE "
    # Check the current shell and set the appropriate shell config file
    if(\$ENV{SHELL} MATCHES \".*/zsh\")
        set(SHELL_RC \"~/.zshrc\")
    elseif(\$ENV{SHELL} MATCHES \".*/bash\")
        set(SHELL_RC \"~/.bashrc\")
    else()
        message(FATAL_ERROR \"Unsupported shell: \$ENV{SHELL}\")
    endif()

    # Define the export command to add PYTHONPATH
    set(EXPORT_COMMAND \"echo 'export PYTHONPATH=${LIB_DIR}:\\\$PYTHONPATH' >> \${SHELL_RC}\")
    
    # Execute the command to update the shell config
    execute_process(COMMAND /bin/sh -c \"\${EXPORT_COMMAND}\")

    # Source the updated shell config
    execute_process(COMMAND /bin/sh -c \"source \${SHELL_RC}\")
")

# Set compiler flags if needed
# target_compile_options(besign PRIVATE -Wall -Wextra)

# Set compiler features if needed
# target_compile_features(besign PUBLIC cxx_std_11)
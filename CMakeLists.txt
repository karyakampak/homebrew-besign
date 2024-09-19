cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# Set the target architectures for macOS
# set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(BESIGN)

# Define the shared library
add_library(besign_library SHARED
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
    src/visualization.cpp
    src/check_signature.cpp
    src/get_page_reference.cpp
    src/get_pdf_component.cpp
    src/open_file.cpp
    src/addons.cpp
    src/main.cpp
)

add_library(besign_object OBJECT
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
    src/visualization.cpp
    src/check_signature.cpp
    src/get_page_reference.cpp
    src/get_pdf_component.cpp
    src/open_file.cpp
    src/addons.cpp
    src/main.cpp
)

add_executable(besign_executable
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



find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_library(QRENCODE_LIB qrencode)


find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

find_package(nlohmann_json 3.2.0 REQUIRED)
include_directories(${nlohmann_json_INCLUDE_DIRS})


# # Specify the path to the cmake modules if not in the default directory
# set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
# # Find MuPDF
# find_package(MuPDF REQUIRED)
# include_directories(${MUPDF_INCLUDE_DIRS})

# Try to find the UUID library
find_library(UUID_LIBRARY NAMES uuid)


target_link_libraries(besign_executable ${UUID_LIBRARY} nlohmann_json::nlohmann_json ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} ${QRENCODE_LIB} ${OpenCV_LIBS} ${Python3_LIBRARIES})
target_link_libraries(besign_library ${UUID_LIBRARY} nlohmann_json::nlohmann_json ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} ${QRENCODE_LIB} ${OpenCV_LIBS} ${Python3_LIBRARIES})
target_link_libraries(besign_object ${UUID_LIBRARY} nlohmann_json::nlohmann_json ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} ${QRENCODE_LIB} ${OpenCV_LIBS} ${Python3_LIBRARIES})


# Define custom build directory
set(CUSTOM_CYTHON_BUILD_DIR "${CMAKE_BINARY_DIR}/cython_build")

# Set the virtual environment path
set(VENV_PATH "${CMAKE_BINARY_DIR}/venv")

# Create a custom command to ensure required Python packages (like pymupdf) are installed
add_custom_target(
    install_python_deps ALL
    COMMAND ${Python3_EXECUTABLE} -m venv ${VENV_PATH}
    COMMAND ${VENV_PATH}/bin/pip install --upgrade pip
    COMMAND ${VENV_PATH}/bin/pip install Cython pymupdf numpy qrcode[pil] pillow
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Setting up Python virtual environment and installing dependencies"
)

# Add custom command to build the Cython extension
add_custom_target(
    cython_module ALL
    COMMAND ${VENV_PATH}/bin/python ${CMAKE_SOURCE_DIR}/src/setup.py build_ext --inplace
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building Cython extension"
    DEPENDS install_python_deps
)

# Set the environment variable PYTHONPATH for correct module import
set(ENV{PYTHONPATH} "${VENV_PATH}/lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")

# Build the project using CMake
add_custom_target(
    install_besign
    COMMAND ${VENV_PATH}/bin/python ${CMAKE_SOURCE_DIR}/src/setup.py install
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Installing Cython module"
)

# Ensure that the cython_module is built before running cmake build
add_dependencies(install_besign cython_module)

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
install(TARGETS besign_executable DESTINATION bin)

install(FILES
    build/bin/besign
    DESTINATION ${BIN_DIR}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
)

install(TARGETS besign_library
    LIBRARY DESTINATION ${LIB_DIR}
)

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

set_target_properties(besign_library PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

set_target_properties(besign_object PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj
)

set_target_properties(besign_executable PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(besign_library besign_object besign_executable PROPERTIES OUTPUT_NAME besign)



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
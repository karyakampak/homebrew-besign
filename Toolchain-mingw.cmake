# Specify the target platform
set(CMAKE_SYSTEM_NAME Windows)

# Set MinGW compilers
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Path to OpenSSL, adjust as necessary
set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Path to CURL, adjust as necessary

# Path to CURL, adjust as necessary
set(CURL_LIBRARY "/usr/local/opt/curl/lib/libcurl.dylib")
set(CURL_INCLUDE_DIR "/usr/local/opt/curl/include")

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

# Path to nlohmann_json, adjust version as necessary
find_package(nlohmann_json 3.2.0 REQUIRED)
include_directories(${nlohmann_json_INCLUDE_DIRS})

# Find ZLIB
set(ZLIB_ROOT "/usr/local/opt/zlib")
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

# Optional: Find UUID library
find_library(UUID_LIBRARY NAMES uuid)

# Add MinGW-specific flags if needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

# Optionally, you can set other compiler/linker flags specific to MinGW here

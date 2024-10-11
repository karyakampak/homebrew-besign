# Install script for directory: /Users/pusopskamsinas/Documents/Pribadi/Cpp/besign

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/build/bin/besign")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/besign" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/besign")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/besign")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/build/CMakeFiles/besign_executable.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/build/lib/libbesign.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbesign.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbesign.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libbesign.dylib")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/add_ocsp_trailer.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/add_ocsp_dict.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/get_ocsp_response.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/sign_bsre.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/sign_with_p12.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/sign.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/cms.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/detached_cms.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/place_signature.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/add_placeholder.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/save_pdf.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/add_trailer.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/get_added_index.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/calculate_hash.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/add_signature_dict.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/add_seal_dict.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/check_signature.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/get_page_reference.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/get_pdf_component.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/open_file.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/find_char_position.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/addons.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
  file(WRITE "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()

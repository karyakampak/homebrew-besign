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
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/usr/local/lib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/besign")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/besign")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  include("/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/build/CMakeFiles/besign_executable.dir/install-cxx-module-bmi-noconfig.cmake" OPTIONAL)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/bin/besign")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ FILES "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/build/bin/besign")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libbesign.dylib")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/lib" TYPE SHARED_LIBRARY FILES "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/build/lib/libbesign.dylib")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/libbesign.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/libbesign.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/usr/local/lib"
      "$ENV{DESTDIR}/usr/local/lib/libbesign.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}/usr/local/lib/libbesign.dylib")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/add_ocsp_trailer.h;/usr/local/include/add_ocsp_dict.h;/usr/local/include/get_ocsp_response.h;/usr/local/include/sign_bsre.h;/usr/local/include/sign_with_p12.h;/usr/local/include/sign.h;/usr/local/include/cms.h;/usr/local/include/detached_cms.h;/usr/local/include/place_signature.h;/usr/local/include/add_placeholder.h;/usr/local/include/save_pdf.h;/usr/local/include/add_trailer.h;/usr/local/include/get_added_index.h;/usr/local/include/calculate_hash.h;/usr/local/include/add_signature_dict.h;/usr/local/include/add_seal_dict.h;/usr/local/include/visualization.h;/usr/local/include/check_signature.h;/usr/local/include/get_page_reference.h;/usr/local/include/get_pdf_component.h;/usr/local/include/open_file.h;/usr/local/include/addons.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/include" TYPE FILE FILES
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
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/visualization.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/check_signature.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/get_page_reference.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/get_pdf_component.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/open_file.h"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/header/addons.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/LICENSE.txt;/usr/local/include/README.md;/usr/local/include/AUTHORS.md")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/include" TYPE FILE FILES
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/LICENSE.txt"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/README.md"
    "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/AUTHORS.md"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/lib" TYPE DIRECTORY FILES "/Users/pusopskamsinas/Documents/Pribadi/Cpp/besign/cython_build/" FILES_MATCHING REGEX "/[^/]*$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  
    # Check the current shell and set the appropriate shell config file
    if($ENV{SHELL} MATCHES ".*/zsh")
        set(SHELL_RC "~/.zshrc")
    elseif($ENV{SHELL} MATCHES ".*/bash")
        set(SHELL_RC "~/.bashrc")
    else()
        message(FATAL_ERROR "Unsupported shell: $ENV{SHELL}")
    endif()

    # Define the export command to add PYTHONPATH
    set(EXPORT_COMMAND "echo 'export PYTHONPATH=/usr/local/lib:\$PYTHONPATH' >> ${SHELL_RC}")
    
    # Execute the command to update the shell config
    execute_process(COMMAND /bin/sh -c "${EXPORT_COMMAND}")

    # Source the updated shell config
    execute_process(COMMAND /bin/sh -c "source ${SHELL_RC}")

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

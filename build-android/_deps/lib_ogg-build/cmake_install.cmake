# Install script for directory: /Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src

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
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Users/kevin/Library/Android/sdk/ndk/27.0.12077973/toolchains/llvm/prebuilt/darwin-x86_64/bin/llvm-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY MESSAGE_LAZY FILES "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-build/libogg.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ogg" TYPE FILE MESSAGE_LAZY FILES
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-build/include/ogg/config_types.h"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/include/ogg/ogg.h"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/include/ogg/os_types.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE MESSAGE_LAZY FILES "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-build/ogg.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libogg/html" TYPE FILE MESSAGE_LAZY FILES
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/framing.html"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/index.html"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/oggstream.html"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/ogg-multiplex.html"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/fish_xiph_org.png"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/multiplex1.png"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/packets.png"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/pages.png"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/stream.png"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/vorbisword2.png"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/white-ogg.png"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/white-xifish.png"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/rfc3533.txt"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/rfc5334.txt"
    "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/skeleton.html"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libogg/html" TYPE DIRECTORY MESSAGE_LAZY FILES "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-src/doc/libogg")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_ogg-build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()

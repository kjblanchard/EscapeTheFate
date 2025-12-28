# 3.26 is needed for building al on static
cmake_minimum_required(VERSION 3.5)
project("Escape The Fate" LANGUAGES C CXX VERSION 0.1)
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/cmake/EscapeTheFate.entitlements")
set(XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/cmake/EscapeTheFate.entitlements")
option(SYSTEM_PACKAGES "Use system packages if available" ON)
option(ENGINE_CACHED "Is the engine cloned locally in the ./engine directory or should it be downloaded" ON)
option(DEBUG_CMAKE_VARIABLES "Print out all debug variables near end of configure" OFF)
option(DEBUG_BUILD_TIME "Add in the pch that I think will be changing a bit" OFF)
option(LINK_M "Should we link against the M, only useful on linux builds locally")
# If ON, gives a debug message when building about vm space or something, nanomalloc. If ON, Memory shows super high
# usage.. it's wild, to get an accurate reading switch to off.
option(DEBUG_ASAN "Use asan for leak and address sanitization checks when debug mode" OFF)
option(CONVERT_JSON_TO_LUA_IN_ASEPRITE "Convert all json files in aseprite folder for use during install step" OFF)
option(SDL_BACKEND "Does this use SDL backend?" ON)
option(SDL_GL "Does this use SDL GL backend?" ON)
# Enable AddressSanitizer only in Debug builds (excluding iOS and iOS Simulator)
include(FetchContent)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_TARGET_NAME EscapeTheFate)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_MESSAGE LAZY)
# Add defines for specific builds

if(MSVC)
  set(INSTALL_DIR
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>
      CACHE INTERNAL "")
elseif(APPLE)
  if(CMAKE_GENERATOR STREQUAL "Xcode")
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
      add_definitions(-DIS_MOBILE)
      set(INSTALL_DIR
          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${EXECUTABLE_TARGET_NAME}.app
          CACHE INTERNAL "")
    else()
      set(INSTALL_DIR
          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${EXECUTABLE_TARGET_NAME}.app/Contents/Resources
          CACHE INTERNAL "")
    endif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  else()
    set(INSTALL_DIR
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE_TARGET_NAME}.app/Contents/Resources
        CACHE INTERNAL "")
  endif(CMAKE_GENERATOR STREQUAL "Xcode")
else()
  set(INSTALL_DIR
      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      CACHE STRING "" FORCE)
endif(MSVC)

if(ENGINE_CACHED STREQUAL "ON")
  add_subdirectory(SupergoonEngine)
else()
  message(STATUS "Fetching supergoon engine from git..")
  FetchContent_Declare(
    SupergoonEngine
    GIT_REPOSITORY https://github.com/kjblanchard/SupergoonEngine
    # Use master for development, otherwise make sure to pin to proper engine tag for releases GIT_TAG c55aa25 GIT_TAG
    # master
    GIT_TAG master)
  FetchContent_MakeAvailable(SupergoonEngine)
endif(ENGINE_CACHED STREQUAL "ON")

install(
  DIRECTORY src/
  DESTINATION ${INSTALL_DIR}/assets/lua
  COMPONENT assets)

install(
  DIRECTORY assets
  DESTINATION ${INSTALL_DIR}
  COMPONENT assets)

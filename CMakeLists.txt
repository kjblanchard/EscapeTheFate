# 3.26 is needed for building al on static
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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

#glaze speed
# set(CMAKE_UNITY_BUILD ON)
# set(CMAKE_UNITY_BUILD_BATCH_SIZE 8)  # optional

# Add defines for specific builds
add_executable(${EXECUTABLE_TARGET_NAME} MACOSX_BUNDLE WIN32 
  src/main.cpp 
  src/gameConfig.cpp 
  src/gameState.cpp 
  src/gameobject/GameObject.cpp
  src/gameobject/gameobjects/Player.cpp
  src/gameobject/gameobjects/Textbox.cpp
  src/gameobject/gameobjects/MapExit.cpp
  src/ui/ui.cpp
  src/ui/uiObject.cpp
  src/ui/uiNineSlice.cpp
  src/ui/uiImage.cpp
  src/ui/uiText.cpp
  src/systems/dialogSystem.cpp
  src/bindings/engine.cpp
  ${ICON_PATH})

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

if(APPLE)
  set_target_properties(${EXECUTABLE_TARGET_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/Info.plist)
    set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${CMAKE_SOURCE_DIR}/cmake/Info.plist PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_target_properties(
      ${EXECUTABLE_TARGET_NAME}
        PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/cmake/EscapeTheFate.entitlements"
        # XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
        XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
        XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "16.3"
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "H34NGW287W"
        # when building on github, we don't want to sign anything cause there is no cert
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO"
        # XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development"
    )
  endif(APPLE)

if(ENGINE_CACHED STREQUAL "ON")
  add_subdirectory(SupergoonEngine)
else()
  message(STATUS "Fetching supergoon engine from git..")
  FetchContent_Declare(
    SupergoonEngine
    GIT_REPOSITORY https://github.com/kjblanchard/SupergoonEngine
    # Use master for development, otherwise make sure to pin to proper engine tag for releases GIT_TAG c55aa25 GIT_TAG
    # master
    GIT_TAG cpp)
  FetchContent_MakeAvailable(SupergoonEngine)
endif(ENGINE_CACHED STREQUAL "ON")

include(FetchContent)

set(GLZ_USE_FAST_FLOAT 0 CACHE BOOL "" FORCE)
set(GLZ_USE_CONCEPTS 0 CACHE BOOL "" FORCE)
set(GLZ_ENABLE_EXCEPTIONS 1 CACHE BOOL "" FORCE)
set(GLZ_ENABLE_RTTI 1 CACHE BOOL "" FORCE)
set(GLZ_ENABLE_JSON 1 CACHE BOOL "" FORCE)
set(GLZ_ENABLE_BINARY 0 CACHE BOOL "" FORCE)
set(GLZ_ENABLE_YAML 0 CACHE BOOL "" FORCE)
set(GLZ_ENABLE_TOML 0 CACHE BOOL "" FORCE)

# if(SYSTEM_PACKAGES)
#   find_package(glaze)
# endif(SYSTEM_PACKAGES)
# if(NOT glaze_FOUND)
#   message(STATUS "Glaze not found. Fetching Glaze...")
#   FetchContent_Declare(
#     LIB_GLAZE
#     GIT_REPOSITORY https://github.com/stephenberry/glaze.git
#     GIT_TAG main
#     GIT_SHALLOW TRUE
#     EXCLUDE_FROM_ALL
#   )
#   FetchContent_MakeAvailable(LIB_GLAZE)
# endif(NOT glaze_FOUND)
# target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE glaze::glaze)


# Json lib, need to do this when building in github only
# FetchContent_Declare(
#   glaze
#   GIT_REPOSITORY https://github.com/stephenberry/glaze.git
#   GIT_TAG main
#   GIT_SHALLOW TRUE
#   EXCLUDE_FROM_ALL
# )
# FetchContent_MakeAvailable(glaze)

target_include_directories(${EXECUTABLE_TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

if(DEBUG_ASAN AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(NOT (APPLE AND (CMAKE_SYSTEM_NAME STREQUAL "iOS" OR CMAKE_SYSTEM_NAME STREQUAL "iossimulator")) AND NOT (CMAKE_SYSTEM_NAME STREQUAL "Emscripten"))
    message(STATUS "Enabling AddressSanitizer in Debug mode")
    set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ASAN_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ASAN_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ASAN_FLAGS}")

  endif()
endif()

# target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE glaze::glaze)

target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE SupergoonEngine)

# Platform specific executable flags
if(EMSCRIPTEN)
  message(STATUS "Enabling Emscripten flags for supergoon engine ")
  set(CMAKE_EXECUTABLE_SUFFIX
    ".html"
    CACHE STRING "Use html" FORCE)
# cannot use sdl3 currently
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=3 -s USE_ZLIB=1  -s USE_VORBIS=1 -s USE_OGG=1 ")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=3 -s USE_ZLIB=1  -s USE_VORBIS=1 -s USE_OGG=1 ")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -s USE_ZLIB=1  -s USE_VORBIS=1 -s USE_OGG=1 ")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -s USE_ZLIB=1  -s USE_VORBIS=1 -s USE_OGG=1 ")
  set(LINK_FLAGS
    # "${LINK_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=3 -s USE_VORBIS=1 -s USE_OGG=1 -s FULL_ES3=1 -s USE_WEBGL2=1 -s STACK_SIZE=1mb -s EXPORTED_RUNTIME_METHODS=cwrap -sMAX_WEBGL_VERSION=2 -sMIN_WEBGL_VERSION=2 -s 'DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=[\"$autoResumeAudioContext\",\"$dynCall\"]'"
    "${LINK_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s USE_VORBIS=1 -s USE_OGG=1 -s FULL_ES3=1 -s USE_WEBGL2=1 -s STACK_SIZE=1mb -s EXPORTED_RUNTIME_METHODS=cwrap -sMAX_WEBGL_VERSION=2 -sMIN_WEBGL_VERSION=2 -s 'DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=[\"$autoResumeAudioContext\",\"$dynCall\"]'"
  )
  set_target_properties(
    ${EXECUTABLE_TARGET_NAME}
    PROPERTIES
    LINK_FLAGS
    "${LINK_FLAGS} -O0 -s SAFE_HEAP=2 -s STACK_OVERFLOW_CHECK=1 \
    --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets")
    # --preload-file ${CMAKE_SOURCE_DIR}/src@/assets/lua/ \
    # --preload-file ${ENGINE_SCRIPT_PATH}@/assets/lua/ ")
  elseif(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET
      "10.14"
      CACHE STRING "Minimum OS X deployment version")
    set(IS_BUNDLE
      "MACOSX_BUNDLE"
      CACHE STRING "Is this a bundle")
    set(ICON_PATH
      ${CMAKE_SOURCE_DIR}/cmake/AppIcon.icns
      CACHE STRING "Icon path")
  endif(EMSCRIPTEN)

  # install(
  #   DIRECTORY src/
  #   DESTINATION ${INSTALL_DIR}/assets/lua
  #   COMPONENT assets)

  install(
    DIRECTORY assets
    DESTINATION ${INSTALL_DIR}
    COMPONENT assets)

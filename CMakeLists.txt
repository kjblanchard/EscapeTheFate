cmake_minimum_required(VERSION 3.22)

if(CMAKE_GENERATOR STREQUAL "Xcode")
    message(STATUS "Setting Build to Universal Binary")
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" FORCE)
endif(CMAKE_GENERATOR STREQUAL "Xcode")

set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/cmake/entitlements.plist")
set(XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/cmake/entitlements.plist")

project(SupergoonEngine)

# Engine is C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Should we include Imgui stuffs
option(imgui "Include ImGui support" ON)
option(SYSTEM_PACKAGES "Use system packages if available" ON)
option(ENGINE_CACHED "Is the engine cloned locally in the ./engine directory or should it be downloaded" ON)
option(DEBUG_CMAKE_VARIABLES "Print out all debug variables near end of configure" OFF)
option(DEBUG_BUILD_TIME "Add in the pch that I think will be changing a bit" OFF)
option(DEBUG_ASAN "Use asan for leak and address sanitization checks when debug mode" ON)
option(CONVERT_JSON_TO_LUA_IN_ASEPRITE "Convert all json files in aseprite folder for use during install step" OFF)

if(DEBUG_BUILD_TIME)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
endif(DEBUG_BUILD_TIME)

# # Enable AddressSanitizer only in Debug builds
# if(DEBUG_ASAN AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT (APPLE and CMAKE_SYSTEM_NAME STREQUAL "iOS" or (CMAKE_SYSTEM_NAME STREQUAL "iossimulator") ))
#     message(STATUS "Enabling AddressSanitizer in Debug mode")
#     set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer")

#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ASAN_FLAGS}")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
# endif(DEBUG_ASAN and CMAKE_BUILD_TYPE STREQUAL "Debug")

# Enable AddressSanitizer only in Debug builds (excluding iOS and iOS Simulator)
if(DEBUG_ASAN AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(NOT (APPLE AND (CMAKE_SYSTEM_NAME STREQUAL "iOS" OR CMAKE_SYSTEM_NAME STREQUAL "iossimulator"))
    AND NOT (CMAKE_SYSTEM_NAME STREQUAL "Emscripten"))
        message(STATUS "Enabling AddressSanitizer in Debug mode")
        set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ASAN_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
    endif()
endif()



include(FetchContent)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_TARGET_NAME EscapeTheFate)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(imgui)
    add_definitions(-Dimgui)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif(imgui)



if(DEBUG_CMAKE_VARIABLES)
    get_cmake_property(ALL_VARIABLES VARIABLES)

    foreach(VAR ${ALL_VARIABLES})
        message(STATUS "${VAR} = ${${VAR}}")
    endforeach()
endif(DEBUG_CMAKE_VARIABLES)

set(FOLDERS_TO_COPY assets)
set(CMAKE_INSTALL_MESSAGE LAZY)

# set(INSTALL_DIR ${INSTALL_DIR} CACHE INTERNAL "Install path for all subdirs")

if(MSVC)
    # set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG> CACHE STRING "" FORCE)
    set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG> CACHE INTERNAL "" )
elseif(APPLE)
    if(CMAKE_GENERATOR STREQUAL "Xcode")
        if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
            add_definitions(-DIS_MOBILE)
            set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${EXECUTABLE_TARGET_NAME}.app CACHE INTERNAL "" )
        else()
            set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/${EXECUTABLE_TARGET_NAME}.app/Contents/Resources CACHE INTERNAL "" )
        endif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    else()
        set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE_TARGET_NAME}.app/Contents/Resources CACHE INTERNAL "" )
    endif(CMAKE_GENERATOR STREQUAL "Xcode")

else()
    set(INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE STRING "" FORCE)
endif(MSVC)


# if(ENGINE_CACHED AND SYSTEM_PACKAGES)
add_subdirectory(engine)
# else()
# message(STATUS "Fetching supergoon engine from git..")
# FetchContent_Declare(
# engine
# GIT_REPOSITORY https://github.com/kjblanchard/sgEngine

# # Use master for development, otherwise make sure to pin to proper engine tag for releases
# GIT_TAG c55aa25
# # GIT_TAG master
# )
# FetchContent_MakeAvailable(engine)
# endif(ENGINE_CACHED AND SYSTEM_PACKAGES)
add_subdirectory(src)

install(DIRECTORY ${FOLDERS_TO_COPY}
    DESTINATION ${INSTALL_DIR}
    COMPONENT assets)

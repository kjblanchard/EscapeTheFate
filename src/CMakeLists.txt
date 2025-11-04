cmake_minimum_required(VERSION 3.16)
message(STATUS "Starting ${EXECUTABLE_TARGET_NAME} game initialization")
set(FOLDERS_TO_COPY scripts/)
# Should we show everything, or just a few.
set(CMAKE_INSTALL_MESSAGE LAZY)

if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX
      ".html"
      CACHE STRING "Use html" FORCE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=3 -s USE_ZLIB=1  -s USE_VORBIS=1 -s USE_OGG=1 ")
  set(LINK_FLAGS
      "${LINK_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=3 -s USE_VORBIS=1 -s USE_OGG=1 -s FULL_ES3=1 -s USE_WEBGL2=1 -s STACK_SIZE=1mb -s EXPORTED_RUNTIME_METHODS=cwrap -sMAX_WEBGL_VERSION=2 -sMIN_WEBGL_VERSION=2 -s 'DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=[\"$autoResumeAudioContext\",\"$dynCall\"]'"
  )
elseif(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET
      "10.14"
      CACHE STRING "Minimum OS X deployment version")
  if(NOT CREATE_MACOS_TUI)
    set(IS_BUNDLE
        "MACOSX_BUNDLE"
        CACHE STRING "Is this a bundle")
    set(ICON_PATH
        ${CMAKE_SOURCE_DIR}/cmake/AppIcon.icns
        CACHE STRING "Icon path")
  endif(NOT CREATE_MACOS_TUI)

endif(EMSCRIPTEN)

add_executable(${EXECUTABLE_TARGET_NAME} ${IS_BUNDLE} ${IS_WIN32} main.c ${DEBUG_FILES} ${ICON_PATH})

if(APPLE)
  set_target_properties(${EXECUTABLE_TARGET_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST
                                                             ${CMAKE_SOURCE_DIR}/cmake/Info.plist)
  set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  set_source_files_properties(${CMAKE_SOURCE_DIR}/cmake/Info.plist PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  set_target_properties(
    ${EXECUTABLE_TARGET_NAME}
    PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/cmake/EscapeTheFate.entitlements"
               XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
               XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "H34NGW287W"
               # XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development"
  )
elseif(EMSCRIPTEN)
  set_target_properties(
    ${EXECUTABLE_TARGET_NAME}
    PROPERTIES
      LINK_FLAGS
      "${LINK_FLAGS} -O0 -s SAFE_HEAP=2 -s STACK_OVERFLOW_CHECK=1 \
    --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets \
    --preload-file ${CMAKE_SOURCE_DIR}/src/scripts@/assets/lua/\
    --preload-file ${CMAKE_SOURCE_DIR}/engine/scripting@/assets/lua/")
endif(APPLE)

target_compile_options(${EXECUTABLE_TARGET_NAME} PRIVATE ${FULL_COMPILE_OPTIONS})
target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE SupergoonEngine)
target_include_directories(
  ${EXECUTABLE_TARGET_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/src/engine/include ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}_
          /emsdk/upstream/emscripten/cache/sysroot/include /usr/local/include/freetype2) # Add the freetype2 specific
                                                                                         # include directory
install(
  DIRECTORY ${FOLDERS_TO_COPY}
  DESTINATION ${INSTALL_DIR}/assets/lua
  COMPONENT assets)

# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_vorbis-src")
  file(MAKE_DIRECTORY "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_vorbis-src")
endif()
file(MAKE_DIRECTORY
  "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_vorbis-build"
  "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_vorbis-subbuild/lib_vorbis-populate-prefix"
  "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_vorbis-subbuild/lib_vorbis-populate-prefix/tmp"
  "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_vorbis-subbuild/lib_vorbis-populate-prefix/src/lib_vorbis-populate-stamp"
  "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_vorbis-subbuild/lib_vorbis-populate-prefix/src"
  "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_vorbis-subbuild/lib_vorbis-populate-prefix/src/lib_vorbis-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_vorbis-subbuild/lib_vorbis-populate-prefix/src/lib_vorbis-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/kevin/git/cpp/EscapeTheFate/build-android/_deps/lib_vorbis-subbuild/lib_vorbis-populate-prefix/src/lib_vorbis-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()

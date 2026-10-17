cmake_minimum_required(VERSION 3.15)

project(maximilian LANGUAGES C CXX)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(MAXIMILIAN_IS_TOP_LEVEL ON)
else()
  set(MAXIMILIAN_IS_TOP_LEVEL OFF)
endif()

option(BUILD_SHARED_LIBS "Build shared instead of static libraries" OFF)
option(MAXIMILIAN_VORBIS "Enable OGG Vorbis loading via stb_vorbis" OFF)
option(MAXIMILIAN_BUILD_COMMANDLINE "Build the RtAudio player, examples and tests" ${MAXIMILIAN_IS_TOP_LEVEL})
option(MAXIMILIAN_INSTALL "Generate install and export rules" ${MAXIMILIAN_IS_TOP_LEVEL})
option(MAXIMILIAN_BUILD_TESTS "Build tests and register them with CTest" ${MAXIMILIAN_IS_TOP_LEVEL})
option(MAXIMILIAN_SANITIZE "Build everything with AddressSanitizer and UBSan" OFF)

if(MAXIMILIAN_SANITIZE)
  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(FATAL_ERROR "MAXIMILIAN_SANITIZE requires GCC or Clang")
  endif()
  # No recovery: a UBSan report must fail the test, not just print.
  add_compile_options(-fsanitize=address,undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

add_library(maximilian
  src/maximilian.cpp
  src/libs/fft.cpp
  src/libs/maxiAtoms.cpp
  src/libs/maxiBark.cpp
  src/libs/maxiClock.cpp
  src/libs/maxiConvolve.cpp
  src/libs/maxiFFT.cpp
  src/libs/maxiGrains.cpp
  src/libs/maxiMFCC.cpp
  src/libs/maxiReverb.cpp
  src/libs/maxiSynths.cpp
  src/libs/PolyBLEP/PolyBLEP.cpp
)
add_library(maximilian::maximilian ALIAS maximilian)

target_compile_features(maximilian PUBLIC cxx_std_17)
target_include_directories(maximilian PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/libs>
  $<INSTALL_INTERFACE:include/maximilian>
  $<INSTALL_INTERFACE:include/maximilian/libs>
)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(maximilian PRIVATE -Wall)
endif()

# fft.cpp, maxiMFCC.cpp and maxiAtoms.cpp use vDSP when built by Apple clang.
if(APPLE)
  target_link_libraries(maximilian PUBLIC "-framework Accelerate")
endif()

# VORBIS changes declarations in maximilian.h, so consumers must see it too.
if(MAXIMILIAN_VORBIS)
  target_sources(maximilian PRIVATE src/libs/stb_vorbis.c)
  target_compile_definitions(maximilian PUBLIC VORBIS)
endif()

# Before cpp/commandline, which registers smoke tests.
if(MAXIMILIAN_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(MAXIMILIAN_BUILD_COMMANDLINE)
  add_subdirectory(cpp/commandline)
endif()

if(MAXIMILIAN_INSTALL)
  include(GNUInstallDirs)
  install(TARGETS maximilian EXPORT maximilianTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
  # Headers include each other as "../maximilian.h", so keep the src/ layout.
  install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/maximilian
    FILES_MATCHING PATTERN "*.h"
  )
  install(EXPORT maximilianTargets
    NAMESPACE maximilian::
    FILE maximilianConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/maximilian
  )
endif()

cmake_minimum_required(VERSION 3.0)

# Project settings
project(SIKE NONE)
enable_language(C)
enable_language(CXX)
enable_language(ASM)

# Environment settings
set(PERL_EXECUTABLE "perl")

# Check ARCH
if(NO_ASM)
  set(ARCH "generic")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
  set(ARCH "x86_64")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "amd64")
  set(ARCH "x86_64")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64")
  # used on windows
  set(ARCH "x86_64")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
  set(ARCH "aarch64")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
  set(ARCH "aarch64")
else()
  set(ARCH "generic")
endif()

if(CMAKE_OSX_ARCHITECTURES)
  set(FLAVOUR "macosx")
else()
  set(FLAVOUR "linux")
endif()

# Set per ARCH defaults
if(${ARCH} STREQUAL "aarch64")
  set(
    CRYPTO_ARCH_SOURCES

    asm/fp-aarch64.S
  )
  set(CMAKE_C_FLAGS "-DARCH_AARCH64 -DCPU_64_BIT -DHAS_UINT128")
  set(CMAKE_CXX_FLAGS "-DARCH_AARCH64 -DCPU_64_BIT -DHAS_UINT128")
elseif(${ARCH} STREQUAL "x86_64")
  set(
    CRYPTO_ARCH_SOURCES

    asm/fp-x86_64.S
  )
  set(CMAKE_C_FLAGS "-DARCH_X86_64 -DCPU_64_BIT -DHAS_UINT128")
  set(CMAKE_CXX_FLAGS "-DARCH_X86_64 -DCPU_64_BIT -DHAS_UINT128")
elseif(${ARCH} STREQUAL "generic")
  set(CMAKE_C_FLAGS "-DARCH_GENERIC")
else()
  message(FATAL_ERROR "Something got wrong for CPU:" ${CMAKE_SYSTEM_PROCESSOR})
endif()

# Set compilation flags
if(DEBUG)
  set(CMAKE_C_FLAGS "-g")
else()
  set(CMAKE_C_FLAGS "-O3")
endif()

# Common include directories
include_directories(
    3rd
    3rd/googletest/include
    includes
)

# Add minimal googletest targets.
add_library(boringssl_gtest 3rd/googletest/src/gtest-all.cc)
target_include_directories(boringssl_gtest PRIVATE 3rd/googletest)

# Generates ASM file
add_custom_command(
  OUTPUT asm/fp-x86_64.S
  COMMAND ${CMAKE_COMMAND} -E make_directory asm
  COMMAND ${PERL_EXECUTABLE} ../src/sike-p434-sha256/asm/fp-x86_64.pl ${FLAVOUR} asm/fp-x86_64.S
  DEPENDS
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src/sike-p434-sha256/asm/fp-x86_64.pl
  ${PROJECT_SOURCE_DIR}/3rd/boringssl/arm-xlate.pl
  ${PROJECT_SOURCE_DIR}/3rd/boringssl/x86_64-xlate.pl
  WORKING_DIRECTORY .
)

add_custom_command(
  OUTPUT asm/fp-aarch64.S
  COMMAND ${CMAKE_COMMAND} -E make_directory asm
  COMMAND ${PERL_EXECUTABLE} ../src/sike-p434-sha256/asm/fp-aarch64.pl ${FLAVOUR} asm/fp-aarch64.S
  DEPENDS
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src/sike-p434-sha256/asm/fp-aarch64.pl
  ${PROJECT_SOURCE_DIR}/3rd/boringssl/arm-xlate.pl
  ${PROJECT_SOURCE_DIR}/3rd/boringssl/x86_64-xlate.pl
  WORKING_DIRECTORY .
)

# SIKE/p434 library
add_library(
  sike434sha256

  src/sike-p434-sha256/fpx.c
  src/sike-p434-sha256/isogeny.c
  src/sike-p434-sha256/params.c
  src/sike-p434-sha256/sike.c
  
  ${CRYPTO_ARCH_SOURCES}
  src/sike-p434-sha256/asm/fp_generic.c

  3rd/random/randombytes.c
  3rd/sha2/sha256.c
)

# Tests
add_executable(
  testsike434

  test/p434.cc
  3rd/googletest/src/gtest_main.cc
)

include_directories(
  src/sike-p434-sha256
)

target_link_libraries(
    testsike434
    sike434sha256
    boringssl_gtest
    pthread)

# Speed
add_executable(
  speedsike434

  test/speed434.cc
)

include_directories(
  src/sike-p434-sha256
  src/internal
)

target_link_libraries(
    speedsike434
    sike434sha256)

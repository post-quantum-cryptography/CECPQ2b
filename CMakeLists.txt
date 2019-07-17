cmake_minimum_required(VERSION 3.0)

# Defer enabling C and CXX languages.
project(SIKE NONE)

enable_language(C)
enable_language(CXX)

# Temporary flags for SIKE
set(CMAKE_C_FLAGS "-DARCH_NO_ASM -DCPU_64_BIT -DHAS_UINT128 -g")

# Common include directories
include_directories(
    3rd
    3rd/googletest/include
    includes
)

# Add minimal googletest targets.
add_library(boringssl_gtest 3rd/googletest/src/gtest-all.cc)
target_include_directories(boringssl_gtest PRIVATE 3rd/googletest)

# SIKE/p434 library
add_library(
  sike434sha256

  src/sike-p434-sha256/fpx.c
  src/sike-p434-sha256/isogeny.c
  src/sike-p434-sha256/params.c
  src/sike-p434-sha256/sike.c
  src/sike-p434-sha256/asm/fp_generic.c

  3rd/random/randombytes.c
  3rd/sha2/sha256.c
)

# Tests

add_executable(
  test

  test/p434.cc
  3rd/googletest/src/gtest_main.cc
)

include_directories(
  src/sike-p434-sha256
)

target_link_libraries(
    test
    sike434sha256
    boringssl_gtest
    pthread)

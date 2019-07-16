cmake_minimum_required(VERSION 3.0)

# Defer enabling C and CXX languages.
project(SIKE NONE)

enable_language(C)
enable_language(CXX)

# Temporary flags for SIKE
set(CMAKE_C_FLAGS "-DARCH_NO_ASM -DCPU_64_BIT -DHAS_UINT128")

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
  sike434

  src/p434/fpx.c
  src/p434/isogeny.c
  src/p434/params.c
  src/p434/sike.c
  src/p434/asm/fp_generic.c

  3rd/random/randombytes.c
  3rd/sha2/sha256.c
)

# Tests

add_executable(
  test

  test/p434.cc
  3rd/googletest/src/gtest_main.cc
)

target_link_libraries(
    test
    sike434
    boringssl_gtest
    pthread)

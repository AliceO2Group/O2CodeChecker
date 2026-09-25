cmake_minimum_required(VERSION 4.0)
enable_testing()

project(O2CodeChecker)

# LLVM's installed HandleLLVMOptions.cmake sets no C++ standard, so without this
# we inherit whatever the compiler defaults to -- C++14 on Apple clang, which is
# not enough for the clang-tidy headers.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find clang + llvm
find_package(Clang REQUIRED CONFIG)

if( LLVM_FOUND )
  list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
  include(AddLLVM)

  # set the compiler flags to match llvm
  include(HandleLLVMOptions)
endif()

# Make sure that our source directory is on the current cmake module path so that
# we can include cmake files from this directory.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# include Clang macros (unfortunately they are not part of the cmake installation)
include(AddClang)

# the main executable
add_subdirectory(tool)

# the specific aliceO2 checker code
add_subdirectory(aliceO2)

# for testing
add_subdirectory(test)

# some extra utilities
add_subdirectory(utility)

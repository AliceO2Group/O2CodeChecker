cmake_minimum_required(VERSION 3.4.3)
enable_testing()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # require at least gcc 4.9 !!
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
    message(FATAL_ERROR "GCC version must be at least 4.9!")
  endif()
endif()


if( CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR )
  project(O2CodeChecker)


  # find clang + llvm
  find_package(Clang REQUIRED)

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

  # add include directories
  include_directories(${LLVM_INCLUDE_DIRS})

set(LLVM_LINK_COMPONENTS
  Support
  )

# the main executable
add_subdirectory(tool)

# the specific aliceO2 checker code
add_subdirectory(aliceO2)
# the specific reporting tools code
add_subdirectory(reporting)


# for testing
add_subdirectory(test)

endif()

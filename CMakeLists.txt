cmake_minimum_required(VERSION 3.4.3)
enable_testing()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # require at least gcc 12.0 !!
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 12.0)
    message(FATAL_ERROR "GCC version must be at least 12.0!")
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

# plugin
add_subdirectory(plugin)

# for testing
add_subdirectory(test)

# some extra utilities
add_subdirectory(utility)

string(REPLACE "." ";" LLVM_PACKAGE_VERSION_LIST ${LLVM_PACKAGE_VERSION})
list(GET LLVM_PACKAGE_VERSION_LIST 0 LLVM_PACKAGE_VERSION_MAJOR)
install(FILES omp.h DESTINATION lib/clang/${LLVM_PACKAGE_VERSION_MAJOR}/include)

endif()

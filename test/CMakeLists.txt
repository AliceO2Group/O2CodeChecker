# testing if regex works correctly (has problem with gcc < 4.9)
add_executable(testregex testregex.cpp)
add_test(testregexcorrect testregex)

# we need the FileCheck program in order to execute the check_clang_tidy.py script
find_program(LLVMFILECHECK FileCheck HINTS PATH)
if (LLVMFILECHECK)

# testing namespace naming checker

  add_test(NAME aliceo2namespace COMMAND 
                ${CMAKE_SOURCE_DIR}/check_clang_tidy.py 
                ${CMAKE_SOURCE_DIR}/test/aliceO2-namespace-naming.cpp 
                aliceO2-namespace-naming ${CMAKE_BINARY_DIR}/tool/O2codecheck))

else()
  message(STATUS "FileCheck not found; disabling the check_clang_tidy unit tests\n")
endif(LLVMFILECHECK)


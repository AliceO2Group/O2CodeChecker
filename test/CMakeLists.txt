# testing if regex works correctly (has problem with gcc < 4.9)
add_executable(testregex testregex.cpp)
add_test(testregexcorrect testregex)
add_test(aliceo2namespace COMMAND check_clang_tidy.py ${CMAKE_SOURCE_DIR}/test/aliceO2-namespace-naming.cpp 'aliceO2-namespace-naming')

# correctness testing of checks

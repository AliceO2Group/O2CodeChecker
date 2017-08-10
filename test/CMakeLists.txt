# testing if regex works correctly (has problem with gcc < 4.9)
add_executable(testregex testregex.cpp)
add_test(testregexcorrect testregex)

# correctness testing of checks

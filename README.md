# O2CodeChecker
A prototype for a standalone code checker for the AliceO2 project

It offers:
  * code checks based on clang-tidy and existing components
  * standalone CMake infrastructure for compilation outside of llvm/clang source tree

# Requirements/Building

  * llvm + clang + clang extra tools (release 3_9) required
  * build using `cmake -DLLVM_DIR=PATH_TO_LLVM_CMAKE_CONFIG -DClang_DIR=PATH_TO_CLANG_CONFIG`

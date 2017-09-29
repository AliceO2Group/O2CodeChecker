# O2CodeChecker
A prototype for a standalone code checker for the AliceO2 project

It offers:
  * code checks based on clang-tidy and existing components
  * standalone CMake infrastructure for compilation outside of llvm/clang source tree

# Requirements/Building

  * llvm + clang + clang extra tools (release 3_9) required
    * (recommended to use `alibuild build clang` which will give you the right version)
  * build using `cmake -DLLVM_DIR=PATH_TO_LLVM_CMAKE_CONFIG -DClang_DIR=PATH_TO_CLANG_CONFIG`

# Usage

In the build directory of AliceO2 (containing the CMake compilations database in form of `compile_command.json`), run

    run_O2CodeChecker.py -clang-tidy-binary `which O2codecheck` -checks=-*,alice*


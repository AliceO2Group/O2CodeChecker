set(LLVM_LINK_COMPONENTS
  support
  )

add_clang_executable(O2codecheck
  ClangTidyMain.cpp
  )

target_link_libraries(O2codecheck
  clangAST
  clangASTMatchers
  clangBasic
  clangTidy

  # link our own checks
  clangTidyAliceO2Module

  # include checkers available from main clang-tidy
  clangTidyBoostModule
  clangTidyCERTModule
  clangTidyCppCoreGuidelinesModule
  clangTidyGoogleModule
  clangTidyLLVMModule
  clangTidyMiscModule
  clangTidyModernizeModule
  clangTidyPerformanceModule
  clangTidyReadabilityModule

  clangTooling
  )

install(TARGETS O2codecheck
  RUNTIME DESTINATION bin)

#install(PROGRAMS clang-tidy-diff.py DESTINATION share/clang)
#install(PROGRAMS run-clang-tidy.py DESTINATION share/clang)

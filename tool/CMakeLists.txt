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
  clangTidyReportingModule

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
install(PROGRAMS run_O2CodeChecker.py DESTINATION bin)

# we need to install the builtin headers in a path which is searched by the tool
# FIXME: a soft link would be better
install(DIRECTORY ${LLVM_LIBRARY_DIR}/clang/${LLVM_PACKAGE_VERSION}/include DESTINATION lib/clang/${LLVM_PACKAGE_VERSION})

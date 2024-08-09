set(LLVM_LINK_COMPONENTS
  AllTargetsAsmParsers
  AllTargetsDescs
  AllTargetsInfos
  support
  )

add_clang_executable(O2codecheck
  ClangTidyMain.cpp
  )

target_link_libraries(O2codecheck
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangTidy

  # link our own checks
  clangTidyAliceO2Module
  clangTidyReportingModule

  # include checkers available from main clang-tidy
  clangTidyAlteraModule
  clangTidyAndroidModule
  clangTidyAbseilModule
  clangTidyBoostModule
  clangTidyBugproneModule
  clangTidyCERTModule
  clangTidyConcurrencyModule
  clangTidyCppCoreGuidelinesModule
  clangTidyDarwinModule
  clangTidyFuchsiaModule
  clangTidyGoogleModule
  clangTidyHICPPModule
  clangTidyLinuxKernelModule
  clangTidyLLVMModule
  clangTidyLLVMLibcModule
  clangTidyMiscModule
  clangTidyModernizeModule
  clangTidyMPIModule
  clangTidyObjCModule
  clangTidyOpenMPModule
  clangTidyPerformanceModule
  clangTidyPortabilityModule
  clangTidyReadabilityModule
  clangTidyZirconModule
  clangTooling
  clangToolingCore
  )

install(TARGETS O2codecheck
  RUNTIME DESTINATION bin)

#install(PROGRAMS clang-tidy-diff.py DESTINATION share/clang)
install(PROGRAMS run_O2CodeChecker.py DESTINATION bin)

# we need to install the builtin headers in a path which is searched by the tool
# FIXME: a soft link would be better
string(REPLACE "." ";" LLVM_PACKAGE_VERSION_LIST ${LLVM_PACKAGE_VERSION})
list(GET LLVM_PACKAGE_VERSION_LIST 0 LLVM_PACKAGE_VERSION_MAJOR)
install(DIRECTORY ${LLVM_LIBRARY_DIR}/clang/${LLVM_PACKAGE_VERSION_MAJOR}/include DESTINATION lib/clang/${LLVM_PACKAGE_VERSION_MAJOR})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # On MacOS we need to do the same with C++ headers since they are in a non-standard location
  # (alternative would be to set the CPATH environment variable)
  INSTALL(CODE "execute_process(COMMAND mkdir ${CMAKE_INSTALL_PREFIX}/include)")
  INSTALL(CODE "execute_process(COMMAND ln -sf \
      /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++ \
      ${CMAKE_INSTALL_PREFIX}/include/c++)")
ENDIF()

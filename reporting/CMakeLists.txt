set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyReportingModule
  ReportingTidyModule.cpp
  InterfaceLister.cpp
  VirtFuncLister.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTidyUtils
  clangTooling
  )

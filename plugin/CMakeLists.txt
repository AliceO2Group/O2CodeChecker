set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyPluginModule
  SHARED

  FooCheck.cpp
  PluginTidyModule.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTidyUtils
  clangTooling
  )

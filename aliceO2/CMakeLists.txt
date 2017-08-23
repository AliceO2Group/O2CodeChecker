set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyAliceO2Module
  AliceO2TidyModule.cpp
  MemberNamesCheck.cpp
  NamespaceNamingCheck.cpp
  SizeofCheck.cpp
  VariableNamingConstexprCheck.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTidyUtils
  clangTooling
  )

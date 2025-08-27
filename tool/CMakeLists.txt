set(LLVM_LINK_COMPONENTS
  AllTargetsAsmParsers
  AllTargetsDescs
  AllTargetsInfos
  FrontendOpenMP
  TargetParser
  support
  )

# Needed by LLVM's CMake checks because this file defines multiple targets.
set(LLVM_OPTIONAL_SOURCES ClangTidyMain.cpp ClangTidyToolMain.cpp)

add_clang_library(clangTidyMainO2 STATIC
  ClangTidyMain.cpp

  LINK_LIBS
  clangTidy
  ${ALL_CLANG_TIDY_CHECKS}

  DEPENDS
  omp_gen
  )

target_include_directories(clangTidyMainO2
  PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
  )
target_link_libraries(clangTidyMainO2
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangTooling
  clangToolingCore
  )

# Support plugins.
if(CLANG_PLUGIN_SUPPORT)
  set(support_plugins SUPPORT_PLUGINS)
endif()

add_clang_tool(o2-clang-tidy
  ClangTidyToolMain.cpp

  DEPENDS
  ${support_plugins}
  )
target_link_libraries(o2-clang-tidy
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangTooling
  clangToolingCore
  )
target_link_libraries(o2-clang-tidy
  PRIVATE
  clangTidyO2
  clangTidyMainO2
  ${ALL_CLANG_TIDY_CHECKS}
  )

if(CLANG_PLUGIN_SUPPORT)
  export_executable_symbols_for_plugins(o2-clang-tidy)
endif()

install(PROGRAMS clang-tidy-diff.py
  DESTINATION "${CMAKE_INSTALL_DATADIR}/clang"
  COMPONENT o2-clang-tidy)
install(PROGRAMS run-clang-tidy.py
  DESTINATION "${CMAKE_INSTALL_BINDIR}"
  COMPONENT o2-clang-tidy
  RENAME run-clang-tidy)

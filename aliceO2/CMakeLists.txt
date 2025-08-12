add_library(clangTidyAliceO2Module MODULE "")
target_compile_options(clangTidyAliceO2Module PRIVATE -fno-rtti)
target_include_directories(clangTidyAliceO2Module
  PRIVATE
  ${CLANG_INCLUDE_DIRS}
  ${LLVM_INCLUDE_DIRS}
)

target_sources(clangTidyAliceO2Module
  PRIVATE 
  ${CMAKE_CURRENT_LIST_DIR}/AliceO2TidyModule.cpp
  ${CMAKE_CURRENT_LIST_DIR}/MemberNamesCheck.cpp
  ${CMAKE_CURRENT_LIST_DIR}/NamespaceNamingCheck.cpp
  ${CMAKE_CURRENT_LIST_DIR}/SizeofCheck.cpp
)
install(TARGETS clangTidyAliceO2Module LIBRARY DESTINATION lib)

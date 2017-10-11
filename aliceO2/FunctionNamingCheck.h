//===--- FunctionNamingCheck.h - clang-tidy----------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_FUNCTION_NAMING_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_FUNCTION_NAMING_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace aliceO2 {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/aliceO2-function-naming.html
class FunctionNamingCheck : public ClangTidyCheck {
public:
  FunctionNamingCheck(StringRef Name, ClangTidyContext *Context);
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  const std::string VALID_NAME_REGEX;
  std::string VALID_PATH_REGEX;

  bool isOutsideOfTargetScope(std::string filename);
  bool fixName(const std::string &qualifier, std::string &name);
  void logNameError(SourceLocation Loc, std::string name);
  bool isBaseMethodOutOfTargetScope(clang::SourceManager *const sourceManager,const CXXMethodDecl* methodDecl);
};

} // namespace aliceO2
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_FUNCTION_NAMING_H

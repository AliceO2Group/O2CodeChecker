//===--- TypeNamesNamingCheck.h - clang-tidy---------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_TYPE_NAMES_NAMING_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_TYPE_NAMES_NAMING_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace aliceO2 {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/aliceO2-type-names-naming.html
class TypeNamesNamingCheck : public ClangTidyCheck {
public:
  TypeNamesNamingCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  
  bool fixName(std::string &name);
  void logNameError(SourceLocation Loc, std::string name);
};

} // namespace aliceO2
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_TYPE_NAMES_NAMING_H

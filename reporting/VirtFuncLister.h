//===--- MemberNamesCheck.h - clang-tidy-------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_VIRTFUNCLISTER_NAMES_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_VIRTFUNCLISTER_NAMES_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace reporting {

/// A simple tool/check that given a ClassName reports the interfaces
/// used in the code base
///
class VirtFuncLister : public ClangTidyCheck {
private:
  ClangTidyContext * MyContext;
public:
  VirtFuncLister(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context), MyContext(Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace reporting
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_VIRTFUNCLISTER_NAMES_H

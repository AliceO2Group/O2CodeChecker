//===--- FooCheck.h - clang-tidy---------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_PLUGIN_FOO_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_PLUGIN_FOO_H

#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace plugin {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/plugin-Foo.html
class FooCheck : public ClangTidyCheck {
public:
  FooCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace plugin
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_PLUGIN_FOO_H

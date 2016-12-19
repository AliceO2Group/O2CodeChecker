//===--- MemberNamesCheck.h - clang-tidy-------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_MEMBER_NAMES_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_MEMBER_NAMES_H

#include "../ClangTidy.h"
#include <regex>

namespace clang {
namespace tidy {
namespace aliceO2 {

/// Checking the class member naming convention
///
class MemberNamesCheck : public ClangTidyCheck {
private:
  const std::string Pattern; // the regular expression string
  const std::regex Regex; // the regular expression class
public:
  MemberNamesCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context), Pattern(Options.get("Pattern","m[A-Z].*")), Regex(Pattern)
  {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override {
    Options.store(Opts, "Pattern", Pattern);
  }
};

} // namespace aliceO2
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_MEMBER_NAMES_H

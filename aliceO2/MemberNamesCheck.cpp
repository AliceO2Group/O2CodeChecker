//===--- MemberNamesCheck.cpp - clang-tidy---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MemberNamesCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <iostream>
#include <regex>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace aliceO2 {

void MemberNamesCheck::registerMatchers(MatchFinder *Finder) {
  // Our policy says that only class members need to start with m, not
  // struct members
  Finder->addMatcher((fieldDecl(hasParent(recordDecl(isClass())))).bind("field_decl1"), this);
}

void MemberNamesCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FieldDecl>("field_decl1");
  if (MatchedDecl) {
    // check that we are inside the AliceO2 namespace to exlude system stuff
    // FIXME: needs to be configurable
    // NOTE: AliceO2:: is the old one. We agreed to use o2::
    if ((MatchedDecl->getQualifiedNameAsString().find("AliceO2::") != 0)
        && (MatchedDecl->getQualifiedNameAsString().find("o2::") != 0))
      return;

    if (std::regex_match(MatchedDecl->getNameAsString(), Regex)) {
      return;
    }

    diag(MatchedDecl->getLocation(),
         "field declaration %0 does not match naming rule",
         DiagnosticIDs::Error)
        << MatchedDecl;
  }
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

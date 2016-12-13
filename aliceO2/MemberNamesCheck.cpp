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

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace aliceO2 {

void MemberNamesCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(fieldDecl().bind("field_decl1"), this);
}

void MemberNamesCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FieldDecl>("field_decl1");
  if(MatchedDecl) {
    // check that we are inside the AliceO2 namespace to exlude system stuff
    if ( MatchedDecl->getQualifiedNameAsString().find("AliceO2::") != 0 )
     return;

    if (MatchedDecl->getName().startswith("m")){
     return;
    }
    diag(MatchedDecl->getLocation(), "field declaration %0 does not match naming rule", DiagnosticIDs::Error)
    << MatchedDecl;
    //  << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "f");
  }
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

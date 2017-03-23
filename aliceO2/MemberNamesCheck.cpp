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
#include <string>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace aliceO2 {

void MemberNamesCheck::registerMatchers(MatchFinder *Finder) {
  // Our policy says that only class members need to start with m, not
  // struct members
  auto o2PatchableMember = matchesName("::[a-z][A-Z][^:]*$");
  auto o2InvalidMember = matchesName("::[^m][A-Z][^:]*$");
  auto namingRules = allOf(o2PatchableMember,o2InvalidMember);
  auto isAClass = hasParent(recordDecl(isClass()));
  auto field = fieldDecl(allOf(isAClass, namingRules));
  auto initialisers = forEachConstructorInitializer(cxxCtorInitializer(forField(field)).bind("member_initialiser1"));
  Finder->addMatcher(field.bind("field_decl1"), this);
  Finder->addMatcher(memberExpr(hasDeclaration(field)).bind("member_decl1"), this);
  Finder->addMatcher(cxxConstructorDecl(forEachConstructorInitializer(allOf(cxxCtorInitializer(isMemberInitializer()).bind("member_initialiser1"), forField(field)))), this);
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

    std::string newName(MatchedDecl->getNameAsString());
    newName[0] = 'm';
    diag(MatchedDecl->getLocation(),
         "field declaration %0 does not match naming rule",
         DiagnosticIDs::Error)
        << MatchedDecl
        << FixItHint::CreateReplacement(MatchedDecl->getLocation(), newName);
  }
  const auto *MatchedValue = Result.Nodes.getNodeAs<MemberExpr>("member_decl1");
  if (MatchedValue) {
    const auto *MatchedValueDecl = MatchedValue->getMemberDecl();
    // check that we are inside the AliceO2 namespace to exlude system stuff
    // FIXME: needs to be configurable
    // NOTE: AliceO2:: is the old one. We agreed to use o2::

    if ((MatchedValueDecl->getQualifiedNameAsString().find("AliceO2::") != 0)
        && (MatchedValueDecl->getQualifiedNameAsString().find("o2::") != 0))
      return;

    std::string newName(MatchedValueDecl->getNameAsString());
    newName[0] = 'm';
    diag(MatchedValue->getMemberLoc(),
         "field reference %0 does not match naming rule",
         DiagnosticIDs::Error)
        << MatchedValueDecl
        << FixItHint::CreateReplacement(MatchedValue->getMemberLoc(), newName);
  }
  const auto *MatchedInitializer = Result.Nodes.getNodeAs<CXXCtorInitializer>("member_initialiser1");
  if (MatchedInitializer) {
    if (!MatchedInitializer->isWritten())
      return;
    const auto *MatchedValueDecl = MatchedInitializer->getMember();
    // check that we are inside the AliceO2 namespace to exlude system stuff
    // FIXME: needs to be configurable
    // NOTE: AliceO2:: is the old one. We agreed to use o2::

    if ((MatchedValueDecl->getQualifiedNameAsString().find("AliceO2::") != 0)
        && (MatchedValueDecl->getQualifiedNameAsString().find("o2::") != 0))
      return;

    std::string newName(MatchedValueDecl->getNameAsString());
    newName[0] = 'm';
    diag(MatchedInitializer->getMemberLocation(),
         "initialiser %0 does not match naming rule",
         DiagnosticIDs::Error)
        << MatchedValueDecl
        << FixItHint::CreateReplacement(MatchedInitializer->getMemberLocation(), newName);
  }
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

//===--- NamespaceNamingCheck.cpp - clang-tidy-----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "NamespaceNamingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <regex>
#include <string>
#include <ctype.h>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace aliceO2 {

void NamespaceNamingCheck::registerMatchers(MatchFinder *Finder) {
  auto validNameMatch = matchesName("::([a-z]+_)*[a-z]+$");
  
  // matches namespace declarations that have invalid name
  Finder->addMatcher(namespaceDecl( unless( validNameMatch ) ).bind("namespace-decl"), this);
  // matches usage of namespace
  Finder->addMatcher(nestedNameSpecifierLoc(loc(nestedNameSpecifier(specifiesNamespace(
    unless( validNameMatch ) )))).bind("namespace-usage"), this );
}

void NamespaceNamingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedNamespaceDecl = Result.Nodes.getNodeAs<NamespaceDecl>("namespace-decl");
  if( MatchedNamespaceDecl )
  {
    std::string newName(MatchedNamespaceDecl->getDeclName().getAsString());
    
    fixNamespaceName(newName);
    
    diag(MatchedNamespaceDecl->getLocation(), "namespace %0 does not follow the underscore convention")
        << MatchedNamespaceDecl
        << FixItHint::CreateReplacement(MatchedNamespaceDecl->getLocation(), newName);
  }
  
  const auto *MatchedNamespaceLoc = Result.Nodes.getNodeAs<NestedNameSpecifierLoc>("namespace-usage");
  if( MatchedNamespaceLoc )
  {
    const auto *AsNamespace = MatchedNamespaceLoc->getNestedNameSpecifier()->getAsNamespace();
    std::string newName(AsNamespace->getDeclName().getAsString());
    
    fixNamespaceName(newName);
    
    diag(MatchedNamespaceLoc->getLocalBeginLoc(), "namespace %0 does not follow the underscore convention")
        << AsNamespace
        << FixItHint::CreateReplacement(MatchedNamespaceLoc->getLocalBeginLoc(), newName);
  }
}

void NamespaceNamingCheck::fixNamespaceName(std::string &name)
{
  for(int i=0; i<name.size(); i++) {
    if(isupper(name[i])) {
      if(i != 0 && name[i-1] != '_') {
        name.insert(i, "_");
        i++;
      }
      name[i] = tolower(name[i]);
    }
  }
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

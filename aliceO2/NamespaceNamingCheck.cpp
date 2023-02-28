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
#include "clang/ASTMatchers/ASTMatchersMacros.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <regex>
#include <string>
#include <ctype.h>
 
using namespace clang::ast_matchers;

namespace clang {
namespace ast_matchers {
AST_MATCHER_P(UsingDirectiveDecl, nominatedNamespace,
              internal::Matcher<NamespaceDecl>, InnerMatcher)
{
  return InnerMatcher.matches(*Node.getNominatedNamespace(), Finder, Builder);
}
} // namespace ast_matchers
namespace tidy {
namespace aliceO2 {
 
const std::string VALID_NAME_REGEX = "[a-z][a-z_0-9]+";
std::string VALID_PATH_REGEX = "";

NamespaceNamingCheck::NamespaceNamingCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context)
{
  VALID_PATH_REGEX = Options.getLocalOrGlobal("CheckPathRegex","");
	if( VALID_PATH_REGEX == "" )
	{
	  fprintf(stderr,"Error: User must provide a .clang-tidy file in any of the parent directories of the source files containing a key-value pair for key \'CheckPathRegex\' "
	                        "or pass the key-value pair with the command line argument --config");
	  exit(1);
	}
}

bool isOutsideOfTargetScope(std::string filename)
{
  return !std::regex_match(filename, std::regex(VALID_PATH_REGEX));
}

void NamespaceNamingCheck::registerMatchers(MatchFinder *Finder) {
  const auto validNameMatch = matchesName( std::string("::") + VALID_NAME_REGEX + "$" );
  const auto inO2NSMatch = matchesName("^::o2::");

  // matches namespace declarations that have invalid name
  Finder->addMatcher(namespaceDecl(allOf(
    inO2NSMatch,
    unless(validNameMatch),
    unless(isAnonymous())
    )).bind("namespace-decl"), this);
  // matches usage of namespace
  Finder->addMatcher(nestedNameSpecifierLoc(loc(nestedNameSpecifier(specifiesNamespace(allOf(
    inO2NSMatch,
    unless(validNameMatch)
    ))))).bind("namespace-usage"), this);
  // matches "using namespace" declarations
  Finder->addMatcher(usingDirectiveDecl(allOf(
    unless(isImplicit()),
    nominatedNamespace(allOf(
      inO2NSMatch,
      unless(validNameMatch)
    )))).bind("using-namespace"), this);
}

void NamespaceNamingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedNamespaceDecl = Result.Nodes.getNodeAs<NamespaceDecl>("namespace-decl");
  if( MatchedNamespaceDecl )
  {
    if( isOutsideOfTargetScope( Result.SourceManager->getFilename(MatchedNamespaceDecl->getLocation()).str() ) )
    {
      return;
    }
    std::string newName(MatchedNamespaceDecl->getDeclName().getAsString());
    std::string oldName=newName;

    if(fixNamespaceName(newName))
    {
      diag(MatchedNamespaceDecl->getLocation(), "namespace %q0 does not follow the underscore convention")
        << MatchedNamespaceDecl
        << FixItHint::CreateReplacement(MatchedNamespaceDecl->getLocation(), newName);
    }
    else
    {
      logNameError(MatchedNamespaceDecl->getLocation(), oldName);
    }
  }

  const auto *MatchedNamespaceLoc = Result.Nodes.getNodeAs<NestedNameSpecifierLoc>("namespace-usage");
  if( MatchedNamespaceLoc )
  {
    const auto *AsNamespace = MatchedNamespaceLoc->getNestedNameSpecifier()->getAsNamespace();
    if( isOutsideOfTargetScope( Result.SourceManager->getFilename(AsNamespace->getLocation()).str() ) )
    {
      return;
    }
    std::string newName(AsNamespace->getDeclName().getAsString());
    std::string oldName=newName;

    if(fixNamespaceName(newName))
    {
      diag(MatchedNamespaceLoc->getLocalBeginLoc(), "namespace %q0 does not follow the underscore convention")
        << AsNamespace
        << FixItHint::CreateReplacement(MatchedNamespaceLoc->getLocalBeginLoc(), newName);
    }
    else
    {
      logNameError(MatchedNamespaceLoc->getLocalBeginLoc(), oldName);
    }
  }

  const auto *MatchedUsingNamespace = Result.Nodes.getNodeAs<UsingDirectiveDecl>("using-namespace");
  if( MatchedUsingNamespace )
  {
    if( isOutsideOfTargetScope( Result.SourceManager->getFilename(MatchedUsingNamespace->getNominatedNamespace()->getLocation()).str() ) )
    {
      return;
    }
    std::string newName(MatchedUsingNamespace->getNominatedNamespace()->getDeclName().getAsString());
    std::string oldName=newName;

    if(fixNamespaceName(newName))
    {
      diag(MatchedUsingNamespace->getLocation(), "namespace %q0 does not follow the underscore convention")
        << MatchedUsingNamespace->getNominatedNamespace()
        << FixItHint::CreateReplacement(MatchedUsingNamespace->getLocation(), newName);
    }
    else
    {
      logNameError(MatchedNamespaceDecl->getLocation(), oldName);
    }
  }
}

bool NamespaceNamingCheck::fixNamespaceName(std::string &name)
{
  std::string replace_option = Options.get(name, "").str();
  if( replace_option != "" )
  {
    name = replace_option;
    return true;
  }

  for(int i=name.size()-1; i>=0; i--) {
    if(isupper(name[i])) {
      if(i != 0 && islower(name[i-1])) {
        name.insert(i, "_");
        i++;
      }
      name[i] = tolower(name[i]);
    }
  }

  return std::regex_match( name, std::regex(VALID_NAME_REGEX) );
}

void NamespaceNamingCheck::logNameError(SourceLocation Loc, std::string errorName)
{
  diag(Loc, "Could not fix \'%0\'", DiagnosticIDs::Level::Error)
      << errorName;
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

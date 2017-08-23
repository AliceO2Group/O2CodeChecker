//===--- VariableNamingGlobalCheck.cpp - clang-tidy------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "VariableNamingGlobalCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <regex>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace aliceO2 {

const std::string VALID_NAME_REGEX = "g[A-Z].+";

void VariableNamingGlobalCheck::registerMatchers(MatchFinder *Finder) {
  const auto validNameMatch = matchesName( std::string("::") + VALID_NAME_REGEX + "$" );
  const auto invalidGlobalVarDecl = varDecl(allOf(
    isConstexpr(),
    unless(validNameMatch)
    ));

  Finder->addMatcher(invalidGlobalVarDecl.bind("global-var-decl"), this);
  Finder->addMatcher(declRefExpr(hasDeclaration(invalidGlobalVarDecl)).bind("global-var-static-usage"), this);
  Finder->addMatcher(memberExpr(hasDeclaration(invalidGlobalVarDecl)).bind("global-var-instance-usage"), this);  
}

void VariableNamingGlobalCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedGlobalVarDecl = Result.Nodes.getNodeAs<VarDecl>("static-member-decl");
  if( MatchedGlobalVarDecl )
  {
    std::string oldName = MatchedGlobalVarDecl->getDeclName().getAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedGlobalVarDecl->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());
    
    bool fixed = fixName(qualifier, newName);
    
    if( fixed && (newName != oldName) )
    {
      diag(MatchedGlobalVarDecl->getLocation(), "global variable \'%0\' must be prefixed by \'g\'")
          << oldName
          << FixItHint::CreateReplacement(MatchedGlobalVarDecl->getLocation(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedGlobalVarDecl->getLocation(), qualifier + oldName);
    }    
  }
  const auto *MatchedGlobalVarStaticUsage = Result.Nodes.getNodeAs<DeclRefExpr>("static-member-static-usage");
  if( MatchedGlobalVarStaticUsage )
  {
    std::string oldName = MatchedGlobalVarStaticUsage->getDecl()->getNameAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedGlobalVarStaticUsage->getDecl()->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());
    
    bool fixed = fixName(qualifier, newName);
    
    if( fixed && (newName != oldName) )
    {
      diag(MatchedGlobalVarStaticUsage->getLocation(), "global variable \'%0\' must be prefixed by \'g\'")
          << oldName
          << FixItHint::CreateReplacement(MatchedGlobalVarStaticUsage->getLocation(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedGlobalVarStaticUsage->getLocation(), qualifier + oldName);
    }    
  }
  const auto *MatchedGlobalVarInstanceUsage = Result.Nodes.getNodeAs<MemberExpr>("static-member-instance-usage");
  if( MatchedGlobalVarInstanceUsage )
  {
    std::string oldName = MatchedGlobalVarInstanceUsage->getMemberDecl()->getNameAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedGlobalVarInstanceUsage->getMemberDecl()->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());
    
    bool fixed = fixName(qualifier, newName);
    
    if( fixed && (newName != oldName) )
    {
      diag(MatchedGlobalVarInstanceUsage->getExprLoc(), "global variable \'%0\' must be prefixed by \'g\'")
          << oldName
          << FixItHint::CreateReplacement(MatchedGlobalVarInstanceUsage->getExprLoc(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedGlobalVarInstanceUsage->getExprLoc(), qualifier + oldName);
    }    
  }
}

/// This function modifies the input string to one of the following ways:
/// - Into a CamelCase string.
/// - Into a string that is specified in the config options.
///
/// return: Returns true if the input string is succesfully modified,
///         If neither of the modifying options succeeds false is returned.
bool VariableNamingGlobalCheck::fixName(const std::string &qualifier, std::string &name)
{
  std::string replace_option = Options.get(qualifier + name, "");
  if( replace_option != "" )
  {
    // default fix string
    if( replace_option == "<provide_fix_for_key>" )
    {
      return false;
    }
    
    name = replace_option;
    return true;
  }
  
  name[0] = toupper(name[0]);
  name.insert(0, "g");
  
  return std::regex_match( name, std::regex(VALID_NAME_REGEX) );
}


void VariableNamingGlobalCheck::logNameError(SourceLocation Loc, std::string errorName)
{
  diag(Loc, "Could not fix '%0'", DiagnosticIDs::Level::Error)
      << errorName;
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

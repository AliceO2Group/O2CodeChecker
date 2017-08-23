//===--- VariableNamingConstexprCheck.cpp - clang-tidy---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "VariableNamingConstexprCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <regex>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace aliceO2 {

const std::string VALID_NAME_REGEX = "[A-Z]+(_[A-Z]+)*";

void VariableNamingConstexprCheck::registerMatchers(MatchFinder *Finder) {
  const auto validNameMatch = matchesName( std::string("::") + VALID_NAME_REGEX + "$" );
  const auto invalidConstexprDecl = varDecl(allOf(
    isConstexpr(),
    unless(validNameMatch)
    ));

  Finder->addMatcher(invalidConstexprDecl.bind("constexpr-decl"), this);
  Finder->addMatcher(declRefExpr(hasDeclaration(invalidConstexprDecl)).bind("constexpr-static-usage"), this);
  Finder->addMatcher(memberExpr(hasDeclaration(invalidConstexprDecl)).bind("constexpr-instance-usage"), this);
}

void VariableNamingConstexprCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedConstexprDecl = Result.Nodes.getNodeAs<VarDecl>("constexpr-decl");
  if( MatchedConstexprDecl )
  {
    std::string oldName = MatchedConstexprDecl->getDeclName().getAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedConstexprDecl->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());
    
    bool fixed = fixName(qualifier, newName);
    
    if( fixed && (newName != oldName) )
    {
      diag(MatchedConstexprDecl->getLocation(), "constexpr variable \'%0\' must be CAPITALIZED")
          << oldName
          << FixItHint::CreateReplacement(MatchedConstexprDecl->getLocation(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedConstexprDecl->getLocation(), qualifier + oldName);
    }    
  }
  const auto *MatchedConstexprStaticUsage = Result.Nodes.getNodeAs<DeclRefExpr>("constexpr-static-usage");
  if( MatchedConstexprStaticUsage )
  {
    std::string oldName = MatchedConstexprStaticUsage->getDecl()->getNameAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedConstexprStaticUsage->getDecl()->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());
    
    bool fixed = fixName(qualifier, newName);
    
    if( fixed && (newName != oldName) )
    {
      diag(MatchedConstexprStaticUsage->getLocation(), "constexpr variable \'%0\' must be CAPITALIZED")
          << oldName
          << FixItHint::CreateReplacement(MatchedConstexprStaticUsage->getLocation(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedConstexprStaticUsage->getLocation(), qualifier + oldName);
    }    
  }
  const auto *MatchedConstexprInstanceUsage = Result.Nodes.getNodeAs<MemberExpr>("constexpr-instance-usage");
  if( MatchedConstexprInstanceUsage )
  {
    std::string oldName = MatchedConstexprInstanceUsage->getMemberDecl()->getNameAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedConstexprInstanceUsage->getMemberDecl()->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());
    
    bool fixed = fixName(qualifier, newName);
    
    if( fixed && (newName != oldName) )
    {
      diag(MatchedConstexprInstanceUsage->getExprLoc(), "constexpr variable \'%0\' must be CAPITALIZED")
          << oldName
          << FixItHint::CreateReplacement(MatchedConstexprInstanceUsage->getExprLoc(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedConstexprInstanceUsage->getExprLoc(), qualifier + oldName);
    }    
  }
}

/// This function modifies the input string to one of the following ways:
/// - Into a CamelCase string.
/// - Into a string that is specified in the config options.
///
/// return: Returns true if the input string is succesfully modified,
///         If neither of the modifying options succeeds false is returned.
bool VariableNamingConstexprCheck::fixName(const std::string &qualifier, std::string &name)
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
  
  for(int i=0;i<name.size();i++)
  {
    if(i!=0 && isupper(name[i]) && name[i-1]!='_')
    {
      name.insert(i, "_");
      i++;
    }
    name[i] = toupper(name[i]);
  }
  
  return std::regex_match( name, std::regex(VALID_NAME_REGEX) );
}

void VariableNamingConstexprCheck::logNameError(SourceLocation Loc, std::string errorName)
{
  diag(Loc, "Could not fix '%0'", DiagnosticIDs::Level::Error)
      << errorName;
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

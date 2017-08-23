//===--- VariableNamingStaticMemberCheck.cpp - clang-tidy------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "VariableNamingStaticMemberCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <regex>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace aliceO2 {

const std::string VALID_NAME_REGEX = "s[A-Z].+";

void VariableNamingStaticMemberCheck::registerMatchers(MatchFinder *Finder) {
  const auto validNameMatch = matchesName( std::string("::") + VALID_NAME_REGEX + "$" );
  const auto invalidStaticMemberDecl = varDecl(allOf(
    hasParent(cxxRecordDecl()),
    hasStaticStorageDuration(),
    unless(validNameMatch)
    ));

  Finder->addMatcher(invalidStaticMemberDecl.bind("static-member-decl"), this);
  Finder->addMatcher(declRefExpr(hasDeclaration(invalidStaticMemberDecl)).bind("static-member-static-usage"), this);
  Finder->addMatcher(memberExpr(hasDeclaration(invalidStaticMemberDecl)).bind("static-member-instance-usage"), this);
}

void VariableNamingStaticMemberCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedStaticMemberDecl = Result.Nodes.getNodeAs<VarDecl>("static-member-decl");
  if( MatchedStaticMemberDecl )
  {
    std::string oldName = MatchedStaticMemberDecl->getDeclName().getAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedStaticMemberDecl->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());
    
    bool fixed = fixName(qualifier, newName);
    
    if( fixed && (newName != oldName) )
    {
      diag(MatchedStaticMemberDecl->getLocation(), "static member variable \'%0\' must be prefixed by \'s\'")
          << oldName
          << FixItHint::CreateReplacement(MatchedStaticMemberDecl->getLocation(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedStaticMemberDecl->getLocation(), qualifier + oldName);
    }    
  }
  const auto *MatchedStaticMemberStaticUsage = Result.Nodes.getNodeAs<DeclRefExpr>("static-member-static-usage");
  if( MatchedStaticMemberStaticUsage )
  {
    std::string oldName = MatchedStaticMemberStaticUsage->getDecl()->getNameAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedStaticMemberStaticUsage->getDecl()->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());
    
    bool fixed = fixName(qualifier, newName);
    
    if( fixed && (newName != oldName) )
    {
      diag(MatchedStaticMemberStaticUsage->getLocation(), "static member variable \'%0\' must be prefixed by \'s\'")
          << oldName
          << FixItHint::CreateReplacement(MatchedStaticMemberStaticUsage->getLocation(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedStaticMemberStaticUsage->getLocation(), qualifier + oldName);
    }    
  }
  const auto *MatchedStaticMemberInstanceUsage = Result.Nodes.getNodeAs<MemberExpr>("static-member-instance-usage");
  if( MatchedStaticMemberInstanceUsage )
  {
    std::string oldName = MatchedStaticMemberInstanceUsage->getMemberDecl()->getNameAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedStaticMemberInstanceUsage->getMemberDecl()->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());
    
    bool fixed = fixName(qualifier, newName);
    
    if( fixed && (newName != oldName) )
    {
      diag(MatchedStaticMemberInstanceUsage->getExprLoc(), "static member variable \'%0\' must be prefixed by \'s\'")
          << oldName
          << FixItHint::CreateReplacement(MatchedStaticMemberInstanceUsage->getExprLoc(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedStaticMemberInstanceUsage->getExprLoc(), qualifier + oldName);
    }    
  }
}

/// This function modifies the input string to one of the following ways:
/// - Into a CamelCase string.
/// - Into a string that is specified in the config options.
///
/// return: Returns true if the input string is succesfully modified,
///         If neither of the modifying options succeeds false is returned.
bool VariableNamingStaticMemberCheck::fixName(const std::string &qualifier, std::string &name)
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
  name.insert(0,"s");
  
  return std::regex_match( name, std::regex(VALID_NAME_REGEX) );
}


void VariableNamingStaticMemberCheck::logNameError(SourceLocation Loc, std::string errorName)
{
  diag(Loc, "Could not fix '%0'", DiagnosticIDs::Level::Error)
      << errorName;
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

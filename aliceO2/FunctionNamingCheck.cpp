//===--- FunctionNamingCheck.cpp - clang-tidy------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "FunctionNamingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <regex>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace aliceO2 {

const std::string VALID_NAME_REGEX = "[a-z]+([A-Z][a-z]+)*[0-9]*";

void FunctionNamingCheck::registerMatchers(MatchFinder *Finder) {
  const auto validNameMatch = matchesName( std::string("::") + VALID_NAME_REGEX + "$" );
  const auto invalidFunctionDecl = functionDecl(allOf(
    unless(isImplicit()),
    anyOf(
      unless(validNameMatch),
      allOf(returns(booleanType()), unless(matchesName("::(is|has).+$")))
    )));
  
  Finder->addMatcher(invalidFunctionDecl.bind("function-decl"), this);
  Finder->addMatcher(callExpr(allOf(
    hasDeclaration(invalidFunctionDecl),
    unless(hasDescendant(memberExpr()))
    )).bind("function-call"), this);
    
  Finder->addMatcher(cxxMemberCallExpr(hasDeclaration(cxxMethodDecl(anyOf(
    unless(validNameMatch),
    allOf(returns(booleanType()), unless(matchesName("::(is|has).+$")))  
    )))).bind("member-call"), this);
}

void FunctionNamingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedFunctionDecl = Result.Nodes.getNodeAs<FunctionDecl>("function-decl");
  if( MatchedFunctionDecl )
  {
    std::string oldName = MatchedFunctionDecl->getDeclName().getAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedFunctionDecl->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());
    
    bool fixed = fixName(qualifier, newName);
    bool isBooleanType = MatchedFunctionDecl->getReturnType().getTypePtr()->isBooleanType();
    bool hasValidPrefix = newName.substr(0,2)=="is" || newName.substr(0,3)=="has";
    
    if( !(isBooleanType && !hasValidPrefix) && fixed && (newName != oldName) )
    {
      if( isBooleanType )
      {
        diag(MatchedFunctionDecl->getLocation(), "function that returns bool \'%0\' must be prefixed with \'is\' or \'has\'")
            << oldName
            << FixItHint::CreateReplacement(MatchedFunctionDecl->getLocation(), newName);
      }
      else
      {
        diag(MatchedFunctionDecl->getLocation(), "function \'%0\' does not follow the naming convention")
            << oldName
            << FixItHint::CreateReplacement(MatchedFunctionDecl->getLocation(), newName);
      }
    }
    else if( (isBooleanType && !hasValidPrefix) || !fixed )
    {
      logNameError(MatchedFunctionDecl->getLocation(), qualifier + oldName);
    }    
  }
  const auto *MatchedFunctionCall = Result.Nodes.getNodeAs<CallExpr>("function-call");
  if( MatchedFunctionCall )
  {
    const auto *AsFunctionDecl = MatchedFunctionCall->getDirectCallee();
    std::string oldName = AsFunctionDecl->getDeclName().getAsString();
    std::string newName = oldName;
    std::string qualifier = AsFunctionDecl->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());
    
    bool fixed = fixName(qualifier, newName);
    bool isBooleanType = AsFunctionDecl->getReturnType().getTypePtr()->isBooleanType();
    bool hasValidPrefix = newName.substr(0,2)=="is" || newName.substr(0,3)=="has";
    const auto LocStart = MatchedFunctionCall->getLocStart().getLocWithOffset( qualifier.size() );
    
    if( !(isBooleanType && !hasValidPrefix) && fixed && (newName != oldName) )
    {
      const std::string message = ( isBooleanType ? 
        "function that returns bool \'%0\' must be prefixed with \'is\' or \'has\'" : 
        "function \'%0\' does not follow the naming convention");

      diag(LocStart, message.c_str())
          << oldName
          << FixItHint::CreateReplacement(LocStart, newName);
    }
    else if( (isBooleanType && !hasValidPrefix) || !fixed )
    {
      logNameError(LocStart, qualifier + oldName);
    }    
  }
  const auto *MatchedMemberExpr = Result.Nodes.getNodeAs<CXXMemberCallExpr>("member-call");
  if( MatchedMemberExpr )
  {
    std::string oldName = MatchedMemberExpr->getMethodDecl()->getDeclName().getAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedMemberExpr->getMethodDecl()->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());

    bool fixed = fixName(qualifier, newName);
    bool isBooleanType = MatchedMemberExpr->getMethodDecl()->getReturnType().getTypePtr()->isBooleanType();
    bool hasValidPrefix = newName.substr(0,2)=="is" || newName.substr(0,3)=="has";
    const auto LocStart = MatchedMemberExpr->getExprLoc();

    if( !(isBooleanType && !hasValidPrefix) && fixed && (newName != oldName) )
    {
      const std::string message = ( isBooleanType ? 
        "function that returns bool \'%0\' must be prefixed with \'is\' or \'has\'" : 
        "function \'%0\' does not follow the naming convention");
        
      diag(LocStart, message.c_str())
          << oldName
          << FixItHint::CreateReplacement(LocStart, newName);
    }
    else if( (isBooleanType && !hasValidPrefix) || !fixed )
    {
      logNameError(LocStart, qualifier + oldName);
    }
  }
}

/// This function modifies the input string to one of the following ways:
/// - Into a CamelCase string.
/// - Into a string that is specified in the config options.
///
/// return: Returns true if the input string is succesfully modified,
///         If neither of the modifying options succeeds false is returned.
bool FunctionNamingCheck::fixName(const std::string &qualifier, std::string &name)
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
  
  if( isupper(name[0]) )
  {
    name[0] = tolower(name[0]);
  }
  for(int i = 1;i < name.size();i++)
  {
    if( name[i] == '_' ) {
      if( i < name.size() - 1 )
      {
        name[i + 1] = toupper(name[i + 1]);
      }
      name.erase(i, 1); // delete '_'
    }
  }
  
  return std::regex_match( name, std::regex(VALID_NAME_REGEX) );
}

void FunctionNamingCheck::logNameError(SourceLocation Loc, std::string errorName)
{
  diag(Loc, "Could not fix '%0'", DiagnosticIDs::Level::Error)
      << errorName;
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

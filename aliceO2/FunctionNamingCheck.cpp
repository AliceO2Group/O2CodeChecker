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

FunctionNamingCheck::FunctionNamingCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context)
    ,VALID_NAME_REGEX("[a-z]+[a-zA-Z0-9]*")
    ,VALID_PATH_REGEX("")
{
  VALID_PATH_REGEX = Options.getLocalOrGlobal("CheckPathRegex","");
  if( VALID_PATH_REGEX == "" )
  {
    fprintf(stderr,"Error: User must provide a .clang-tidy file in any of the parent directories of the source files containing a key-value pair for key \'CheckPathRegex\' "
                          "or pass the key-value pair with the command line argument --config");
    exit(1);
  }
}

bool FunctionNamingCheck::isOutsideOfTargetScope(std::string filename)
{
  return !std::regex_match(filename, std::regex(VALID_PATH_REGEX));
}

void FunctionNamingCheck::registerMatchers(MatchFinder *Finder) {
  const auto validNameMatch = matchesName( std::string("::") + VALID_NAME_REGEX + "$" );
  const auto isOperator = matchesName("::operator.*$");

  const auto invalidFunctionDecl = allOf(
    unless(cxxConstructorDecl()),
    unless(cxxDestructorDecl()),
    unless(cxxMethodDecl(isOverride())),
    functionDecl(allOf(
      unless(isImplicit()),
      unless(isOperator),
      unless(validNameMatch)
      ))
    );

  Finder->addMatcher(functionDecl(invalidFunctionDecl).bind("function-decl"), this);
  Finder->addMatcher(declRefExpr(hasDeclaration(
    invalidFunctionDecl
    )).bind("function-call"), this);

  Finder->addMatcher(callExpr(allOf(
    unless(cxxOperatorCallExpr()), // for not to match operators
    cxxMemberCallExpr(hasDeclaration(cxxMethodDecl(allOf(
      unless(isOverride()),
      unless(cxxConstructorDecl()),
      unless(cxxDestructorDecl()),
      unless(isOperator),
      unless(validNameMatch)
      ))))
    )).bind("member-call"), this);
}

void FunctionNamingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedFunctionDecl = Result.Nodes.getNodeAs<FunctionDecl>("function-decl");
  if( MatchedFunctionDecl )
  {
    if( isOutsideOfTargetScope( Result.SourceManager->getFilename(MatchedFunctionDecl->getLocation()).str() ) )
    {
      return;
    }

    const auto *methodDecl = static_cast<const CXXMethodDecl*>(MatchedFunctionDecl);
    if(methodDecl && isBaseMethodOutOfTargetScope(Result.SourceManager, methodDecl))
    {
      return;
    }

    std::string oldName = MatchedFunctionDecl->getDeclName().getAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedFunctionDecl->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());

    const bool fixed = fixName(qualifier, newName);

    std::string message = "function \'%0\' does not follow the naming convention";

    if( fixed && (newName != oldName) )
    {
      diag(MatchedFunctionDecl->getLocation(), message.c_str())
          << oldName
          << FixItHint::CreateReplacement(MatchedFunctionDecl->getLocation(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedFunctionDecl->getLocation(),
        qualifier + oldName,
        message.replace(message.find("%0"), 2, oldName));
    }
  }
  const auto *MatchedTFunctionCall = Result.Nodes.getNodeAs<UnresolvedLookupExpr>("function-call");
  if( MatchedTFunctionCall )
  {
    if( MatchedTFunctionCall->getName()
          .getAsString()
          .substr(0, std::string("operator").size()) == "operator" )
    {
      return;
    }

    if( isOutsideOfTargetScope(
          Result.SourceManager->getFilename(MatchedTFunctionCall->getLocStart()).str()) ||
        isOutsideOfTargetScope(
          Result.SourceManager->getFilename(MatchedTFunctionCall->getQualifierLoc().getBeginLoc()).str()) ||
        (MatchedTFunctionCall->getQualifier() && (
          (MatchedTFunctionCall->getQualifier()->getAsNamespace()
            && isOutsideOfTargetScope(Result.SourceManager->getFilename(MatchedTFunctionCall->getQualifier()->getAsNamespace()->getLocStart()))) ||
          (MatchedTFunctionCall->getQualifier()->getAsRecordDecl()
            && isOutsideOfTargetScope(Result.SourceManager->getFilename(MatchedTFunctionCall->getQualifier()->getAsRecordDecl()->getLocStart())))
          ))
      )
    {
      return;
    }
    std::string oldName = MatchedTFunctionCall->getName().getAsString();
    std::string newName = oldName;
    std::string qualifier = "";
    if(MatchedTFunctionCall->getQualifier())
    {
      if(MatchedTFunctionCall->getQualifier()->getAsNamespace())
      {
        qualifier = MatchedTFunctionCall->getQualifier()->getAsNamespace()->getNameAsString();
      }
      else if(MatchedTFunctionCall->getQualifier()->getAsRecordDecl())
      {
        qualifier = MatchedTFunctionCall->getQualifier()->getAsRecordDecl()->getNameAsString();
      }
    }

    const bool fixed = fixName(qualifier, newName);

    const auto LocStart = MatchedTFunctionCall->getNameLoc();
    std::string message = "function \'%0\' does not follow the naming convention";

    if( fixed && (newName != oldName) )
    {
      diag(LocStart, message.c_str())
          << oldName
          << FixItHint::CreateReplacement(LocStart, newName);
    }
    else if( !fixed )
    {
      logNameError(LocStart,
        qualifier + oldName,
        message.replace(message.find("%0"), 2, oldName));
    }
  }
  const auto *MatchedFunctionCall = Result.Nodes.getNodeAs<DeclRefExpr>("function-call");
  if( MatchedFunctionCall )
  {
    //fprintf(stderr, "Found funciton: %s\n", MatchedFunctionCall->getFoundDecl()->getNameAsString().c_str());

    if( isOutsideOfTargetScope(
          Result.SourceManager->getFilename(MatchedFunctionCall->getLocation()).str()) ||
        isOutsideOfTargetScope(
          Result.SourceManager->getFilename(MatchedFunctionCall->getDecl()->getLocation()).str())
      )
    {
      return;
    }
    std::string oldName = MatchedFunctionCall->getFoundDecl()->getNameAsString();
    std::string newName = oldName;
    std::string qualifier = "";
    if(MatchedFunctionCall->getQualifier())
    {
      qualifier = MatchedFunctionCall->getQualifier()->getAsIdentifier() ?
        MatchedFunctionCall->getQualifier()->getAsIdentifier()->getName().str() : "";
    }

    const bool fixed = fixName(qualifier, newName);

    const auto LocStart = MatchedFunctionCall->getQualifier() ?
      MatchedFunctionCall->getQualifierLoc().getEndLoc().getLocWithOffset(2) : MatchedFunctionCall->getLocStart();
    std::string message = "function \'%0\' does not follow the naming convention";

    if( fixed && (newName != oldName) )
    {
      diag(LocStart, message.c_str())
          << oldName
          << FixItHint::CreateReplacement(LocStart, newName); 
    }
    else if( !fixed )
    {
      logNameError(LocStart,
        qualifier + oldName,
        message.replace(message.find("%0"), 2, oldName));
    } 
  }

  const auto *MatchedMemberExpr = Result.Nodes.getNodeAs<CXXMemberCallExpr>("member-call");
  if( MatchedMemberExpr )
  {
    if( isOutsideOfTargetScope(
          Result.SourceManager->getFilename(MatchedMemberExpr->getLocStart()).str()) ||
        isOutsideOfTargetScope(
          Result.SourceManager->getFilename(MatchedMemberExpr->getMethodDecl()->getLocation()).str())
      )
    {
      return;
    }
    std::string oldName = MatchedMemberExpr->getMethodDecl()->getDeclName().getAsString();
    std::string newName = oldName;
    std::string qualifier = MatchedMemberExpr->getMethodDecl()->getQualifiedNameAsString();
    qualifier = qualifier.substr(0, qualifier.size() - oldName.size());

    const bool fixed = fixName(qualifier, newName);

    const auto LocStart = MatchedMemberExpr->getExprLoc();
    std::string message = "function \'%0\' does not follow the naming convention";

    if( fixed && (newName != oldName) )
    {
      diag(LocStart, message.c_str())
          << oldName
          << FixItHint::CreateReplacement(LocStart, newName);
    }
    else if( !fixed )
    {
      logNameError(LocStart,
        qualifier + oldName,
        message.replace(message.find("%0"), 2, oldName));
    }
  }
}

/// This function modifies the input string to one of the following ways:
/// - Into a camelCase string.
/// - Into a string that is specified in the config options.
///
/// return: Returns true if the input string is succesfully modified,
///         If neither of the modifying options succeeds false is returned.
bool FunctionNamingCheck::fixName(const std::string &qualifier, std::string &name)
{
  std::string replace_option = Options.get(name, "");
  if( replace_option != "" )
  {
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

void FunctionNamingCheck::logNameError(SourceLocation Loc, std::string errorName, std::string reason)
{
  diag(Loc, "Could not fix '%0'", DiagnosticIDs::Level::Error)
      << errorName;
}

bool FunctionNamingCheck::isBaseMethodOutOfTargetScope(clang::SourceManager* const sourceManager,const CXXMethodDecl* methodDecl)
{
  if(methodDecl->size_overridden_methods() == 0)
  {
    return false;
  }
  // getting the base method decl
  auto methodDeclBase = methodDecl->end_overridden_methods();
  methodDeclBase--; // getting the last overriden method

  return isOutsideOfTargetScope( sourceManager->getFilename((*methodDeclBase)->getLocation()).str() );
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

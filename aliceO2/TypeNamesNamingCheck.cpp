//===--- TypeNamesNamingCheck.cpp - clang-tidy-----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TypeNamesNamingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <regex>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace aliceO2 {

const std::string VALID_NAME_REGEX = "([A-Z][a-z]+)+[0-9]*";

void TypeNamesNamingCheck::registerMatchers(MatchFinder *Finder) {
  const auto validNameMatch = matchesName( std::string("::") + VALID_NAME_REGEX + "$" );

  // match class, struct or enum declarations
  Finder->addMatcher(decl(allOf(
    anyOf(
      // match class or struct declaration
      // "hasDescendant(cxxRecordDecl())" prevents from matching implicit declarations
      cxxRecordDecl(hasDescendant(cxxRecordDecl())),
      enumDecl()),
    namedDecl(unless(validNameMatch))
    )).bind("record-enum-decl"), this );
  
  // match typedef statements, filtering the implicit ones that are added by the compiler
  Finder->addMatcher( typedefDecl(unless(isImplicit())).bind("typedef-decl"), this );
  Finder->addMatcher( usingDecl(unless(validNameMatch)).bind("using-decl"), this );
  
  // match usage of class or struct, filtering the implicit ones
  Finder->addMatcher(loc(recordType(allOf(
    hasDeclaration(unless(isImplicit())),
    hasDeclaration(cxxRecordDecl(unless(validNameMatch)))
    ))).bind("record-enum-usage"), this);
    
  // match usage of enums, filtering the implicit ones
  Finder->addMatcher(loc(enumType(allOf(
    hasDeclaration(unless(isImplicit())),
    hasDeclaration(enumDecl(unless(validNameMatch)))
    ))).bind("record-enum-usage"), this);

  // match usage of typedefs, filtering the implicit ones
  Finder->addMatcher(loc(typedefType(allOf(
    hasDeclaration(unless(isImplicit())),
    hasDeclaration(typedefDecl(unless(validNameMatch)))
    ))).bind("typedef-usage"), this);
}

void TypeNamesNamingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedRecordEnumDecl = Result.Nodes.getNodeAs<Decl>("record-enum-decl");
  if( MatchedRecordEnumDecl )
  {
    std::string oldName("");
    std::string newName("");
    const auto *RecordDeclType = static_cast<const CXXRecordDecl*>(MatchedRecordEnumDecl);
    if( RecordDeclType )
    {
      oldName = RecordDeclType->getDeclName().getAsString();
    }
    const auto *EnumDeclType = static_cast<const EnumDecl*>(MatchedRecordEnumDecl);
    if( EnumDeclType )
    {
      oldName = EnumDeclType->getDeclName().getAsString();
    }
    newName = oldName;
    
    bool fixed = fixName(newName);
    if( fixed && (newName != oldName) )
    {
      diag(MatchedRecordEnumDecl->getLocation(), "typename \'%0\' does not follow the camel case naming convention")
          << oldName
          << FixItHint::CreateReplacement(MatchedRecordEnumDecl->getLocation(), newName);

      // fix ctors namings
      if( RecordDeclType )
      {
        if( RecordDeclType->hasUserDeclaredConstructor() )
        {
          CXXRecordDecl::ctor_iterator it = RecordDeclType->ctor_begin();
          for(; it != RecordDeclType->ctor_end(); ++it)
          {
            if( it->isImplicit() )
              continue;
          
            auto ctor = *it;
            diag(ctor->getLocation(), "constructor for typename \'%0\' does not follow the camel case naming convention")
                << oldName
                << FixItHint::CreateReplacement(ctor->getLocation(), newName);
          }
        }
      }
    }
    else if( !fixed )
    {
      logNameError(MatchedRecordEnumDecl->getLocation(), oldName);
    }    
  }

  const auto *MatchedTypedefDecl = Result.Nodes.getNodeAs<TypedefDecl>("typedef-decl");
  if( MatchedTypedefDecl )
  {
    std::string oldName = MatchedTypedefDecl->getDeclName().getAsString();
    std::string newName = oldName;
    
    if( std::regex_match(oldName, std::regex(VALID_NAME_REGEX)) )
    {
      return;
    }
    
    bool fixed = fixName(newName);
    if( fixed && (newName != oldName) )
    {
      diag(MatchedTypedefDecl->getLocation(), "typename \'%0\' does not follow the camel case naming convention")
          << oldName
          << FixItHint::CreateReplacement(MatchedTypedefDecl->getLocation(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedTypedefDecl->getLocation(), oldName);
    }
  }
  
  const auto *MatchedUsingDecl = Result.Nodes.getNodeAs<UsingDecl>("using-decl");
  if( MatchedUsingDecl )
  {
    std::string oldName = MatchedUsingDecl->getNameAsString();
    std::string newName = oldName;
    
    bool fixed = fixName(newName);
    if( fixed && (newName != oldName) )
    {
      diag(MatchedUsingDecl->getLocation(), "typename \'%0\' does not follow the camel case naming convention")
          << oldName
          << FixItHint::CreateReplacement(MatchedUsingDecl->getLocation(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedUsingDecl->getLocation(), oldName);
    }
  }
  
  const auto *MatchedRecordEnumTypeLoc = Result.Nodes.getNodeAs<TypeLoc>("record-enum-usage");
  if( MatchedRecordEnumTypeLoc )
  {
    const auto Type = MatchedRecordEnumTypeLoc->getType().split().asPair().first;
    std::string oldName = Type->getAsTagDecl()->getNameAsString();    
    std::string newName = oldName;
    
    bool fixed = fixName(newName);
    if( fixed && (newName != oldName) )
    {
      diag(MatchedRecordEnumTypeLoc->getBeginLoc(), "typename \'%0\' does not follow the camel case naming convention")
          << oldName
          << FixItHint::CreateReplacement(MatchedRecordEnumTypeLoc->getSourceRange(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedRecordEnumTypeLoc->getBeginLoc(), oldName);
    }
  }

  const auto *MatchedTypedefTypeLoc = Result.Nodes.getNodeAs<TypeLoc>("typedef-usage");
  if( MatchedTypedefTypeLoc )
  {
    const auto Type = MatchedTypedefTypeLoc->getType();
    std::string oldName = Type->getAs<TypedefType>()->getDecl()->getNameAsString();
    std::string newName = oldName;
    
    bool fixed = fixName(newName);
    if( fixed && (newName != oldName) )
    {
      diag(MatchedTypedefTypeLoc->getBeginLoc(), "typename \'%0\' does not follow the camel case naming convention")
          << oldName
          << FixItHint::CreateReplacement(MatchedTypedefTypeLoc->getSourceRange(), newName);
    }
    else if( !fixed )
    {
      logNameError(MatchedTypedefTypeLoc->getBeginLoc(), oldName);
    }
  }
}

/// This function modifies the input string to one of the following ways:
/// - Into a CamelCase string.
/// - Into a string that is specified in the config options.
///
/// return: Returns true if the input string is succesfully modified,
///         If neither of the modifying options succeeds false is returned.
bool TypeNamesNamingCheck::fixName(std::string &name)
{
  std::string replace_option = Options.get(name, "");
  if( replace_option != "" )
  {
    name = replace_option;
    return true;
  }
  
  if( islower(name[0]) )
  {
    name[0] = toupper(name[0]);
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

void TypeNamesNamingCheck::logNameError(SourceLocation Loc, std::string errorName)
{
  diag(Loc, "Could not fix \'%0\'", DiagnosticIDs::Level::Error)
      << errorName;
}


} // namespace aliceO2
} // namespace tidy
} // namespace clang

//===--- InterfaceLister.cpp - clang-tidy---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "InterfaceLister.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace reporting {

void InterfaceLister::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxMemberCallExpr().bind("member"), this);
}

void InterfaceLister::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedCallExpr =
      Result.Nodes.getNodeAs<CXXMemberCallExpr>("member");
  if (MatchedCallExpr) {
    if (std::strcmp(MatchedCallExpr->getRecordDecl()
                        ->getDeclName()
                        .getAsString()
                        .c_str(),
                    ClassName.c_str()) != 0)
      return;

    std::string sourceInfo(MatchedCallExpr->getExprLoc().printToString(
        MyContext->getSourceManager()));
    std::cerr << sourceInfo << " ; " << ClassName << " : "
              << MatchedCallExpr->getMethodDecl()->getQualifiedNameAsString()
              << "\n";
  }
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

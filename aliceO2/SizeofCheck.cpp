//===--- SizeofCheck.cpp - clang-tidy--------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "SizeofCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace aliceO2 {

void SizeofCheck::registerMatchers(MatchFinder *Finder) {
  // matches sizeof + alignof
  Finder->addMatcher(unaryExprOrTypeTraitExpr().bind("unaryexp"), this);
}

void SizeofCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Expr = Result.Nodes.getNodeAs<UnaryExprOrTypeTraitExpr>("unaryexp");
  const auto Kind = Expr->getKind();
  if (Kind != UETT_SizeOf){
    return;
  }

  // is argument of sizeof a Type?
  if (Expr->isArgumentType()) {
    diag(Expr->getBeginLoc(), "consider using sizeof() on instance instead on direct type");
    return;
  }

  // we found a correct expression; check if it uses parens
  if (dyn_cast<ParenExpr>(Expr->getArgumentExpr())) {
    return;
  }

  // no parens -> issue warning
  diag(Expr->getBeginLoc(), "consider using parens () for arguments to sizeof");
}

} // namespace aliceO2
} // namespace tidy
} // namespace clang

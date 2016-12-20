//===--- VirtFuncLister.cpp - clang-tidy---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "VirtFuncLister.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace reporting {

void VirtFuncLister::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxMethodDecl().bind("method"), this);
}

void VirtFuncLister::check(const MatchFinder::MatchResult &Result) {
  const auto &SM = *Result.SourceManager;
  const auto *MatchedDecl =
      Result.Nodes.getNodeAs<CXXMethodDecl>("method");
  if (MatchedDecl) {
      DeclarationNameInfo name_info = MatchedDecl->getNameInfo();
      std::string func_name = name_info.getAsString();

      bool isvirtual = MatchedDecl->isVirtual();

        llvm::errs() << "MEMBER FUNCTION DECL " << func_name << "("<<MatchedDecl<<")" <<
        " marked virtual "
               << MatchedDecl->isVirtualAsWritten() << " is virtual "
               << isvirtual << "\n";

      // get class declaration of this member function
      // auto record = MatchedDecl->getCanonicalDecl()->getParent();

      // we need to get source location the base node
      // as well as overriding node
      if (isvirtual) {
        auto iter = MatchedDecl->begin_overridden_methods();
        auto enditer = MatchedDecl->end_overridden_methods();

        if (iter == enditer) {
          SourceLocation loc = MatchedDecl->getLocStart();//getLocation();
          loc.dump(SM);
          llvm::errs() << "VIRTUAL-START-DECLARATION \n";
        } else {
          // otherwise find the base this is referring to
          decltype(iter) lastiter = iter;
          while (iter != enditer) {
            lastiter = iter;
            // counter number of paths up; if there is more than one
            // we have to give up for the moment
            int counter = 0;
            decltype(iter) countiter = iter;
            for (; countiter != enditer; ++countiter) {
              counter++;
            }
            if (counter > 1) {
              llvm::errs() << " OVERRIDING MULTIPLE FUNCTIONS NOT TREATED YET\n";
              return;
            }
            enditer = (*iter)->end_overridden_methods();
            iter = (*iter)->begin_overridden_methods();
          }
          SourceLocation loc = (*lastiter)->getLocStart();//getLocation();
          loc.dump(SM);
          llvm::errs() << " OVERRIDEN-AT ";
          loc = MatchedDecl->getLocStart();//getLocation();
          loc.dump(SM);
          llvm::errs() << "\n";
        }
      }
    }
}

} // namespace reporting
} // namespace tidy
} // namespace clang

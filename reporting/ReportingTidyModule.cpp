//===--- AliceO2TidyModule.cpp - clang-tidy ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "InterfaceLister.h"

namespace clang {
namespace tidy {
namespace reporting {

class ReportingModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<InterfaceLister>("Reporting-interfaces-used");
  }
};

} // namespace Reporting

// Register the ReportingTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<reporting::ReportingModule>
X("Reporting-module", "Adds Reporting tools (for simple analytics)");

// This anchor is used to force the linker to link in the generated object file
// and thus register the ReportingModule.
volatile int ReportingModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang

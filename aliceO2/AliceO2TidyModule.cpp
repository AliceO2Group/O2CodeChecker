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
#include "MemberNamesCheck.h"

namespace clang {
namespace tidy {
namespace aliceO2 {

class AliceO2Module : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<MemberNamesCheck>("aliceO2-member-name");
  }
};

} // namespace aliceO2

// Register the AliceO2TidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<aliceO2::AliceO2Module>
X("aliceO2-module", "Adds AliceO2 specific checks");

// This anchor is used to force the linker to link in the generated object file
// and thus register the AliceO2Module.
volatile int AliceO2ModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang

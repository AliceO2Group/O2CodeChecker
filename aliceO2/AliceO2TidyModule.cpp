//===--- AliceO2TidyModule.cpp - clang-tidy ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyModule.h"
// Up to Clang 21 ClangTidyModuleRegistry was declared only in
// ClangTidyModuleRegistry.h. Clang 22 moved it into ClangTidyModule.h above and
// left the old header behind as a bare #warning, to be deleted in Clang 24 --
// so __has_include() would keep including it, and keep warning, on 22 and 23.
#include "llvm/Config/llvm-config.h"
#if LLVM_VERSION_MAJOR < 22
#include "clang-tidy/ClangTidyModuleRegistry.h"
#endif
#include "MemberNamesCheck.h"
#include "NamespaceNamingCheck.h"
#include "SizeofCheck.h"

namespace clang {
namespace tidy {
namespace aliceO2 {

class AliceO2Module : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<MemberNamesCheck>(
        "aliceO2-member-name");
    CheckFactories.registerCheck<NamespaceNamingCheck>(
        "aliceO2-namespace-naming");
    CheckFactories.registerCheck<SizeofCheck>(
        "aliceO2-SizeOf");
  }
};

 
} // namespace aliceO2

// Register the AliceO2TidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<aliceO2::AliceO2Module> X("aliceO2-module",
						     "Adds AliceO2 specific checks");


// This anchor is used to force the linker to link in the generated object file
// and thus register the AliceO2Module.
volatile int AliceO2ModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang

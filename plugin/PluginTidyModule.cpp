//===--- PluginTidyModule.cpp - clang-tidy ----------------------------------===//
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
#include "FooCheck.h"
#include <iostream>

namespace clang {
namespace tidy {
namespace plugin {

class PluginModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    std::cerr << "side effect \n";
    CheckFactories.registerCheck<FooCheck>(
        "plugin-Foo");
  }
};

} // namespace plugin

// Register the PluginTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<plugin::PluginModule>
X("pluginO2-module", "Adds Plugin specific checks");

// This anchor is used to force the linker to link in the generated object file
// and thus register the PluginModule.
volatile int PluginModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang


// A function to execute upon load of shared library
//__attribute__((constructor))
static int huhuhuhuhu() {
  static clang::tidy::plugin::PluginModule module;
  return clang::tidy::PluginModuleAnchorSource;
}

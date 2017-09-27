//===--- NamespaceNamingCheck.h - clang-tidy---------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_CONFIGURATION_PARSER_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_CONFIGURATION_PARSER_H

#include <unordered_map>

namespace clang {
namespace tidy {
namespace aliceO2 {

/// This class is used to parse a configuration file in a predefined format
/// like the .clang-tidy format.
///
class ConfigurationParser {
public:
  void init(std::string configFilePath, std::string checkName);
  std::string getValue(std::string key);
  void writeKey(std::string key);

private:
  std::string configFilePath;
  std::string checkName;
  std::unordered_map<std::string,std::string> configKeyToValue;
};

} // namespace aliceO2
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ALICEO2_CONFIGURATION_PARSER_H

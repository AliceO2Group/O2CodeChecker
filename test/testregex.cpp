#include <regex>
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>

// testing correct compiler support for regular expressions
// (For example: although the code compiles fine with gcc4.8; it does not
// execute)
int main() {
  assert(std::regex_match("mGood", std::regex("m[A-Z][a-z]+")));
  assert(std::regex_match("mGood", std::regex("m[A-Z].*")));
  return 0;
}

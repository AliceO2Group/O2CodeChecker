// RUN: %check_clang_tidy %s aliceO2-variable-naming-constexpr %t

namespace first_namespace
{
const int constSomeFormattingRuleCorrect = 1;
const int ConstAnotherFormattingRuleCorrect = 2;
const int CONST_THIRD_FORMATTING_RULE_CORRECT = 3;
const int const_fourth_formatting_rule_correct = 4;

constexpr int var = 42;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: constexpr variable 'var' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
// CHECK-FIXES: {{^}}constexpr int VAR = 42;{{$}}
constexpr int someFormattingRuleWrong = 5;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: constexpr variable 'someFormattingRuleWrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
// CHECK-FIXES: {{^}}constexpr int SOME_FORMATTING_RULE_WRONG = 5;{{$}}
constexpr int AnotherFormattingRuleWrong = 6;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: constexpr variable 'AnotherFormattingRuleWrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
// CHECK-FIXES: {{^}}constexpr int ANOTHER_FORMATTING_RULE_WRONG = 6;{{$}}
constexpr int THIRD_FORMATTING_RULE_CORRECT = 7;
constexpr int fourth_formatting_rule_wrong = 8;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: constexpr variable 'fourth_formatting_rule_wrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
// CHECK-FIXES: {{^}}constexpr int FOURTH_FORMATTING_RULE_WRONG = 8;{{$}}

constexpr int fifth_formatting_rule_wrong = fourth_formatting_rule_wrong;
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: constexpr variable 'fifth_formatting_rule_wrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
// CHECK-MESSAGES: :[[@LINE-2]]:45: warning: constexpr variable 'fourth_formatting_rule_wrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
// CHECK-FIXES: {{^}}constexpr int FIFTH_FORMATTING_RULE_WRONG = FOURTH_FORMATTING_RULE_WRONG;{{$}}

class MyClass
{
public:
  const int constSomeFormattingRuleCorrect = 1;
  const int ConstAnotherFormattingRuleCorrect = 2;
  const int CONST_THIRD_FORMATTING_RULE_CORRECT = 3;
  const int const_fourth_formatting_rule_correct = 4;

  static constexpr int someFormattingRuleWrong = 5;
  // CHECK-MESSAGES: :[[@LINE-1]]:24: warning: constexpr variable 'someFormattingRuleWrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-FIXES: {{^}}  static constexpr int SOME_FORMATTING_RULE_WRONG = 5;{{$}}
  static constexpr int AnotherFormattingRuleWrong = 6;
  // CHECK-MESSAGES: :[[@LINE-1]]:24: warning: constexpr variable 'AnotherFormattingRuleWrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-FIXES: {{^}}  static constexpr int ANOTHER_FORMATTING_RULE_WRONG = 6;{{$}}
  static constexpr int THIRD_FORMATTING_RULE_CORRECT = 7;
  static constexpr int fourth_formatting_rule_wrong = 8;
  // CHECK-MESSAGES: :[[@LINE-1]]:24: warning: constexpr variable 'fourth_formatting_rule_wrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-FIXES: {{^}}  static constexpr int FOURTH_FORMATTING_RULE_WRONG = 8;{{$}}  
};
} // first_namespace

int main()
{
  int dummy;

  dummy = first_namespace::var;
// CHECK-MESSAGES: :[[@LINE-1]]:28: warning: constexpr variable 'var' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
// CHECK-FIXES: {{^}}  dummy = first_namespace::VAR;{{$}}
 
  dummy = first_namespace::constSomeFormattingRuleCorrect;
  dummy = first_namespace::ConstAnotherFormattingRuleCorrect;
  dummy = first_namespace::CONST_THIRD_FORMATTING_RULE_CORRECT;
  dummy = first_namespace::const_fourth_formatting_rule_correct;

  dummy = first_namespace::someFormattingRuleWrong;
  // CHECK-MESSAGES: :[[@LINE-1]]:28: warning: constexpr variable 'someFormattingRuleWrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-FIXES: {{^}}  dummy = first_namespace::SOME_FORMATTING_RULE_WRONG;{{$}}
  dummy = first_namespace::AnotherFormattingRuleWrong;
  // CHECK-MESSAGES: :[[@LINE-1]]:28: warning: constexpr variable 'AnotherFormattingRuleWrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-FIXES: {{^}}  dummy = first_namespace::ANOTHER_FORMATTING_RULE_WRONG;{{$}}
  dummy = first_namespace::THIRD_FORMATTING_RULE_CORRECT;
  dummy = first_namespace::fourth_formatting_rule_wrong;
  // CHECK-MESSAGES: :[[@LINE-1]]:28: warning: constexpr variable 'fourth_formatting_rule_wrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-FIXES: {{^}}  dummy = first_namespace::FOURTH_FORMATTING_RULE_WRONG;{{$}}

  dummy = first_namespace::someFormattingRuleWrong * first_namespace::fourth_formatting_rule_wrong;
  // CHECK-MESSAGES: :[[@LINE-1]]:28: warning: constexpr variable 'someFormattingRuleWrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-MESSAGES: :[[@LINE-2]]:71: warning: constexpr variable 'fourth_formatting_rule_wrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-FIXES: {{^}}  dummy = first_namespace::SOME_FORMATTING_RULE_WRONG * first_namespace::FOURTH_FORMATTING_RULE_WRONG;{{$}}

  dummy = first_namespace::MyClass::someFormattingRuleWrong;
  // CHECK-MESSAGES: :[[@LINE-1]]:37: warning: constexpr variable 'someFormattingRuleWrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-FIXES: {{^}}  dummy = first_namespace::MyClass::SOME_FORMATTING_RULE_WRONG;{{$}}
  dummy = first_namespace::MyClass::AnotherFormattingRuleWrong;
  // CHECK-MESSAGES: :[[@LINE-1]]:37: warning: constexpr variable 'AnotherFormattingRuleWrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-FIXES: {{^}}  dummy = first_namespace::MyClass::ANOTHER_FORMATTING_RULE_WRONG;{{$}}
  dummy = first_namespace::MyClass::THIRD_FORMATTING_RULE_CORRECT;
  dummy = first_namespace::MyClass::fourth_formatting_rule_wrong;
  // CHECK-MESSAGES: :[[@LINE-1]]:37: warning: constexpr variable 'fourth_formatting_rule_wrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-FIXES: {{^}}  dummy = first_namespace::MyClass::FOURTH_FORMATTING_RULE_WRONG;{{$}}
  
  first_namespace::MyClass dummyClassVar;
  dummy = dummyClassVar.fourth_formatting_rule_wrong;
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: constexpr variable 'fourth_formatting_rule_wrong' must be CAPITALIZED [aliceO2-variable-naming-constexpr]
  // CHECK-FIXES: {{^}}  dummy = dummyClassVar.FOURTH_FORMATTING_RULE_WRONG;{{$}}

  return 0;
}

// RUN: %check_clang_tidy %s aliceO2-function-naming %t

void globalFunctionCorrect() {}
bool isGlobalFunctionCorrect2() { return true; }

void global_function_wrong() {}
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'global_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}void globalFunctionWrong() {}{{$}}
bool globalFunctionWrong2() { return true; }
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function that returns bool 'globalFunctionWrong2' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}bool isGlobalFunctionWrong2() { return true; }{{$}}

int getFiveCorrect() { return 5; }
int get_five_wrong() { return 5; }
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: function 'get_five_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}int getFiveWrong() { return 5; }{{$}}

namespace first_namespace
{
void globalFunctionCorrect() {}
bool isGlobalFunctionCorrect2() { return true; }

void global_function_wrong() {}
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'global_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}void globalFunctionWrong() {}{{$}}
bool globalFunctionWrong2() { return true; }
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function that returns bool 'globalFunctionWrong2' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}bool isGlobalFunctionWrong2() { return true; }{{$}}

int getFiveCorrect() { return 5; }
int get_five_wrong() { return 5; }
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: function 'get_five_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}int getFiveWrong() { return 5; }{{$}}

class MyClass
{
  public:
    void firstFunctionCorrect() {}
    void first_function_wrong() {}
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function 'first_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}    void firstFunctionWrong() {}{{$}}
    bool functionSecondWrong() { return true; }
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function that returns bool 'functionSecondWrong' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}    bool isFunctionSecondWrong() { return true; }{{$}}
    bool function_second_wrong2() { return true; }
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function that returns bool 'function_second_wrong2' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}    bool hasFunctionSecondWrong2() { return true; }{{$}}

    static void firstStaticFunctionCorrect() {}
    static void first_static_function_wrong() {}
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: function 'first_static_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}    static void firstStaticFunctionWrong() {}{{$}}
    static bool functionStaticSecondWrong() { return true; }
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: function that returns bool 'functionStaticSecondWrong' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}    static bool isFunctionStaticSecondWrong() { return true; }{{$}}
};
};

int main()
{
  // usage  
  globalFunctionCorrect();
  isGlobalFunctionCorrect2();

  global_function_wrong();
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: function 'global_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  globalFunctionWrong();{{$}}
  globalFunctionWrong2();
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: function that returns bool 'globalFunctionWrong2' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  isGlobalFunctionWrong2();{{$}}

  getFiveCorrect();
  get_five_wrong();
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: function 'get_five_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  getFiveWrong();{{$}}

  first_namespace::globalFunctionCorrect();
  first_namespace::isGlobalFunctionCorrect2();

  first_namespace::global_function_wrong();
// CHECK-MESSAGES: :[[@LINE-1]]:20: warning: function 'global_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  first_namespace::globalFunctionWrong();{{$}}
  first_namespace::globalFunctionWrong2();
// CHECK-MESSAGES: :[[@LINE-1]]:20: warning: function that returns bool 'globalFunctionWrong2' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  first_namespace::isGlobalFunctionWrong2();{{$}}

  first_namespace::getFiveCorrect();
  first_namespace::get_five_wrong();
// CHECK-MESSAGES: :[[@LINE-1]]:20: warning: function 'get_five_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  first_namespace::getFiveWrong();{{$}}

  first_namespace::MyClass myClassVar;
  myClassVar.firstStaticFunctionCorrect();
  myClassVar.firstFunctionCorrect();
  myClassVar.first_function_wrong();
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: function 'first_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  myClassVar.firstFunctionWrong();{{$}}
  myClassVar.functionSecondWrong();
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: function that returns bool 'functionSecondWrong' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  myClassVar.isFunctionSecondWrong();{{$}}
  bool result = myClassVar.function_second_wrong2();
// CHECK-MESSAGES: :[[@LINE-1]]:28: warning: function that returns bool 'function_second_wrong2' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  bool result = myClassVar.hasFunctionSecondWrong2();{{$}}

  first_namespace::MyClass *myClassVar2 = new first_namespace::MyClass();
  myClassVar2->firstStaticFunctionCorrect();
  myClassVar2->firstFunctionCorrect();
  myClassVar2->first_function_wrong();
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: function 'first_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  myClassVar2->firstFunctionWrong();{{$}}
  myClassVar2->functionSecondWrong();
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: function that returns bool 'functionSecondWrong' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  myClassVar2->isFunctionSecondWrong();{{$}}
  result = myClassVar2->function_second_wrong2();
// CHECK-MESSAGES: :[[@LINE-1]]:25: warning: function that returns bool 'function_second_wrong2' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  result = myClassVar2->hasFunctionSecondWrong2();{{$}}

  first_namespace::MyClass::firstStaticFunctionCorrect();
  first_namespace::MyClass::first_static_function_wrong();
// CHECK-MESSAGES: :[[@LINE-1]]:29: warning: function 'first_static_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  first_namespace::MyClass::firstStaticFunctionWrong();{{$}}
  first_namespace::MyClass::functionStaticSecondWrong();
// CHECK-MESSAGES: :[[@LINE-1]]:29: warning: function that returns bool 'functionStaticSecondWrong' must be prefixed with 'is' or 'has' [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  first_namespace::MyClass::isFunctionStaticSecondWrong();{{$}}

  delete myClassVar2;

  return 0;
}

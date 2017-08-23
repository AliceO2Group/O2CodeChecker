// RUN: %check_clang_tidy %s aliceO2-variable-naming-static-member %t

class MyClass
{
public:
  int a;
};

static int static_var1;
MyClass static_var2;

class SecondClass
{
public:
  int var1;
  MyClass var2;

  static int sStaticVarCorrect;
  
  static int staticVar1;
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: static member variable 'staticVar1' must be prefixed by 's' [aliceO2-variable-naming-static-member]
// CHECK-FIXES: {{^}}  static int sStaticVar1;{{$}}
  static int anotherStaticVar;
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: static member variable 'anotherStaticVar' must be prefixed by 's' [aliceO2-variable-naming-static-member]
// CHECK-FIXES: {{^}}  static int sAnotherStaticVar;{{$}}
  static MyClass staticVar2;
// CHECK-MESSAGES: :[[@LINE-1]]:18: warning: static member variable 'staticVar2' must be prefixed by 's' [aliceO2-variable-naming-static-member]
// CHECK-FIXES: {{^}}  static MyClass sStaticVar2;{{$}}
  
  SecondClass()
  {
    sStaticVarCorrect = 42;
    anotherStaticVar = 42;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: static member variable 'anotherStaticVar' must be prefixed by 's' [aliceO2-variable-naming-static-member]
// CHECK-FIXES: {{^}}    sAnotherStaticVar = 42;{{$}}
    var1 = 42;
    staticVar1 = 42;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: static member variable 'staticVar1' must be prefixed by 's' [aliceO2-variable-naming-static-member]
// CHECK-FIXES: {{^}}    sStaticVar1 = 42;{{$}}
  }
  
  void memberFunction();
};

void SecondClass::memberFunction()
{
  var1++;
  staticVar1++;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: static member variable 'staticVar1' must be prefixed by 's' [aliceO2-variable-naming-static-member]
// CHECK-FIXES: {{^}}  sStaticVar1++;{{$}}
  staticVar2.a = 42;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: static member variable 'staticVar2' must be prefixed by 's' [aliceO2-variable-naming-static-member]
// CHECK-FIXES: {{^}}  sStaticVar2.a = 42;{{$}}
}

int main()
{
  SecondClass classVar1;
  classVar1.var1 = 42;
  SecondClass::staticVar1 = 44;
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: static member variable 'staticVar1' must be prefixed by 's' [aliceO2-variable-naming-static-member]
// CHECK-FIXES: {{^}}  SecondClass::sStaticVar1 = 44;{{$}}
  SecondClass::staticVar2.a = 42;
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: static member variable 'staticVar2' must be prefixed by 's' [aliceO2-variable-naming-static-member]
// CHECK-FIXES: {{^}}  SecondClass::sStaticVar2.a = 42;{{$}}
  classVar1.staticVar2.a = 42;
// CHECK-MESSAGES: :[[@LINE-1]]:13: warning: static member variable 'staticVar2' must be prefixed by 's' [aliceO2-variable-naming-static-member]
// CHECK-FIXES: {{^}}  classVar1.sStaticVar2.a = 42;{{$}}

  return 0;
};

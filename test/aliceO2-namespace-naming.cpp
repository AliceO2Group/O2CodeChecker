// RUN: %check_clang_tidy %s aliceO2-namespace-naming %t

namespace
{
namespace simple
{
};
};

namespace o2
{
}
namespace o2codecheck
{
}
namespace a_1234_5678_b
{
}

namespace TPC
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: namespace 'TPC' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}namespace tpc{{$}}
{
}

namespace OuterNamespaceWrong
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: namespace 'OuterNamespaceWrong' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}namespace outer_namespace_wrong{{$}}
{
namespace InnerNamespaceWrong
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: namespace 'InnerNamespaceWrong' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}namespace inner_namespace_wrong{{$}}
{
class InnerClass
{
public:
  int a;
};
};
};

namespace outer_namespace_correct
{
namespace inner_namespace_correct
{
class InnerClass
{
public:
  int b;
};
};
};

using namespace OuterNamespaceWrong::InnerNamespaceWrong;
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: namespace 'OuterNamespaceWrong' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:38: warning: namespace 'InnerNamespaceWrong' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}using namespace outer_namespace_wrong::inner_namespace_wrong;{{$}}
using namespace outer_namespace_correct::inner_namespace_correct;

typedef OuterNamespaceWrong::InnerNamespaceWrong::InnerClass JustInnerClass1;
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: namespace 'OuterNamespaceWrong' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:30: warning: namespace 'InnerNamespaceWrong' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}typedef outer_namespace_wrong::inner_namespace_wrong::InnerClass JustInnerClass1;{{$}}
typedef outer_namespace_correct::inner_namespace_correct::InnerClass JustInnerClass2;

namespace FirstNamespace
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}namespace first_namespace{{$}}
{
  int a;
  void fooFromNamespace();
  
  void bar()
  {
  }
  
  class NamespaceClassForLaterDefinition;
  
  class NamespaceClass
  {
    static int c1;
    public:
      static int bb;
      static int getC1();
      int getC1NotStatic();
      
      class NamespaceClassInnerClass
      {
        int c2;
        public:
          int c3;
          static int c4;
          
          static int getC2();
          int getC2NotStatic();
          
          void dummy1()
          {
            NamespaceClass::bb=2;
          }
      };
  };
};

class FirstNamespace::NamespaceClassForLaterDefinition
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}class first_namespace::NamespaceClassForLaterDefinition{{$}}
{
int justSomething;
};

namespace Singleword
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: namespace 'Singleword' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}namespace singleword{{$}}
{
};

namespace Double_Words
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: namespace 'Double_Words' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}namespace double_words{{$}}
{
};

namespace secondNamespace
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: namespace 'secondNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}namespace second_namespace{{$}}
{
  int b;
};

class FirstClass
{
int fooVar;
public:
  void fooFromClass();
};

struct FirstStruct
{
int fooVar;
public:
  void fooFromStruct();
};

int FirstNamespace::NamespaceClass::getC1()
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}int first_namespace::NamespaceClass::getC1(){{$}}
{
  return c1;
}

void FirstClass::fooFromClass()
{
}

void FirstStruct::fooFromStruct()
{
}

void FirstNamespace::fooFromNamespace(); // just forward declaration
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}void first_namespace::fooFromNamespace(); // just forward declaration{{$}}

void FirstNamespace::fooFromNamespace() // definition
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}void first_namespace::fooFromNamespace() // definition{{$}}
{
}

void dummy1(int par1)
{
}

int main()
{
  FirstNamespace::a=5;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}  first_namespace::a=5;{{$}}
  
  dummy1(FirstNamespace::a);
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}  dummy1(first_namespace::a);{{$}}
  
  int temp = FirstNamespace::NamespaceClass::getC1();
// CHECK-MESSAGES: :[[@LINE-1]]:14: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}  int temp = first_namespace::NamespaceClass::getC1();{{$}}
  FirstNamespace::NamespaceClass classInstance;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}  first_namespace::NamespaceClass classInstance;{{$}}
  temp = classInstance.getC1NotStatic();

  temp = FirstNamespace::NamespaceClass::NamespaceClassInnerClass::getC2();
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}  temp = first_namespace::NamespaceClass::NamespaceClassInnerClass::getC2();{{$}}
  FirstNamespace::NamespaceClass::NamespaceClassInnerClass classInstance2;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}  first_namespace::NamespaceClass::NamespaceClassInnerClass classInstance2;{{$}}
  temp = classInstance2.getC2NotStatic();
  
  temp = classInstance2.c3;
  temp = FirstNamespace::NamespaceClass::NamespaceClassInnerClass::c4;
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: namespace 'FirstNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}  temp = first_namespace::NamespaceClass::NamespaceClassInnerClass::c4;{{$}}
  
  secondNamespace::b=166;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: namespace 'secondNamespace' does not follow the underscore convention [aliceO2-namespace-naming]
// CHECK-FIXES: {{^}}  second_namespace::b=166;{{$}}

  return 0;
}


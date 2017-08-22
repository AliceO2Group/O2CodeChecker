// RUN: %check_clang_tidy %s aliceO2-type-names-naming %t

class Circle
{
public:
  int a;
};

class square
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: typename 'square' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}class Square{{$}}
{
public:
  int a;
};

class SSQ
{
};

SSQ abcd;

namespace first_namespace
{
class BBS
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: typename 'BBS' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}class bbs{{$}}
{
};
}
namespace second_namespace_legacy
{
class BBS
{
};
}

first_namespace::BBS firstVar;
// CHECK-MESSAGES: :[[@LINE-1]]:18: warning: typename 'BBS' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}first_namespace::bbs firstVar;{{$}}
second_namespace_legacy::BBS secondVar;

using namespace second_namespace_legacy;
BBS thirdVar;

class abird
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: typename 'abird' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}class Abird{{$}}
{
};

Circle a;
square b;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: typename 'square' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}Square b;{{$}}

// Declarations
class FirstClassNameCorrect
{
public:
  int a1;
  class FirstClassNameInnerCorrect
  {
  public:
    int a2;
  };
  class FirstClassNameInner_wrong
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: typename 'FirstClassNameInner_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  class FirstClassNameInnerWrong{{$}}
  {
  public:
    int a2;
  };
  
  struct FirstClassNameInner_struct_wrong
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  struct FirstClassNameInnerStructWrong{{$}}
  {
    int a3;
    static int a4;

    // ctor
    FirstClassNameInner_struct_wrong(int a = 6)
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: constructor for typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}    FirstClassNameInnerStructWrong(int a = 6){{$}}
    {
      this->a3 = a;
    }
    
    ~FirstClassNameInner_struct_wrong()
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}    ~FirstClassNameInnerStructWrong(){{$}}
    {
    }
    
    enum Inner_Enum_wrong
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: typename 'Inner_Enum_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}    enum InnerEnumWrong{{$}}
    {
      enum1e1 = 0,
      enum1e2 = 1,
      enum1eMax = 2
    };
    
    class FirstClassNameInner_struct_wrong_ClassWrong
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: typename 'FirstClassNameInner_struct_wrong_ClassWrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}    class FirstClassNameInnerStructWrongClassWrong{{$}}
    {
    public:
      static int a5;
    };
  };
  
  static FirstClassNameInner_struct_wrong a3;
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  static FirstClassNameInnerStructWrong a3;{{$}}
  FirstClassNameInner_struct_wrong a31;
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameInnerStructWrong a31;{{$}}
  
  enum FirstClassNameInnerEnumCorrect
  {
    enum2e1 = 0,
    enum2e2 = 1,
    enum2eMax = 2
  };
  enum First_Class_Name_Inner_Enum_Wrong
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: typename 'First_Class_Name_Inner_Enum_Wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  enum FirstClassNameInnerEnumWrong{{$}}
  {
    enum3e1 = 0,
    enum3e2 = 1,
    enum3eMax = 2
  };

  typedef FirstClassNameInnerCorrect CorrectTypedefOfClass;
  typedef FirstClassNameInnerCorrect wrong_typedef_of_class;
// CHECK-MESSAGES: :[[@LINE-1]]:38: warning: typename 'wrong_typedef_of_class' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  typedef FirstClassNameInnerCorrect WrongTypedefOfClass;{{$}}
  typedef FirstClassNameInner_struct_wrong CorrectTypedefOfStruct;
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  typedef FirstClassNameInnerStructWrong CorrectTypedefOfStruct;{{$}}
  typedef FirstClassNameInner_struct_wrong wrong_typedef_Of_Struct;
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:44: warning: typename 'wrong_typedef_Of_Struct' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  typedef FirstClassNameInnerStructWrong WrongTypedefOfStruct;{{$}}
  typedef First_Class_Name_Inner_Enum_Wrong CorrectTypedefOfEnum;
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: typename 'First_Class_Name_Inner_Enum_Wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  typedef FirstClassNameInnerEnumWrong CorrectTypedefOfEnum;{{$}}
  typedef First_Class_Name_Inner_Enum_Wrong wrongTypedefOfEnum;
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: typename 'First_Class_Name_Inner_Enum_Wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:45: warning: typename 'wrongTypedefOfEnum' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  typedef FirstClassNameInnerEnumWrong WrongTypedefOfEnum;{{$}}
  typedef FirstClassNameInner_struct_wrong::Inner_Enum_wrong wrongTypedefOfInnerEnum;
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:45: warning: typename 'Inner_Enum_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-3]]:62: warning: typename 'wrongTypedefOfInnerEnum' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  typedef FirstClassNameInnerStructWrong::InnerEnumWrong WrongTypedefOfInnerEnum;{{$}}
  typedef FirstClassNameInner_wrong wrongTypedefOfInnerEnum223;
// CHECK-MESSAGES: :[[@LINE-1]]:11: warning: typename 'FirstClassNameInner_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:37: warning: typename 'wrongTypedefOfInnerEnum223' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  typedef FirstClassNameInnerWrong WrongTypedefOfInnerEnum223;{{$}}
  
  void f()
  {
    FirstClassNameInnerCorrect varClass1;
    CorrectTypedefOfClass varClass2;
    wrong_typedef_of_class varClass3;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: typename 'wrong_typedef_of_class' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}    WrongTypedefOfClass varClass3;{{$}}
    
    CorrectTypedefOfStruct varStruct1;
    wrong_typedef_Of_Struct varStruct2;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: typename 'wrong_typedef_Of_Struct' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}    WrongTypedefOfStruct varStruct2;{{$}}
  }
};

  typedef FirstClassNameCorrect::FirstClassNameInner_wrong wrongTypedefOfInnerEnum22;
// CHECK-MESSAGES: :[[@LINE-1]]:34: warning: typename 'FirstClassNameInner_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:60: warning: typename 'wrongTypedefOfInnerEnum22' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  typedef FirstClassNameCorrect::FirstClassNameInnerWrong WrongTypedefOfInnerEnum22;{{$}}
  typedef FirstClassNameCorrect::FirstClassNameInner_struct_wrong::FirstClassNameInner_struct_wrong_ClassWrong wrongTypedefOfInnerEnum222;
// CHECK-MESSAGES: :[[@LINE-1]]:34: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:68: warning: typename 'FirstClassNameInner_struct_wrong_ClassWrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-3]]:112: warning: typename 'wrongTypedefOfInnerEnum222' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  typedef FirstClassNameCorrect::FirstClassNameInnerStructWrong::FirstClassNameInnerStructWrongClassWrong WrongTypedefOfInnerEnum222;{{$}}

void dummy1(int a) {}

void dummy2(FirstClassNameCorrect::FirstClassNameInner_struct_wrong var) {}
// CHECK-MESSAGES: :[[@LINE-1]]:36: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}void dummy2(FirstClassNameCorrect::FirstClassNameInnerStructWrong var) {}{{$}}


namespace just_a_namespace
{
  class NamespaceClassCorrect
  {
  public:
    int a;
  };
  class NamespaceClass_wrong
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: typename 'NamespaceClass_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  class NamespaceClassWrong{{$}}
  {
  public:
    int a;
  };
  
  struct NamespaceStructCorrect
  {
  public:
    int a;
  };
  struct NamespaceStruct_wrong
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: typename 'NamespaceStruct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  struct NamespaceStructWrong{{$}}
  {
  public:
    int a;
    int unique_var_name;
  };
  
  union NamespaceUnionCorrect
  {
    int a;
    int b;
  };
  union NamespaceUnion_wrong
// CHECK-MESSAGES: :[[@LINE-1]]:9: warning: typename 'NamespaceUnion_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  union NamespaceUnionWrong{{$}}
  {
    int a;
    int b;
  };

  enum NamespaceEnumCorrect
  {
    ns_enum_c_1 = 0,
    ns_enum_c_2 = 1,
    ns_enum_c_MAX = 2
  };
  enum NamespaceEnum_wrong
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: typename 'NamespaceEnum_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  enum NamespaceEnumWrong{{$}}
  {
    ns_enum_w_1 = 0,
    ns_enum_w_2 = 1,
    ns_enum_w_MAX = 2
  };
};

just_a_namespace::NamespaceClass_wrong ns_var_1;
// CHECK-MESSAGES: :[[@LINE-1]]:19: warning: typename 'NamespaceClass_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}just_a_namespace::NamespaceClassWrong ns_var_1;{{$}}
just_a_namespace::NamespaceStruct_wrong ns_var_2;
// CHECK-MESSAGES: :[[@LINE-1]]:19: warning: typename 'NamespaceStruct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}just_a_namespace::NamespaceStructWrong ns_var_2;{{$}}
just_a_namespace::NamespaceUnion_wrong ns_var_3;
// CHECK-MESSAGES: :[[@LINE-1]]:19: warning: typename 'NamespaceUnion_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}just_a_namespace::NamespaceUnionWrong ns_var_3;{{$}}
just_a_namespace::NamespaceEnum_wrong ns_var_4;
// CHECK-MESSAGES: :[[@LINE-1]]:19: warning: typename 'NamespaceEnum_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}just_a_namespace::NamespaceEnumWrong ns_var_4;{{$}}

using namespace just_a_namespace;

NamespaceClass_wrong ns_var_5;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: typename 'NamespaceClass_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}NamespaceClassWrong ns_var_5;{{$}}
NamespaceStruct_wrong ns_var_6;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: typename 'NamespaceStruct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}NamespaceStructWrong ns_var_6;{{$}}
NamespaceUnion_wrong ns_var_7;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: typename 'NamespaceUnion_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}NamespaceUnionWrong ns_var_7;{{$}}
NamespaceEnum_wrong ns_var_8;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: typename 'NamespaceEnum_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}NamespaceEnumWrong ns_var_8;{{$}}

using just_a_namespace::NamespaceStruct_wrong;
// CHECK-MESSAGES: :[[@LINE-1]]:25: warning: typename 'NamespaceStruct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}using just_a_namespace::NamespaceStructWrong;{{$}}

int main()
{
  // Definitions
  FirstClassNameCorrect varClass1;
  
  FirstClassNameCorrect::FirstClassNameInnerCorrect varClassInner1;
  FirstClassNameCorrect::FirstClassNameInner_struct_wrong varStructInner1;
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameCorrect::FirstClassNameInnerStructWrong varStructInner1;{{$}}
  FirstClassNameCorrect::FirstClassNameInner_struct_wrong varStructInner2(5);
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameCorrect::FirstClassNameInnerStructWrong varStructInner2(5);{{$}}

  FirstClassNameCorrect::FirstClassNameInner_struct_wrong::Inner_Enum_wrong varEnum1;
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:60: warning: typename 'Inner_Enum_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameCorrect::FirstClassNameInnerStructWrong::InnerEnumWrong varEnum1;{{$}}
  FirstClassNameCorrect::First_Class_Name_Inner_Enum_Wrong varEnum2;
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: typename 'First_Class_Name_Inner_Enum_Wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameCorrect::FirstClassNameInnerEnumWrong varEnum2;{{$}}
  FirstClassNameCorrect::FirstClassNameInnerEnumCorrect varEnum3;

  FirstClassNameCorrect::CorrectTypedefOfClass varTypedef1;
  FirstClassNameCorrect::wrong_typedef_of_class varTypedef2;
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: typename 'wrong_typedef_of_class' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameCorrect::WrongTypedefOfClass varTypedef2;{{$}}
  FirstClassNameCorrect::CorrectTypedefOfStruct varTypedef3;
  FirstClassNameCorrect::wrong_typedef_Of_Struct varTypedef4;
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: typename 'wrong_typedef_Of_Struct' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameCorrect::WrongTypedefOfStruct varTypedef4;{{$}}
  FirstClassNameCorrect::CorrectTypedefOfEnum varTypedef5;
  FirstClassNameCorrect::wrongTypedefOfEnum varTypedef6;
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: typename 'wrongTypedefOfEnum' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameCorrect::WrongTypedefOfEnum varTypedef6;{{$}}
  FirstClassNameCorrect::wrongTypedefOfInnerEnum varTypedef7;
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: typename 'wrongTypedefOfInnerEnum' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameCorrect::WrongTypedefOfInnerEnum varTypedef7;{{$}}
  
  // Usages
  FirstClassNameCorrect::FirstClassNameInner_struct_wrong::a4 = 5;
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameCorrect::FirstClassNameInnerStructWrong::a4 = 5;{{$}}
  dummy1(FirstClassNameCorrect::FirstClassNameInner_struct_wrong::a4);
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  dummy1(FirstClassNameCorrect::FirstClassNameInnerStructWrong::a4);{{$}}
  
  FirstClassNameCorrect::FirstClassNameInner_struct_wrong::FirstClassNameInner_struct_wrong_ClassWrong::a5 = 6;
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:60: warning: typename 'FirstClassNameInner_struct_wrong_ClassWrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameCorrect::FirstClassNameInnerStructWrong::FirstClassNameInnerStructWrongClassWrong::a5 = 6;{{$}}
  dummy1(FirstClassNameCorrect::FirstClassNameInner_struct_wrong::FirstClassNameInner_struct_wrong_ClassWrong::a5);
// CHECK-MESSAGES: :[[@LINE-1]]:33: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:67: warning: typename 'FirstClassNameInner_struct_wrong_ClassWrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  dummy1(FirstClassNameCorrect::FirstClassNameInnerStructWrong::FirstClassNameInnerStructWrongClassWrong::a5);{{$}}
  
  dummy2( FirstClassNameCorrect::FirstClassNameInner_struct_wrong(5) );
// CHECK-MESSAGES: :[[@LINE-1]]:34: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  dummy2( FirstClassNameCorrect::FirstClassNameInnerStructWrong(5) );{{$}}
  FirstClassNameCorrect::FirstClassNameInner_struct_wrong temp1 = FirstClassNameCorrect::FirstClassNameInner_struct_wrong(6);
// CHECK-MESSAGES: :[[@LINE-1]]:26: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-MESSAGES: :[[@LINE-2]]:90: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}  FirstClassNameCorrect::FirstClassNameInnerStructWrong temp1 = FirstClassNameCorrect::FirstClassNameInnerStructWrong(6);{{$}}
  
  return 0;
}


class temp_struct
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: typename 'temp_struct' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}class TempStruct{{$}}
{
int a;
};

FirstClassNameCorrect::FirstClassNameInner_struct_wrong one;
// CHECK-MESSAGES: :[[@LINE-1]]:24: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}FirstClassNameCorrect::FirstClassNameInnerStructWrong one;{{$}}
static FirstClassNameCorrect::FirstClassNameInner_struct_wrong two;
// CHECK-MESSAGES: :[[@LINE-1]]:31: warning: typename 'FirstClassNameInner_struct_wrong' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}static FirstClassNameCorrect::FirstClassNameInnerStructWrong two;{{$}}
temp_struct three;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: typename 'temp_struct' does not follow the camel case naming convention [aliceO2-type-names-naming]
// CHECK-FIXES: {{^}}TempStruct three;{{$}}




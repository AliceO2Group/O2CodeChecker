// RUN: %check_clang_tidy %s aliceO2-function-naming %t

#include <cstdint>
//#include <iostream>

void globalFunctionCorrect() {}
bool isGlobalFunctionCorrect2() { return true; }

void global_function_wrong() {}
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'global_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}void globalFunctionWrong() {}{{$}}
bool globalFunctionWrong2() { return true; }

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

int getFiveCorrect() { return 5; }
int get_five_wrong() { return 5; }
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: function 'get_five_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}int getFiveWrong() { return 5; }{{$}}

class SimConfig
{
private:
  SimConfig()
  {
  };
public:
  static int someInt;
  static SimConfig& Instance()
// CHECK-MESSAGES: :[[@LINE-1]]:21: warning: function 'Instance' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  static SimConfig& instance(){{$}}
  {
    static SimConfig conf;
    return conf;
  }
};

void fooFooFoo()
{
SimConfig::Instance().someInt = 4;
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: function 'Instance' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}SimConfig::instance().someInt = 4;{{$}}
first_namespace::SimConfig::Instance().someInt = 4;
// CHECK-MESSAGES: :[[@LINE-1]]:29: warning: function 'Instance' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}first_namespace::SimConfig::instance().someInt = 4;{{$}}
}

class BaseClass
{
public:
  virtual void needs_fix1() = 0;
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: function 'needs_fix1' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  virtual void needsFix1() = 0;{{$}}
  virtual void needs_fix2() {}
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: function 'needs_fix2' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  virtual void needsFix2() {}{{$}}
};

class MyClass : public BaseClass
{
  public:

    MyClass() {}
    ~MyClass() {}

    void Print(int a=3) const;
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function 'Print' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}    void print(int a=3) const;{{$}}

    void firstFunctionCorrect() {}
    void first_function_wrong() {}
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function 'first_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}    void firstFunctionWrong() {}{{$}}
    bool functionSecondWrong() { return true; }

    static void firstStaticFunctionCorrect() {}
    static void first_static_function_wrong() {}
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: function 'first_static_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}    static void firstStaticFunctionWrong() {}{{$}}
    static bool functionStaticSecondWrong() { return true; }

    void needs_fix1()
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function 'needs_fix1' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}    void needsFix1(){{$}}
    {
    }
    void needs_fix2()
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: function 'needs_fix2' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}    void needsFix2(){{$}}
    {
    }
};

void MyClass::Print(int a) const
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: function 'Print' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}void MyClass::print(int a) const{{$}}
{
}
};

namespace Internal
{
template <typename T, int N>
constexpr int ArraySize() {
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: function 'ArraySize' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}constexpr int arraySize() {{{$}}
  return N / sizeof(T) + ((N%sizeof(T))?1:0);
  }
} // Internal

template <std::size_t N>
struct Descriptor {
  using self_type = Descriptor<N>;
  static int const size = N;
  static constexpr int arraySize = Internal::ArraySize<uint64_t, size>();
// CHECK-MESSAGES: :[[@LINE-1]]:46: warning: function 'ArraySize' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  static constexpr int arraySize = Internal::arraySize<uint64_t, size>();{{$}}

  Descriptor()
  {
    int dummy = Internal::ArraySize<uint64_t, size>();
// CHECK-MESSAGES: :[[@LINE-1]]:27: warning: function 'ArraySize' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}    int dummy = Internal::arraySize<uint64_t, size>();{{$}}
  }

  union {
    char     str[N];
    int  itg[arraySize];
  };

  template <typename T, int NN>
  int dummyTemplate()
  {
    return 0;
  }

  template <typename T, int NN>
  int dummyTemplate_Wrong()
// CHECK-MESSAGES: :[[@LINE-1]]:7: warning: function 'dummyTemplate_Wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  int dummyTemplateWrong(){{$}}
  {
    return 0;
  }
};

void classTemplateFunctionTest()
{
  Descriptor<1> dummyDescriptor;
  dummyDescriptor.dummyTemplate<int,2>();
  dummyDescriptor.dummyTemplate_Wrong<int,2>();
// CHECK-MESSAGES: :[[@LINE-1]]:19: warning: function 'dummyTemplate_Wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  dummyDescriptor.dummyTemplateWrong<int,2>();{{$}}
}

template<class T>
class MyTemplateClass
{
public:

  int myInt;

  bool operator ==(const MyTemplateClass& other) const { return this->myInt == other.myInt; }
  bool operator !=(const MyTemplateClass& other) const { return not this->operator==(other); }

  template<typename OtherT>
  bool operator==(const OtherT*) const = delete;
};

template <typename T, typename... Targs>
constexpr T String2(char c, Targs... Fargs)
// CHECK-MESSAGES: :[[@LINE-1]]:13: warning: function 'String2' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}constexpr T string2(char c, Targs... Fargs){{$}}
{
  T result;
  return result;
}

void someFunction()
{
  uint32_t someInt = String2<uint32_t>('q');
// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: function 'String2' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  uint32_t someInt = string2<uint32_t>('q');{{$}}
}

struct HeartbeatTrailer
{
  HeartbeatTrailer() { HeartbeatTrailer(1); }
  HeartbeatTrailer(int number) { }
  ~HeartbeatTrailer() = default;

  void veryStrangeFunction()
  {
    this->~HeartbeatTrailer();
  }

  uint64_t trailerWord = 0x5100000000000000;
  uint8_t blockType;

  operator bool() const {return trailerWord != 0 && blockType == 0x5;}
  operator uint64_t() const {return trailerWord;}
  int operator +(int rhs) { return 1; }
  int add(int rhs) { return 2; }

  bool operator ==(const HeartbeatTrailer& other) { return this->trailerWord == other.trailerWord; }
  bool operator !=(const HeartbeatTrailer& other) { return !(this->operator==(other)); }
  bool operator ==(const int& otherInt) = delete;
};

int main()
{
  HeartbeatTrailer a1,a2;
  bool varvarBool = a1 == a2;

  // usage
  globalFunctionCorrect();
  isGlobalFunctionCorrect2();

  global_function_wrong();
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: function 'global_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  globalFunctionWrong();{{$}}
  globalFunctionWrong2();

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

  first_namespace::MyClass *myClassVar2 = new first_namespace::MyClass();
  myClassVar2->firstStaticFunctionCorrect();
  myClassVar2->firstFunctionCorrect();
  myClassVar2->first_function_wrong();
// CHECK-MESSAGES: :[[@LINE-1]]:16: warning: function 'first_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  myClassVar2->firstFunctionWrong();{{$}}
  myClassVar2->functionSecondWrong();

  first_namespace::MyClass::firstStaticFunctionCorrect();
  first_namespace::MyClass::first_static_function_wrong();
// CHECK-MESSAGES: :[[@LINE-1]]:29: warning: function 'first_static_function_wrong' does not follow the naming convention [aliceO2-function-naming]
// CHECK-FIXES: {{^}}  first_namespace::MyClass::firstStaticFunctionWrong();{{$}}
  first_namespace::MyClass::functionStaticSecondWrong();

  delete myClassVar2;

  //std::cout << std::endl;
  //using namespace std;
  //cout << endl;

  return 0;
}

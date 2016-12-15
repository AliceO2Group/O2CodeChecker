#include "header.h"

namespace AliceO2 {
  class A {
    double mGood;
    double fBad;

  public:
    void interface1();
    void interface2();
  };

  class B {
  public:
    void foo();
  };

  void consumer(A &a, B &b, C &c){
    a.interface1();
    b.foo();
    c.bar();
  }
}

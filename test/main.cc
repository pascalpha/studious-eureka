//
// Created by admin on 2021/1/19.
//

#include <iostream>
#include "eureka/traits/traits.h"

using namespace eureka;
class X {
 public:
  int x;

  static void p() {}
};
int main(int argc, char* argv[]) {
  decay_t<decltype(main)> s;
  decltype(s) p;
  std::cout << std::boolalpha
            << is_same_v<decay_t<const int[2]>, const int *> << std::endl
            << is_same_v<decay_t<int[2][2]>, int (*)[2]> << std::endl
            << std::endl;
}

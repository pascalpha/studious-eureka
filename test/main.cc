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
int main() {
  std::cout << std::boolalpha
            << is_unsigned_v<int> << std::endl
            << is_unsigned_v<unsigned long long> << std::endl
            << is_unsigned_v<X> << std::endl
            << is_unsigned_v<unsigned char> << std::endl
            << is_unsigned_v<signed short> << std::endl;
}

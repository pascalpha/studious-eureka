//
// Created by admin on 2021/1/19.
//

#include <iostream>
#include "eureka/type_traits.h"

using namespace eureka;
union T {
  int x;
};

class X {
  int x;
};

enum x {

};
int main() {
  std::cout << is_integral_v<x> << std::endl;
  std::cout << is_integral_v<int> << std::endl;
  std::cout << is_integral_v<int &> << std::endl;
  std::cout << is_integral_v<const bool> << std::endl;
  std::cout << is_integral_v<const volatile wchar_t> << std::endl;
}

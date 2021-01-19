//
// Created by admin on 2021/1/19.
//

#include <iostream>

#include "eureka/traits.h"

using namespace eureka;

int k() {
  return 0;
}

class clazz {

};
int main() {
  auto p = []() { return 1; };

  std::cout << is_object_t<int>::value << std::endl;
  std::cout << is_object_t<double>::value << std::endl;
  std::cout << is_object_t<decltype(main)>::value << std::endl;
  std::cout << is_object_t<decltype(p)>::value << std::endl;
  std::cout << is_object_t<clazz>::value << std::endl;
}

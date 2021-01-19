//
// Created by admin on 2021/1/19.
//

#include <iostream>

#include "memory/allocator.h"
#include "container/vector.h"

int main() {
  eureka::vector<int> vec;
  for (int i = 0; i < 100; ++i) {
    vec.push_back(i);
  }
  for (int i = 0; i < 100; ++i) {
    std::cout << vec.back() << std::endl;
    vec.pop_back();
  }
  return 0;
}

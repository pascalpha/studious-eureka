//
// Created by admin on 2021/1/19.
//
#include <vector>
#include <iostream>
#include <string>

#include "eureka/traits/traits.h"
#include "eureka/utility/utility.h"

#include "eureka/memory/pointer_access.h"
#include "eureka/memory/general.h"

using namespace eureka;
class X {
 public:
  explicit X() {
    std::cout << "default" << std::endl;
  };

  X(const X &x) {
    std::cout << "copy" << std::endl;

  };

  X(X &&x) noexcept {
    std::cout << "move" << std::endl;

  }

  explicit X(std::vector<char> &&x) {}
};

template<typename A, typename B, typename C>
struct ptr {
  using et = B;
  using dt = C;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  int x = 1054;
  auto t = pointer_access<ptr<int, int, int>>::pointer_to(x);
}

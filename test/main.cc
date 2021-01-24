//
// Created by admin on 2021/1/19.
//
#include <vector>
#include <iostream>
#include <string>
#include <memory>

#include "eureka/traits/traits.h"
#include "eureka/utility/utility.h"

#include "eureka/memory/pointer.h"
#include "eureka/memory/general.h"
#include "eureka/memory/allocator.h"
#include "eureka/memory/unique_ptr.h"

using namespace eureka;
class X {
  int x = 0;

 public:
  X() {
    std::cout << "default" << std::endl;
  };

  X(const X &x) {
    std::cout << "copy" << std::endl;

  };

  X(X &&x) noexcept {
    std::cout << "move" << std::endl;

  }

  ~X() {
    std::cout << "destruct" << std::endl;
  }
};

template<typename A, typename B, typename C>
struct ptr {
  using et = B;
  using dt = C;
};

using Alloc = allocator<X>;

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  std::cout << is_implicitly_constructible_v<X> << std::endl;
}
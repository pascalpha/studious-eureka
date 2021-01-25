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
#include "eureka/utility/pair.h"

using namespace eureka;
class X {
  int x = 0;

 public:
  explicit X() {
    std::cout << "default" << std::endl;
  };

  X(int x) : x(x) {
    std::cout << "default" << std::endl;
  }

  X(const X &x) {
    std::cout << "copy" << std::endl;
  };

  X(X &&x) noexcept {
    std::cout << "move" << std::endl;
  };

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

void func(pair<X, X> p) {
  std::cout << "func" << std::endl;
}

void test(X p) {
  std::cout << "func" << std::endl;

}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  X x, y(8);
  std::cout << std::is_convertible_v<X &&, X> << std::endl;
  std::cout << pair<X, X>::constraints::implicitly_move_convertible<X, X> << std::endl;
  func({x, y});
}
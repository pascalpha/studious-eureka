//
// Created by admin on 2021/1/19.
//
#include <vector>
#include <iostream>
#include <string>

#include "eureka/traits/traits.h"
#include "eureka/utility/utility.h"

#include "eureka/memory/pointer.h"
#include "eureka/memory/general.h"
#include "eureka/memory/allocator.h"

using namespace eureka;
class X {
  int x = 0;

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
  Alloc alloc;
  X tmp;
  X *x = allocator_access<Alloc>::allocate(alloc, 5);
  for (int i = 0; i < 5; ++i) {
    allocator_access<Alloc>::construct(alloc, x + i, move(tmp));
  }
  for (int i = 0; i < 5; ++i) {
    allocator_access<Alloc>::destroy(alloc, x + i);
  }
  std::cout << allocator_access<Alloc>::max_size(alloc) << std::endl;
}

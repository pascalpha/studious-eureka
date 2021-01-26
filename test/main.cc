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
	std::cout << "default x" << std::endl;
  };

  X(int x) : x(x) {
	std::cout << "default " << x << std::endl;
  }

  X(const X &x) {
	std::cout << "copy x" << std::endl;
  };

  X(X &&x) noexcept {
	std::cout << "move x" << std::endl;
  };

  ~X() {
	std::cout << "destruct x" << std::endl;
  }
};

class Y {
  friend bool constexpr operator<(const Y &, const Y &);
  int y = 1;

 public:
  explicit Y() {
	std::cout << "default y" << std::endl;
  };

  Y(int y) : y(y) {
	std::cout << "default " << y << std::endl;
  }

  Y(const Y &y) {
	std::cout << "copy y" << std::endl;
  };

  Y(Y &&y) noexcept {
	std::cout << "move y" << std::endl;
  };

  Y(const X &x) {
	std::cout << "copy y from x" << std::endl;
  }

  Y(X &&x) {
	std::cout << "move y from x" << std::endl;
  }

  ~Y() {
	std::cout << "destruct y" << std::endl;
  }

  Y &operator=(const Y &x) {
	std::cout << "copy assign" << std::endl;
	return *this;
  }
  Y &operator=(Y &&x) {
	std::cout << "move assign" << std::endl;
	return *this;
  }

  Y &operator=(const X &x) {
	std::cout << "copy assign y from x" << std::endl;
	return *this;
  }
  Y &operator=(X &&x) {
	std::cout << "move assign y from x" << std::endl;
	return *this;
  }
};

inline constexpr bool
operator==(const Y &x, const Y &y) {
  return false;
}
inline constexpr bool
operator<(const Y &x, const Y &y) {
  return x.y < y.y;
}

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
  std::cout << std::boolalpha << is_move_assignable_v<pair<int, int>> << std::endl;
  std::cout << std::boolalpha << is_move_assignable_v<pair<X, X>> << std::endl;
  std::cout << std::boolalpha << is_move_assignable_v<pair<Y, Y>> << std::endl;

}

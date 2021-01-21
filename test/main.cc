//
// Created by admin on 2021/1/19.
//
#include <vector>
#include <iostream>
#include <string>
#include "eureka/traits/traits.h"
#include "eureka/utility/utility.h"

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

int main(int argc, char *argv[]) {

  X x;
  X y;

  X &z = x;
  X &&t = X();

  pair<X, X> r;
  pair<X, X> t1(x, y);
  pair<X, X> t2(z, move(t));
  pair<X, X> t3(x, z);
  pair<X, X> t4(move(x), z);
  pair<X, X> t5(move(x), move(z));
}

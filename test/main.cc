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

int main([[maybe_unused]] int argc, char *argv[]) {

  X x;
  X y;

  std::cout << std::boolalpha
			<< is_constructible_v<X &&, X &> << std::endl
			<< std::is_constructible_v<X &&, X &> << std::endl
	//
	  ;
  std::cout << "..............." << std::endl;
}

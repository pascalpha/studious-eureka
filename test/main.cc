//
// Created by admin on 2021/1/19.
//
#include <vector>
#include <iostream>
#include <string>
#include "eureka/traits/traits.h"

using namespace eureka;
class X {
 public:
  explicit X(double &&x) {}

  explicit X(const std::vector<char> &x) {}

  explicit X(std::vector<char> &&x) {}

  explicit X(const X &x) = delete;

  explicit X(X &&x) = delete;

  ~X() = delete;
};

int main(int argc, char *argv[]) {

  std::cout << std::boolalpha
			<< is_destructible_v<std::vector<int> &> << std::endl
			<< is_destructible_v<int> << std::endl
			<< is_destructible_v<double> << std::endl
			<< is_destructible_v<std::string> << std::endl
			<< is_destructible_v<X&> << std::endl
			<< is_destructible_v<int[][5][7]> << std::endl
			<< is_destructible_v<int[]> << std::endl
			<< is_destructible_v<decltype(main)> << std::endl
			<< is_destructible_v<const volatile void> << std::endl
			<< std::endl;
}

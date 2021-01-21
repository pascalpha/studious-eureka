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
  explicit X(double &&x) {}

  explicit X(const std::vector<char> &x) {}

  explicit X(std::vector<char> &&x) {}

  explicit X(const X &x) = delete;

  explicit X(X &&x) = delete;

  ~X() = delete;
};

int main(int argc, char *argv[]) {
  int x;
  int &y = x;

  std::cout << std::boolalpha
			<< is_same_v<decltype(move(x)), int &&> << std::endl
			<< is_same_v<decltype(move(y)), int &&> << std::endl
			<< is_same_v<int, int &&> << std::endl
			<< is_same_v<decltype(move(y)), int &&> << std::endl
			<< std::endl;
}

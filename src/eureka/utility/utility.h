//
// Created by admin on 2021/1/21.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_UTILITY_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_UTILITY_H_

#include "eureka/traits/transformation.h"

namespace eureka {
/*cast a value to rvalue reference if referencable*/
template<typename Arg>
constexpr auto move(Arg &&arg) noexcept -> decltype(auto) {
  return static_cast<remove_reference_t<Arg> &&>(arg);
}

/*forward a value as lvalue reference*/
/*Arg is lvalue reference type*/
template<typename Arg>
constexpr Arg &&forward(remove_reference_t<Arg> &arg) noexcept {
  return static_cast<Arg &&>(arg);
}

/*forward a value as rvalue reference*/
/*Arg is value type*/
template<typename Arg>
constexpr Arg &&forward(remove_reference_t<Arg> &&arg) noexcept {
  static_assert(!std::is_lvalue_reference<Arg>::value, "forwarding as lvalue reference");
  return static_cast<Arg &&>(arg);
}

template<typename Arg>
constexpr enable_if_t<(conjunction_v < is_move_assignable < Arg > , is_move_constructible < Arg >>), void>
swap(Arg &x, Arg &y) noexcept {
  auto tmp = move(y);
  y = move(x);
  x = move(tmp);
}
}

#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_UTILITY_H_

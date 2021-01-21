//
// Created by admin on 2021/1/21.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_UTILITY_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_UTILITY_H_

#include "eureka/traits/transformation.h"

namespace eureka {
template<typename Arg>
constexpr auto move(Arg &&arg) noexcept -> decltype(auto) {
  return static_cast<remove_reference_t<Arg> &&>(arg);
}

template<typename Arg>
constexpr Arg &&forward(remove_reference_t<Arg> &arg) noexcept {
  return static_cast<Arg &&>(arg);
}

template<typename Arg>
constexpr Arg &&forward(remove_reference_t<Arg> &&arg) noexcept {
  static_assert(!std::is_lvalue_reference<Arg>::value, "forwarding as lvalue reference");
  return static_cast<Arg &&>(arg);
}

template<typename First, typename Second>
struct pair {
  using first_type = First;
  using second_type = Second;

  first_type first;
  second_type second;

  template<typename ArgFirst = First, typename ArgSecond = Second, typename =
  enable_if_t<conjunction_v<is_default_constructible < ArgFirst>, is_default_constructible<ArgSecond>>, detect_t>>
  explicit
  pair() : first(), second() {}

  template<typename ArgFirst, typename ArgSecond, typename =
  enable_if_t<conjunction_v<is_constructible < First, ArgFirst>, is_constructible<Second, ArgSecond>>, detect_t>>
  explicit
  pair(ArgFirst &&f, ArgSecond &&s) : first(forward<ArgFirst>(f)), second(forward<ArgSecond>(s)) {}
};
}

#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_UTILITY_H_

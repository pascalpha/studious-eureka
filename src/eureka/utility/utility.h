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

template<typename First, typename Second>
struct pair {
  using first_type = First;
  using second_type = Second;

  first_type first;
  second_type second;

  template<typename F = First, typename S = Second, typename =
  enable_if_t<conjunction_v<is_default_constructible < F>, is_default_constructible<S>>, detect_t>>
  explicit
  constexpr
  pair() : first(), second() {}

  template<typename F, typename S, typename =
  enable_if_t<conjunction_v<is_constructible < First, F>, is_constructible<Second, S>>, detect_t>>
  explicit
  constexpr
  pair(F &&f, S &&s) : first(forward<F>(f)), second(forward<S>(s)) {}

  template<typename F = First, typename S = Second, typename =
  enable_if_t<conjunction_v<is_constructible < First, F &&>, is_constructible<Second, S &&>>, detect_t>>
  explicit
  constexpr pair(pair<F, S> &&other) : first(forward<F>(other.first)), second(forward<S>(other.second)) {}
};
}

#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_UTILITY_H_

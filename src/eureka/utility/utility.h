//
// Created by admin on 2021/1/21.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_UTILITY_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_UTILITY_H_

#include "eureka/traits/transformation.h"

namespace eureka {
/**
 * \brief cast a value to rvalue reference
 * \tparam Arg
 * \param arg
 * \return
 */
template<typename Arg>
constexpr auto move(Arg &&arg) noexcept -> decltype(auto) {
  return static_cast<remove_reference_t<Arg> &&>(arg);
}

/**
 * \brief forward an lvalue reference
 * \tparam Arg
 * \param arg
 * \return
 */
template<typename Arg>
constexpr Arg &&forward(remove_reference_t<Arg> &arg) noexcept {
  return static_cast<Arg &&>(arg);
}

/**
 * \brief forward an rvalue reference
 * \tparam Arg
 * \param arg
 * \return
 */
template<typename Arg>
constexpr Arg &&forward(remove_reference_t<Arg> &&arg) noexcept {
  static_assert(!std::is_lvalue_reference<Arg>::value, "forwarding as lvalue reference");
  return static_cast<Arg &&>(arg);
}

/**
 * \brief swap two values
 * \tparam Arg
 * \param x
 * \param y
 * \return
 */
template<typename Arg>
constexpr enable_if_t<(conjunction_v < is_move_assignable < Arg > , is_move_constructible < Arg >>), void>
swap(Arg &x, Arg &y) noexcept {
  auto tmp = move(y);
  y = move(x);
  x = move(tmp);
}

/**
 * \brief replaces value of object the a value and returns the old value
 * \tparam Arg
 * \tparam Param
 * \param obj
 * \param value
 * \return
 */
template<typename Arg, typename Param = Arg>
constexpr Arg exchange(Arg &obj, Param &&value) {
  Arg old = move(obj);
  obj = forward<Param>(value);
  return old;
}
}

#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_UTILITY_H_

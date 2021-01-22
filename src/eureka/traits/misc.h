//
// Created by admin on 2021/1/21.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_MISC_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_MISC_H_

#include "general.h"
#include "categorization.h"

namespace eureka {
template<typename Arg>
struct decay {
 protected:
  using unreferenced = remove_reference_t<Arg>;
 public:
  using type = conditional_t< /*if*/
  is_array_v<unreferenced>, /*is array*/
  remove_extent_t<unreferenced> *, /*then decay first dimension*/
  conditional_t< /*else if*/
  is_function_v<unreferenced>, /*is function*/
  add_pointer_t<unreferenced>, /*decay to pointer*/
  remove_const_volatile_t<unreferenced> /*remove cv qualifiers*/
  >>;
};
template<typename Arg>
using decay_t = typename decay<Arg>::type;

namespace _impl {
/*referencable types here, rvalue references to functions resolve to an lvalue reference instead*/
template<typename Arg, typename RvalueRef = Arg &&>
RvalueRef &&declared_value_impl(nullptr_t);
/*or reference types, void_t here*/
template<typename Arg>
Arg declared_value_impl(nullptr_t *);
template<typename...>
struct declared_value_invocation_stopper : false_t {};
} // namespace _impl
template<typename Arg>
decltype(_impl::declared_value_impl<Arg>(nullptr)) // explicitly naming return type can suppress compiler complaints
declared_value() {
  static_assert(_impl::declared_value_invocation_stopper<Arg>::value, "invocation of declared_value() forbidden");
  return _impl::declared_value_impl<Arg>(nullptr);
}

template<typename...>
using void_t = void;
template<typename...>
using valid_t = void;
// used for template deduction
using placeholder_t = void;
} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_MISC_H_

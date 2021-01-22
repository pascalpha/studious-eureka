//
// Created by admin on 2021/1/22.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_POINTER_ACCESS_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_POINTER_ACCESS_H_

#include "eureka/traits/traits.h"

#include "general.h"

namespace eureka {

eureka_member_type_helper_macro(pointer_t);
eureka_member_type_helper_macro(element_t);
eureka_member_type_helper_macro(difference_t);

template<typename Template>
struct first_template_argument {};
template<template<typename, typename ...> typename Template, typename First, typename ... Args>
struct first_template_argument<Template<First, Args...>> { using type = First; };
template<typename Template>
using first_template_argument_t = typename first_template_argument<Template>::type;

template<typename Template, typename Target>
struct rebind_first_template_argument {};
template<template<typename, typename ...> typename Template, typename First, typename... Args, typename Target>
struct rebind_first_template_argument<Template<First, Args...>, Target> { using type = Template<Target, Args...>; };
template<typename Template, typename Target>
using rebind_first_template_argument_t = typename rebind_first_template_argument<Template, Target>::type;

namespace _impl {
/*element is Ptr::element_t if present*/
template<typename Ptr>
auto element_impl(nullptr_t) -> typename Ptr::element_t;
/*or Arg if Ptr = Template<Arg, ...>*/
template<typename Ptr>
auto element_impl(nullptr_t *) -> first_template_argument_t<Ptr>;
/*else ill-formed*/

/*difference is Ptr::difference_t if present*/
template<typename Ptr>
auto difference_impl(nullptr_t) -> typename Ptr::difference_t;
/*or ptrdiff_t if not*/
template<typename Ptr>
auto difference_impl(nullptr_t *) -> ptrdiff_t;

template<typename Ptr, typename Rebind>
auto rebind_impl(nullptr_t) -> typename Ptr::template rebind<Rebind>;
template<typename Ptr, typename Rebind>
auto rebind_impl(nullptr_t *) -> rebind_first_template_argument_t<Ptr, Rebind>;
} //  namespace _impl

template<typename Ptr>
struct pointer_access {
  using pointer_t = Ptr;
  using element_t = decltype(_impl::element_impl<Ptr>(nullptr));
  using difference_t = decltype(_impl::difference_impl<Ptr>(nullptr));

  template<typename Rebind>
  using rebind = decltype(_impl::rebind_impl<Ptr, Rebind>(nullptr));

  static pointer_t pointer_to(element_t &v) {
    return Ptr::pointer_to(v);
  }
};

template<typename Arg>
struct pointer_access<Arg *> {
  using pointer_t = Arg *;
  using element_t = Arg;
  using difference_t = ptrdiff_t;

  template<typename Rebind>
  using rebind = Rebind *;

  static constexpr pointer_t pointer_to(element_t &v) {
    return address_of(v);
  }
};
} //  namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_POINTER_ACCESS_H_

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-forwarding-reference-overload"
#pragma ide diagnostic ignored "google-explicit-constructor"
//
// Created by admin on 2021/1/26.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_FUNCTIONAL_REFERENCE_WRAPPER_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_FUNCTIONAL_REFERENCE_WRAPPER_H_

#include "eureka/traits/traits.h"
#include "eureka/utility/utility.h"
#include "eureka/memory/pointer.h"

namespace eureka {
namespace _impl {
template<typename Class>
Class *reference_wrapper_impl(Class &r) noexcept { return address_of(r); }
template<typename Class>
[[maybe_unused]] void reference_wrapper_impl(Class &&r) = delete;
}

// todo bugfix
template<typename Class>
class reference_wrapper {
 public:
  Class *ptr;

  template<typename Arg>
  using argument_constraints
  = enable_if_t<!is_same_v < reference_wrapper, remove_const_volatile_reference_t<Arg>>, placeholder_t>;

 public:
  using type [[maybe_unused]] = Class;

  template<typename Arg, typename = argument_constraints<Arg>,
	  typename = decltype(_impl::reference_wrapper_impl(declared_value<Arg>()))>
  reference_wrapper(Arg &&ref) noexcept(noexcept(_impl::reference_wrapper_impl(declared_value<Arg>()))):
	  ptr(_impl::reference_wrapper_impl(forward<Arg>(ref))) {}

  reference_wrapper(const reference_wrapper &) = default;

  reference_wrapper &operator=(const reference_wrapper &) = default;

  reference_wrapper(reference_wrapper &&) = delete;

  reference_wrapper &operator=(reference_wrapper &&) = delete;

  Class &get() const noexcept { return *ptr; }

  operator Class &() const noexcept { return this->get(); }
}; // class reference_wrapper

template<typename Arg>
reference_wrapper(Arg &) -> reference_wrapper<Arg>;
} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_FUNCTIONAL_REFERENCE_WRAPPER_H_
#pragma clang diagnostic pop
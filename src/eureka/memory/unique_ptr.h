//
// Created by mamin on 1/24/2021.
//
#ifndef STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_UNIQUE_PTR_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_UNIQUE_PTR_H_

#include "eureka/traits/traits.h"

namespace eureka {

template<typename Class>
struct default_delete {
  constexpr default_delete() noexcept = default;

  template<typename Target, typename = enable_if_t<is_convertible_v<Target *, Class *>, placeholder_t>>
  /*this disallows the implicit conversion from deleter for derived class to deleter for base class,
   *as is the reason why the conversion constructor is present in the first place.
   *Standard implementation does not mark this constructor as explicit to allow this, which is resented*/
  [[maybe_unused]] explicit
  default_delete(const default_delete<Target> &) noexcept {};

  void operator()(Class *ptr) const {
	delete ptr;
  }
};

template<typename Class>
struct default_delete<Class[]> {
  constexpr default_delete() noexcept = default;

  template<typename Target>
  /*See above*/
  [[maybe_unused]] explicit
  default_delete(const default_delete<Target[]> &other) noexcept {}

  template<typename Target, typename = enable_if_t<is_convertible_v<Class(*)[], Target(*)[]>, placeholder_t>>
  void operator()(Target *ptr) const {
	delete[] ptr;
  }
};

namespace _impl {
template<typename, typename Deleter>
auto unique_ptr_pointer_impl(nullptr_t) -> typename remove_reference_t<Deleter>::pointer_t;
template<typename Class, typename>
auto unique_ptr_pointer_impl(nullptr_t *) -> Class *;

template<typename Deleter>
using unique_ptr_deleter_select = boolean_constant<is_default_constructible_v<Deleter> && !is_pointer_v<Deleter>>;
eureka_value_helper_macro(unique_ptr_deleter_select);
} // _impl

template<typename Class, typename Deleter = default_delete<Class>>
class unique_ptr {
 public:
  using element_t = Class;
  using pointer_t = decltype(_impl::unique_ptr_pointer_impl<Class, Deleter>(nullptr));
  using deleter_t = Deleter;

 private:
  pointer_t ptr;

  deleter_t deleter;
};

template<typename Class, typename Deleter>
class unique_ptr<Class[], Deleter> {
 public:
  using element_t = Class;
  using pointer_t = decltype(_impl::unique_ptr_pointer_impl<Class, Deleter>(nullptr));
  using deleter_t = Deleter;
};
} // eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_UNIQUE_PTR_H_

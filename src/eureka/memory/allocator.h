//
// Created by admin on 2021/1/22.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_ALLOCATOR_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_ALLOCATOR_H_

#include "eureka/traits/traits.h"

#include "general.h"
#include "pointer.h"

namespace eureka {
template<typename T>
struct allocator {
  /*types*/
  using value_t = T;
  using size_t = eureka::size_t;
  using difference_t = eureka::ptrdiff_t;
  /*propagation*/
  using propagate_on_container_move_assign = true_t;

  /*constructors*/
  constexpr explicit allocator() noexcept = default;
  constexpr allocator(const allocator &_) noexcept = default;
  template<typename U>
  constexpr explicit allocator(const allocator<U> &_) noexcept {};

  /*destructors*/
  ~allocator() = default;

  constexpr T *allocate(size_t n) {
    return static_cast<T *>(::operator new(sizeof(T) * n));
  }

  constexpr void deallocate(T *ptr, [[maybe_unused]] size_t n) {
    ::operator delete(ptr);
  }
};

template<typename Class, typename... Args>
constexpr Class *construct_at(Class *ptr, Args &&... args) {
  return ::new(const_cast<void *>(static_cast<const volatile void *>(ptr))) Class(forward<Args>(args)...);
}

template<typename Class>
constexpr void destroy_at(Class *ptr) { ptr->~Class(); }

namespace _impl {
template<typename Alloc, typename = placeholder_t>
struct allocator_pointer_impl {
  using type = typename Alloc::value_t *;
};
template<typename Alloc>
struct allocator_pointer_impl<Alloc, enable_if_t < has_pointer_t<Alloc>, placeholder_t>> {
using type = typename Alloc::pointer_t;
};

template<typename Alloc, typename = placeholder_t>
struct allocator_const_pointer_impl {
  using type = typename pointer_access<typename _impl::allocator_pointer_impl<Alloc>::type>
  ::template rebind<const typename Alloc::value_t>;
};
template<typename Alloc>
struct allocator_const_pointer_impl<Alloc, enable_if_t < has_const_pointer_t<Alloc>, placeholder_t>> {
using type = typename Alloc::const_pointer_t;
};

template<typename Alloc, typename = placeholder_t>
struct allocator_void_pointer_impl {
  using type = typename pointer_access<typename _impl::allocator_pointer_impl<Alloc>::type>
  ::template rebind<void>;
};
template<typename Alloc>
struct allocator_void_pointer_impl<Alloc, enable_if_t < has_void_pointer_t<Alloc>, placeholder_t>>{
using type = typename Alloc::void_pointer_t;
};

template<typename Alloc, typename = placeholder_t>
struct allocator_const_void_pointer_impl {
  using type = typename pointer_access<typename _impl::allocator_pointer_impl<Alloc>::type>
  ::template rebind<const void>;
};
template<typename Alloc>
struct allocator_const_void_pointer_impl<Alloc, enable_if_t < has_const_void_pointer_t<Alloc>, placeholder_t>>{
using type = typename Alloc::const_void_pointer_t;
};

template<typename Alloc, typename = placeholder_t>
struct allocator_difference_impl {
  using type = typename pointer_access<typename _impl::allocator_pointer_impl<Alloc>::type>::difference_t;
};
template<typename Alloc>
struct allocator_difference_impl<Alloc, enable_if_t < has_difference_t<Alloc>, placeholder_t>>{
using type = typename Alloc::difference_t;
};

template<typename Alloc, typename = placeholder_t>
struct allocator_size_impl {
  // TODO make_unsigned<difference_t>
  using type = eureka::size_t;
};
template<typename Alloc>
struct allocator_size_impl<Alloc, enable_if_t < has_size_t<Alloc>, placeholder_t>>{
using type = typename Alloc::size_t;
};

template<typename Alloc>
auto allocator_propagate_on_container_copy_impl(nullptr_t) -> false_t;
template<typename Alloc>
auto allocator_propagate_on_container_copy_impl(nullptr_t *) -> typename Alloc::propagate_on_container_copy_assign;

template<typename Alloc>
auto allocator_propagate_on_container_move_impl(nullptr_t) -> false_t;
template<typename Alloc>
auto allocator_propagate_on_container_move_impl(nullptr_t *) -> typename Alloc::propagate_on_container_move_assign;

template<typename Alloc>
auto allocator_propagate_on_container_swap_impl(nullptr_t) -> false_t;
template<typename Alloc>
auto allocator_propagate_on_container_swap_impl(nullptr_t *) -> typename Alloc::propagate_on_container_swap;

template<typename Alloc>
auto allocator_is_always_equal_impl(nullptr_t) -> typename is_empty<Alloc>::type;
template<typename Alloc>
auto allocator_is_always_equal_impl(nullptr_t *) -> typename Alloc::is_always_equal;

template<typename Alloc, typename Target>
auto allocator_rebind_impl(nullptr_t) -> typename Alloc::template rebind<Target>::other;
template<typename Alloc, typename Target>
auto allocator_rebind_impl(nullptr_t *) -> rebind_first_template_argument_t<Alloc, Target>;
} //  namespace _impl

template<typename Alloc>
struct allocator_access {
  static_assert(has_value_t<Alloc>, "no value_t in allocator type");
  /*allocator type*/
  using allocator_t = Alloc;
  /*value type, inherit from Alloc*/
  using value_t = typename Alloc::value_t;
  /*pointer type, inherit from Alloc or rebind using pointer trait*/
  using pointer_t = typename _impl::allocator_pointer_impl<Alloc>::type;
  /*const pointer type, inherit from Alloc or rebind using pointer trait*/
  using const_pointer_t = typename _impl::allocator_const_pointer_impl<Alloc>::type;
  /*void pointer type, inherit from Alloc or rebind using pointer trait*/
  using void_pointer_t = typename _impl::allocator_void_pointer_impl<Alloc>::type;
  /*const void pointer type, inherit from Alloc or rebind using pointer trait*/
  using const_void_pointer_t = typename _impl::allocator_const_void_pointer_impl<Alloc>::type;
  /*difference type, inherit from Alloc or rebind using pointer trait*/
  using difference_t = typename _impl::allocator_difference_impl<Alloc>::type;
  /*size type, inherit from Alloc or size_t. See inline TODO*/
  using size_t = typename _impl::allocator_size_impl<Alloc>::type;
  /*copy propagation type, inherit from Alloc or false_t*/
  using propagate_on_container_copy_assign = decltype(_impl::allocator_propagate_on_container_copy_impl<Alloc>(nullptr));
  /*move propagation type, inherit from Alloc or false_t*/
  using propagate_on_container_move_assign = decltype(_impl::allocator_propagate_on_container_move_impl<Alloc>(nullptr));
  /*swap propagation type, inherit from Alloc or false_te*/
  using propagate_on_container_swap = decltype(_impl::allocator_propagate_on_container_swap_impl<Alloc>(nullptr));
  /*statelessness, inherit from Alloc, or true_t if stateless, false_t otherwise*/
  using is_always_equal = decltype(_impl::allocator_is_always_equal_impl<Alloc>(nullptr));

  template<typename Target>
  using rebind_allocator = decltype(_impl::allocator_rebind_impl<Alloc, Target>(nullptr));

  template<typename Target>
  using rebind_allocator_access = allocator_access<rebind_allocator<Target>>;

  [[nodiscard]] static constexpr
  pointer_t allocate(Alloc &alloc, size_t n) {
    return alloc.allocate(n);
  }

  static constexpr
  void deallocate(Alloc &alloc, pointer_t ptr, size_t n) {
    alloc.deallocate(ptr, n);
  }

  template<typename Class, typename... Args>
  static constexpr void construct(Alloc &alloc, Class *ptr, Args &&... args) {
    construct_at(ptr, forward<Args>(args)...);
  }

  template<typename Class>
  static constexpr void destroy(Alloc &alloc, Class *ptr) {
    destroy_at(ptr);
  }

  static constexpr size_t max_size(const Alloc &alloc) noexcept {
    return static_cast<size_t>(-1) / sizeof(value_t);
  }
};
}  // eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_ALLOCATOR_H_

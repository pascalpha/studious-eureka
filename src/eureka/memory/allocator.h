//
// Created by admin on 2021/1/22.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_ALLOCATOR_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_ALLOCATOR_H_

#include "eureka/traits/traits.h"

namespace eureka {
template<typename Arg>
struct allocator {
  using value_t = Arg;
  using size_t = eureka::size_t;
  using difference_t = eureka::ptrdiff_t;
  using propagate_on_container_move_assignment = true_t;

  constexpr allocator() noexcept = default;

  constexpr allocator(const allocator &other) = default;

  template<typename Rebind>
  constexpr explicit allocator(const allocator<Rebind> &other) {};

  constexpr ~allocator() = default;

  [[nodiscard]] constexpr Arg *allocate(size_t n) {
	// throws bac_array_new_length
	return ::operator new(sizeof(Arg) * n);
  }

  constexpr void deallocate(T *ptr, size_t n) {
	::operator delete(ptr);
  }
};

template<typename A, typename B>
constexpr bool operator==(const allocator<A> &f, const allocator<B> &s) {
  // stateless allocator
  return true;
}

template<typename Alloc>
struct allocator_access {
  using allocator_t = Alloc;
  using value_t = Alloc::value_t;
  using pointer_t = value_t *;
  using const_pointer_t =
};
}  // eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_ALLOCATOR_H_

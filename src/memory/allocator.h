//
// Created by admin on 2021/1/19.
//

#ifndef STUDIOUS_EUREKA_SRC_MEMORY_ALLOCATOR_H_
#define STUDIOUS_EUREKA_SRC_MEMORY_ALLOCATOR_H_

#include <cstddef>
#include <vector>
#include <iostream>

namespace eureka {
template<typename Alloca>
class allocator_traits {
 public:
  using value_t = typename Alloca::value_t;
  using pointer_t = typename Alloca::pointer_t;
  using const_pointer_t = typename Alloca::const_pointer_t;
  using reference_t = typename Alloca::reference_t;
  using const_reference_t = typename Alloca::const_reference_t;
  using size_t = typename Alloca::size_t;
  using difference_t = typename Alloca::difference_t;
};

template<typename T>
class allocator {
 public:
  using value_t = T;
  using pointer_t = T *;
  using const_pointer_t = const T *;
  using reference_t = T &;
  using const_reference_t = const T &;
  using size_t = std::size_t;
  using difference_t = std::ptrdiff_t;

  template<typename U>
  struct rebind {
    using other = allocator<U>;
  };

  explicit allocator() = default;

  ~allocator() = default;

  inline
  pointer_t allocate(size_t num) {
    return static_cast<pointer_t>(::operator new(num * sizeof(value_t)));
  }

  inline
  void deallocate(pointer_t p, size_t num) {
    ::operator delete(p);
  }
};
}

#endif //STUDIOUS_EUREKA_SRC_MEMORY_ALLOCATOR_H_

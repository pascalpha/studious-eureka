//
// Created by admin on 2021/1/19.
//

#ifndef STUDIOUS_EUREKA_SRC_CONTAINER_VECTOR_H_
#define STUDIOUS_EUREKA_SRC_CONTAINER_VECTOR_H_

#include "memory/allocator.h"

namespace eureka {
template<typename T, typename Alloca = allocator<T>>
class vector {
 public:
  Alloca alloca;

  explicit vector() = default;

  ~vector() = default;

  void push_back(const T &t) {
    if (current_vector_size < current_vector_capacity) {
      underlying[current_vector_size++] = t;
      return;
    }
    current_vector_capacity = current_vector_capacity ? current_vector_capacity * 2 : 1;
    T *tmp = alloca.allocate(current_vector_capacity);
    for (size_t i = 0; i < current_vector_size; ++i) {
      tmp[i] = underlying[i];
    }
    alloca.deallocate(underlying, current_vector_capacity);
    underlying = tmp;
    underlying[current_vector_size++] = t;
  }

  void pop_back() {
    current_vector_size -= 1;
  }

  const T &back() {
    if (!current_vector_size) throw "empty";
    return underlying[current_vector_size - 1];
  }

 private:
  T *underlying = nullptr;

  size_t current_vector_size = 0;

  size_t current_vector_capacity = 0;
};
} // namespace eureka
#endif //STUDIOUS_EUREKA_SRC_CONTAINER_VECTOR_H_

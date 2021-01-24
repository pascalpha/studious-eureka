//
// Created by mamin on 1/25/2021.
//
#ifndef STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_PAIR_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_PAIR_H_

#include "eureka/traits/traits.h"

namespace eureka {

namespace _impl {



} // _impl

template<typename F, typename S>
struct pair {
  using first_t = F;
  using second_t = S;

  first_t first;
  second_t second;
};
} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_PAIR_H_

//
// Created by mamin on 1/23/2021.
//
#ifndef STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_GENERAL_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_GENERAL_H_

#include "eureka/traits/traits.h"

namespace eureka {
template<typename Arg>
enable_if_t<is_object_v<Arg>, Arg *> address_of(Arg &arg) noexcept {
  return reinterpret_cast<Arg *>(&const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
}

template<typename Arg>
enable_if_t<!is_object_v<Arg>, Arg *> address_of(Arg &arg) noexcept {
  return &arg;
}
} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_GENERAL_H_

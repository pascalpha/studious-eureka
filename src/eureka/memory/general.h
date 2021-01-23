//
// Created by mamin on 1/23/2021.
//
#ifndef STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_GENERAL_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_GENERAL_H_

#include "eureka/traits/traits.h"

namespace eureka {

eureka_member_type_helper_macro(size_t);
eureka_member_type_helper_macro(difference_t);

eureka_member_type_helper_macro(value_t);
eureka_member_type_helper_macro(pointer_t);
eureka_member_type_helper_macro(const_pointer_t);
eureka_member_type_helper_macro(void_pointer_t);
eureka_member_type_helper_macro(const_void_pointer_t);
eureka_member_type_helper_macro(element_t);

eureka_member_type_helper_macro(propagate_on_container_copy_assign);
eureka_member_type_helper_macro(propagate_on_container_move_assign);
eureka_member_type_helper_macro(propagate_on_container_swap);

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

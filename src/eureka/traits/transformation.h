//
// Created by mamin on 1/20/2021.
//
#ifndef STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_TRANSFORMATION_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_TRANSFORMATION_H_

namespace eureka {
template<typename Arg>
struct remove_const { using type = Arg; };
template<typename Arg>
struct remove_const<const Arg> { using type = Arg; };
template<typename Arg>
using remove_const_t = typename remove_const<Arg>::type;

template<typename Arg>
struct remove_volatile { using type = Arg; };
template<typename Arg>
struct remove_volatile<volatile Arg> { using type = Arg; };
template<typename Arg>
using remove_volatile_t = typename remove_volatile<Arg>::type;

template<typename Arg>
struct remove_const_volatile { using type = Arg; };
template<typename Arg>
struct remove_const_volatile<const Arg> { using type = Arg; };
template<typename Arg>
struct remove_const_volatile<volatile Arg> { using type = Arg; };
template<typename Arg>
struct remove_const_volatile<const volatile Arg> { using type = Arg; };
template<typename Arg>
using remove_const_volatile_t = typename remove_const_volatile<Arg>::type;

template<typename Arg>
struct add_const { using type = const Arg; };
template<typename Arg>
using add_const_t = typename add_const<Arg>::type;

template<typename Arg>
struct add_volatile { using type = volatile Arg; };
template<typename Arg>
using add_volatile_t = typename add_volatile<Arg>::type;

template<typename Arg>
struct add_const_volatile { using type = const volatile Arg; };
template<typename Arg>
using add_const_volatile_t = typename add_const_volatile<Arg>::type;

} // namespace eureka
#endif //STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_TRANSFORMATION_H_

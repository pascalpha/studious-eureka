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

template<typename Arg>
struct remove_reference { using type = Arg; };
template<typename Arg>
struct remove_reference<Arg &> { using type = Arg; };
template<typename Arg>
struct remove_reference<Arg &&> { using type = Arg; };
template<typename Arg>
using remove_reference_t = typename remove_reference<Arg>::type;

template<typename Arg>
struct remove_pointer { using type = Arg; };
template<typename Arg>
struct remove_pointer<Arg *> { using type = Arg; };
template<typename Arg>
struct remove_pointer<const Arg *> { using type = Arg; };
template<typename Arg>
struct remove_pointer<volatile Arg *> { using type = Arg; };
template<typename Arg>
struct remove_pointer<const volatile Arg *> { using type = Arg; };
template<typename Arg>
using remove_pointer_t = typename remove_pointer<Arg>::type;

template<typename Arg>
struct remove_const_volatile_reference { using type = remove_const_volatile_t<remove_reference_t<Arg>>; };
template<typename Arg>
using remove_const_volatile_reference_t = typename remove_const_volatile_reference<Arg>::type;


template<typename Arg>
struct remove_extent { using type = Arg; };
template<typename Arg>
struct remove_extent<Arg[]> { using type = Arg; };
template<typename Arg, size_t Num>
struct remove_extent<Arg[Num]> { using type = Arg; };
template<typename Arg>
using remove_extent_t = typename remove_extent<Arg>::type;


template<typename Arg>
struct remove_all_extents { using type = Arg; };
template<typename Arg>
struct remove_all_extents<Arg[]> { using type = typename remove_all_extents<Arg>::type; };
template<typename Arg, size_t Num>
struct remove_all_extents<Arg[Num]> { using type = typename remove_all_extents<Arg>::type; };
template<typename Arg>
using remove_all_extents_t = typename remove_all_extents<Arg>::type;

namespace _impl {
template<typename Arg>
type_identity<Arg &> add_lvalue_reference_impl(nullptr_t);
template<typename Arg>
type_identity<Arg> add_lvalue_reference_impl(...);
template<typename Arg>
type_identity<Arg &&> add_rvalue_reference_impl(nullptr_t);
template<typename Arg>
type_identity<Arg> add_rvalue_reference_impl(...);
} // namespace _impl
template<typename Arg>
struct add_lvalue_reference : decltype(_impl::add_lvalue_reference_impl<Arg>(nullptr)) {};
template<typename Arg>
using add_lvalue_reference_t = typename add_lvalue_reference<Arg>::type;
template<typename Arg>
struct add_rvalue_reference : decltype(_impl::add_rvalue_reference_impl<Arg>(nullptr)) {};
template<typename Arg>
using add_rvalue_reference_t = typename add_rvalue_reference<Arg>::type;

namespace _impl {
template<typename Arg>
type_identity<remove_reference_t<Arg> *> add_pointer_impl(nullptr_t);
template<typename Arg>
type_identity<Arg> add_pointer_impl(...);
} // namespace _impl
template<typename Arg>
struct add_pointer : decltype(_impl::add_pointer_impl<Arg>(nullptr)) {};
template<typename Arg>
using add_pointer_t = typename add_pointer<Arg>::type;


} // namespace eureka
#endif //STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_TRANSFORMATION_H_

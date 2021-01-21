//
// Created by mamin on 1/20/2021.
//
#ifndef STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_ABILITY_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_ABILITY_H_

#include "general.h"
#include "categorization.h"
#include "misc.h"

namespace eureka {
namespace _impl {
template<typename, typename Class, typename... Args>
struct is_constructible_impl : false_t {};
// use global new operator to avoid C-style cast
template<typename Class, typename... Args>
struct is_constructible_impl<valid_t<decltype(::new Class(declared_value<Args>()...))>, Class, Args...> : true_t {};
} // namespace _impl
template<typename Class, typename... Arg>
using is_constructible = _impl::is_constructible_impl<detect_t, Class, Arg...>;
template<typename Class, typename... Arg>
constexpr typename is_constructible<Class, Arg...>::value_t is_constructible_v = is_constructible<Class, Arg...>::value;

template<typename Class>
using is_default_constructible = is_constructible<Class>;
template<typename Class>
constexpr typename is_default_constructible<Class>::value_t
	is_default_constructible_v = is_default_constructible<Class>::value;

template<typename Class>
using is_copy_constructible = is_constructible<Class, add_lvalue_reference_t<add_const_t<Class>>>;
template<typename Class>
constexpr typename is_copy_constructible<Class>::value_t
	is_copy_constructible_v = is_copy_constructible<Class>::value;

template<typename Class>
using is_move_constructible = is_constructible<Class, add_rvalue_reference_t<Class>>;
template<typename Class>
constexpr typename is_move_constructible<Class>::value_t
	is_move_constructible_v = is_move_constructible<Class>::value;

namespace _impl {
template<typename, typename Class, typename Other>
struct is_assignable_impl : false_t {};
template<typename Class, typename Other>
struct is_assignable_impl<
	valid_t<decltype(declared_value<Class>() = declared_value<Other>())>, Class, Other> : true_t {
};
}
template<typename Class, typename Other>
using is_assignable = _impl::is_assignable_impl<detect_t, Class, Other>;
template<typename Class, typename Other>
constexpr typename is_assignable<Class, Other>::value_t is_assignable_v = is_assignable<Class, Other>::value;

template<typename Class>
using is_copy_assignable = is_assignable<Class, add_lvalue_reference_t<add_const_t<Class>>>;
template<typename Class>
constexpr typename is_copy_assignable<Class>::value_t
	is_copy_assignable_v = is_copy_assignable<Class>::value;

template<typename Class>
using is_move_assignable = is_assignable<Class, add_rvalue_reference_t<Class>>;
template<typename Class>
constexpr typename is_move_assignable<Class>::value_t
	is_move_assignable_v = is_move_assignable<Class>::value;

namespace _impl {
/*destructor not exists*/
template<typename, typename Class>
struct try_destruct_impl : false_t {};
/*destructor does exist*/
template<typename Class>
struct try_destruct_impl<valid_t<decltype(declared_value<Class &>().~Class())>, Class> : true_t {};
/*general case*/
template<typename, typename Class>
struct is_destructible_impl : false_t {};
/*reference types*/
template<typename Class>
struct is_destructible_impl<enable_if_t<is_reference_v<Class>, detect_t>, Class> : true_t {};
/*const volatile qualified void*/
template<typename Class>
struct is_destructible_impl<enable_if_t<is_void_v<Class>, detect_t>, Class> : false_t {};
/*function types*/
template<typename Class>
struct is_destructible_impl<enable_if_t<is_function_v<Class>, detect_t>, Class> : false_t {};
/*unbounded arrays*/
template<typename Class>
struct is_destructible_impl<enable_if_t<is_unbounded_array_v<Class>, detect_t>, Class> : false_t {};
/*bounded arrays, candidate for trying destructing*/
template<typename Class, size_t Num>
struct is_destructible_impl<detect_t, Class[Num]> : try_destruct_impl<detect_t, remove_all_extents_t<Class>> {};
/*object type, candidate for trying destructing*/
template<typename Class>
struct is_destructible_impl<enable_if_t<is_object_v<Class> && !is_array_v<Class>, detect_t>, Class>
	: try_destruct_impl<detect_t, remove_all_extents_t<Class>> {
};
}  // namespace _impl
template<typename Class>
using is_destructible = _impl::is_destructible_impl<detect_t, Class>;
template<typename Class>
constexpr typename is_destructible<Class>::value_t is_destructible_v = is_destructible<Class>::value;
} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_ABILITY_H_

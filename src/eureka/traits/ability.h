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
template<typename, typename, typename Class, typename... Args>
struct is_constructible_impl : false_t {};
template<typename Class, typename... Args>
struct is_constructible_impl<
	enable_if_t<(is_reference_v<Class> || is_object_v<Class>)
					&& !(is_rvalue_reference_v<Class> && conjunction_v<is_lvalue_reference<Args>...>), placeholder_t>,
	valid_t<decltype(Class(declared_value<Args>()...))>, Class, Args...> : true_t {
};
template<typename Class, typename ... Args>
struct is_constructible_impl<enable_if_t<!is_reference_v<Class> && !is_object_v<Class>, placeholder_t>,
							 valid_t<decltype(::new Class(declared_value<Args>()...))>, Class, Args...>
	: true_t {
};
template<typename Class>
struct is_constructible_impl<enable_if_t<!is_reference_v<Class>, placeholder_t>, placeholder_t, Class &&, Class &>
	: false_t {
};
} // namespace _impl
template<typename Class, typename... Arg>
using is_constructible = _impl::is_constructible_impl<placeholder_t, placeholder_t, Class, Arg...>;
template<typename Class, typename... Arg>
constexpr typename is_constructible<Class, Arg...>::value_t is_constructible_v = is_constructible<Class, Arg...>::value;

template<typename Class>
using is_default_constructible = is_constructible<Class>;
eureka_value_helper_macro(is_default_constructible);

template<typename Class>
using is_copy_constructible = is_constructible<Class, add_lvalue_reference_t<add_const_t<Class>>>;
eureka_value_helper_macro(is_copy_constructible);

template<typename Class>
using is_move_constructible = is_constructible<Class, add_rvalue_reference_t<Class>>;
eureka_value_helper_macro(is_move_constructible);

namespace _impl {
template<typename, typename Class, typename Other>
struct is_assignable_impl : false_t {};
template<typename Class, typename Other>
struct is_assignable_impl<
	valid_t<decltype(declared_value<Class>() = declared_value<Other>())>, Class, Other> : true_t {
};
}
template<typename Class, typename Other>
using is_assignable = _impl::is_assignable_impl<placeholder_t, Class, Other>;
template<typename Class, typename Other>
constexpr typename is_assignable<Class, Other>::value_t is_assignable_v = is_assignable<Class, Other>::value;

template<typename Class>
using is_copy_assignable = is_assignable<Class &, add_lvalue_reference_t<add_const_t<Class>>>;
eureka_value_helper_macro(is_copy_assignable);

template<typename Class>
using is_move_assignable = is_assignable<Class &, add_rvalue_reference_t<Class>>;
eureka_value_helper_macro(is_move_assignable);

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
struct is_destructible_impl<enable_if_t<is_reference_v<Class>, placeholder_t>, Class> : true_t {};
/*const volatile qualified void*/
template<typename Class>
struct is_destructible_impl<enable_if_t<is_void_v<Class>, placeholder_t>, Class> : false_t {};
/*function types*/
template<typename Class>
struct is_destructible_impl<enable_if_t<is_function_v<Class>, placeholder_t>, Class> : false_t {};
/*unbounded arrays*/
template<typename Class>
struct is_destructible_impl<enable_if_t<is_unbounded_array_v<Class>, placeholder_t>, Class> : false_t {};
/*bounded arrays, candidate for trying destructing*/
template<typename Class, size_t Num>
struct is_destructible_impl<placeholder_t, Class[Num]> : try_destruct_impl<placeholder_t,
																		   remove_all_extents_t<Class>> {
};
/*object type, candidate for trying destructing*/
template<typename Class>
struct is_destructible_impl<enable_if_t<is_object_v<Class> && !is_array_v<Class>, placeholder_t>, Class>
	: try_destruct_impl<placeholder_t, remove_all_extents_t<Class>> {
};
}  // namespace _impl
template<typename Class>
using is_destructible = _impl::is_destructible_impl<placeholder_t, Class>;
template<typename Class>
constexpr typename is_destructible<Class>::value_t is_destructible_v = is_destructible<Class>::value;

namespace _impl {
template<typename Class, typename =  placeholder_t>
struct is_returnable_impl : false_t {};
template<typename Class>
struct is_returnable_impl<Class, valid_t<Class(&)()>> : true_t {};
template<typename From, typename To, typename = placeholder_t>
struct is_implicitly_convertible_impl : false_t {};
template<typename From, typename To>
struct is_implicitly_convertible_impl<
	From, To, valid_t<decltype(declared_value<void (&)(To)>()(declared_value<From>()))>> : true_t {
};
}  // namespace _impl
template<typename From, typename To>
using is_convertible = boolean_constant<(_impl::is_implicitly_convertible_impl<From, To>::value &&
	_impl::is_returnable_impl<To>::value) || (is_void_v<From> && is_void_v<To>)>;
template<typename From, typename To>
constexpr typename is_convertible<From, To>::value_t is_convertible_v = is_convertible<From, To>::value;

namespace _impl {
template<typename Arg>
void implicit_construct_impl(const Arg &);
template<typename Arg>
true_t is_implicitly_constructible_impl(const Arg &, decltype(implicit_construct_impl<const Arg &>({})) *_ = nullptr);
false_t is_implicitly_constructible_impl(...);
}  // namespace _impl
template<typename Arg>
using is_implicitly_constructible = decltype(_impl::is_implicitly_constructible_impl(declared_value<Arg>()));
eureka_value_helper_macro(is_implicitly_constructible);

namespace _impl {
template<typename Arg>
constexpr enable_if_t<(conjunction_v<is_move_assignable<Arg>, is_move_constructible<Arg >>), void>
swap(Arg &x, Arg &y) noexcept;

template<typename FArg, typename SArg, typename = placeholder_t, typename = placeholder_t>
struct is_swappable_with_impl : false_t {};
template<typename FArg, typename SArg>
struct is_swappable_with_impl<FArg, SArg,
							  valid_t<decltype(swap(declared_value<FArg>(), declared_value<SArg>()))>,
							  valid_t<decltype(swap(declared_value<SArg>(), declared_value<FArg>()))>> : true_t {
};
} // namespace _impl
template<typename FArg, typename SArg>
using is_swappable_with = _impl::is_swappable_with_impl<FArg, SArg, placeholder_t, placeholder_t>;
template<typename FArg, typename SArg>
constexpr typename is_swappable_with<FArg, SArg>::value_t is_swappable_with_v = is_swappable_with<FArg, SArg>::value;

namespace _impl {
template<typename Arg, typename = placeholder_t>
struct is_referencable : true_t {};
template<typename Arg>
struct is_referencable<Arg, enable_if_t<is_void_v<Arg> || is_function_v<remove_const_volatile_reference_t<Arg>>,
										placeholder_t >> : false_t {
};
eureka_value_helper_macro(is_referencable);
template<typename Arg, typename = placeholder_t>
struct is_swappable_impl : false_t {};
template<typename Arg>
struct is_swappable_impl<Arg, enable_if_t<is_referencable_v<Arg>, placeholder_t>> : is_swappable_with<Arg &, Arg &> {};
} // namespace _impl
template<typename Arg>
using is_swappable = _impl::is_swappable_impl<Arg, placeholder_t>;
eureka_value_helper_macro(is_swappable);
} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_ABILITY_H_

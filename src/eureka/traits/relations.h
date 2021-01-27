//
// Created by mamin on 1/26/2021.
//
#ifndef STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_RELATIONS_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_RELATIONS_H_

#include "general.h"
#include "categorization.h"

namespace eureka {
namespace _impl {
template<typename Base>
true_t try_convert_to_base(const volatile Base *);
template<typename>
false_t try_convert_to_base(const volatile void *);

/*for protected and private inheritance*/
template<typename Base, typename Derived>
auto is_base_of_impl(nullptr_t *) -> true_t;
template<typename Base, typename Derived>
auto is_base_of_impl(nullptr_t) -> decltype(try_convert_to_base<Base>(static_cast<Derived *>(nullptr)));
} //  namespace _impl
/**
 * represents an IS-A relationship
 */
template<typename Base, typename Derived>
using is_base_of = conjunction<is_class<Base>, is_class<Derived>,
	decltype(_impl::is_base_of_impl<Base, Derived>(nullptr))>;
template<typename Base, typename Derived>
constexpr typename is_base_of<Base, Derived>::value_t is_base_of_v = is_base_of<Base, Derived>::value;

template<typename...>
struct common_type {};
template<typename... Args>
using common_type_t = typename common_type<Args...>::type;
namespace _impl {
template<typename First, typename Second>
using common_type_enabler = decltype(declared_value<bool>() ? declared_value<First>() : declared_value<Second>());
template<typename First, typename Second, typename  = placeholder_t>
struct common_type_impl {};
template<typename First, typename Second>
struct common_type_impl<First, Second, valid_t < common_type_enabler<First, Second>>> {
using type = decay_t<common_type_enabler<First, Second>>;
};
template<typename Placeholder, typename First, typename Second, typename... Args>
struct common_type_multi_impl {};
template<typename First, typename Second, typename... Args>
struct common_type_multi_impl<valid_t < typename common_type<First, Second>::type>, First, Second, Args...>
: common_type<typename common_type<First, Second>::type, Args...> {
};
} //  namespace _impl
template<typename Arg>
struct common_type<Arg> : common_type<Arg, Arg> {};
template<typename First, typename Second>
struct common_type<First, Second> : _impl::common_type_impl<decay_t < First>, decay_t<Second>> {
};
template<typename First, typename Second, typename... Args>
struct common_type<First, Second, Args...> : _impl::common_type_multi_impl<placeholder_t, First, Second, Args...> {};

}  // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_RELATIONS_H_

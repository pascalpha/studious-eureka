//
// Created by admin on 2021/1/20.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_TYPE_TRAITS_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_TYPE_TRAITS_H_

namespace eureka {

using nullptr_t = decltype(nullptr);
using size_t = decltype(sizeof(nullptr_t));

template<typename Arithmetic, Arithmetic Value>
struct arithmetic_constant {
  using type = arithmetic_constant<Arithmetic, Value>;
  using value_t = Arithmetic;
  static const Arithmetic value = Value;
};

template<bool Value>
using boolean_constant = arithmetic_constant<bool, Value>;
using true_t = boolean_constant<true>;
using false_t = boolean_constant<false>;


/**
 * logic
 */

template<bool If, typename Then, typename Else>
struct conditional { using type = Else; };
template<typename Then, typename Else>
struct conditional<true, Then, Else> { using type = Then; };
template<bool If, typename Then, typename Else>
using conditional_t = typename conditional<If, Then, Else>::type;

template<typename... Disjuncts>
struct disjunction;
template<>
struct disjunction<> : false_t {};
template<typename Disjunct, typename... Disjuncts>
struct disjunction<Disjunct, Disjuncts...> : conditional_t<Disjunct::value, true_t, disjunction<Disjuncts...>> {};
template<typename... Disjuncts>
typename disjunction<Disjuncts...>::value_t disjunction_v = disjunction<Disjuncts...>::value;

template<typename... Conjuncts>
struct conjunction;
template<>
struct conjunction<> : true_t {};
template<typename Conjunct, typename... Conjuncts>
struct conjunction<Conjunct, Conjuncts...> : conditional_t<Conjunct::value, conjunction<Conjuncts...>, false_t> {};
template<typename... Conjuncts>
typename conjunction<Conjuncts...>::value_t conjunction_v = conjunction<Conjuncts...>::value;

template<typename Clause>
using negation = boolean_constant<!Clause::value>;
template<typename Clause>
typename negation<Clause>::value_t negation_v = negation<Clause>::value;

template<typename... Args>
struct is_same : false_t {};
template<typename Arg>
struct is_same<Arg> : true_t {};
template<typename Arg, typename... Args>
struct is_same<Arg, Arg, Args...> : is_same<Arg, Args...> {};
template<typename... Args>
typename is_same<Args...>::value_t is_same_v = is_same<Args...>::value;

/**
 * qualifiers traits
 */
template<typename Arg>
struct is_const : false_t {};
template<typename Arg>
struct is_const<const Arg> : true_t {};
template<typename Arg>
typename is_const<Arg>::value_t is_const_v = is_const<Arg>::value;

template<typename Arg>
struct is_volatile : false_t {};
template<typename Arg>
struct is_volatile<volatile Arg> : true_t {};
template<typename Arg>
typename is_volatile<Arg>::value_t is_volatile_v = is_volatile<Arg>::value;

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

/**
 * categorization traits
 */
template<typename Arg>
struct is_void : is_same<void, remove_const_volatile_t<Arg>> {};
template<typename Arg>
typename is_void<Arg>::value_t is_void_v = is_void<Arg>::value;

namespace _impl {
template<typename Arg>
struct is_pointer_impl : false_t {};
template<typename Arg>
struct is_pointer_impl<Arg *> : true_t {};
} // namespace _impl
template<typename Arg>
using is_pointer = _impl::is_pointer_impl<remove_const_volatile_t<Arg>>;
template<typename Arg>
typename is_pointer<Arg>::value_t is_pointer_v = is_pointer<Arg>::value;

template<typename Arg>
struct is_array : false_t {};
template<typename Arg>
struct is_array<Arg[]> : true_t {};
template<typename Arg, size_t Rank>
struct is_array<Arg[Rank]> : true_t {};
template<typename Arg>
typename is_array<Arg>::value_t is_array_v = is_array<Arg>::value;

template<typename Arg>
struct is_lvalue_reference : false_t {};
template<typename Arg>
struct is_lvalue_reference<Arg &> : true_t {};
template<typename Arg>
typename is_lvalue_reference<Arg>::value_t is_lvalue_reference_v = is_lvalue_reference<Arg>::value;

template<typename Arg>
struct is_rvalue_reference : false_t {};
template<typename Arg>
struct is_rvalue_reference<Arg &&> : true_t {};
template<typename Arg>
typename is_rvalue_reference<Arg>::value_t is_rvalue_reference_v = is_rvalue_reference<Arg>::value;

// __built_in implementation
template<typename Arg>
using is_union = boolean_constant<__is_union(Arg)>;
template<typename Arg>
typename is_union<Arg>::value_t is_union_v = is_union<Arg>::value;

template<typename Arg>
using is_enum = boolean_constant<__is_enum(Arg)>;
template<typename Arg>
typename is_enum<Arg>::value_t is_enum_v = is_enum<Arg>::value;
//

namespace _impl {
template<typename Arg>
true_t is_class_or_union_impl(int Arg::*);
template<typename Arg>
false_t is_class_or_union_impl(...);
template<typename Arg>
struct is_class_or_union : decltype(_impl::is_class_or_union_impl<Arg>(nullptr)) {};
template<typename Arg>
typename is_class_or_union<Arg>::value_t is_class_or_union_v = is_class_or_union<Arg>::value;
} // namespace _impl
template<typename Arg>
using is_class = conjunction<_impl::is_class_or_union<Arg>, negation<is_union<Arg>>>;
template<typename Arg>
typename is_class<Arg>::value_t is_class_v = is_class<Arg>::value;

namespace _impl {
template<typename Arg>
struct is_integral_impl : false_t {};
#ifdef _is_integral_impl_macro
#undef _is_integral_impl_macro
#endif
#define _is_integral_impl_macro(x) template<> struct is_integral_impl<x> : true_t {}
_is_integral_impl_macro(bool);
_is_integral_impl_macro(char);
_is_integral_impl_macro(signed char);
_is_integral_impl_macro(unsigned char);
_is_integral_impl_macro(wchar_t);
_is_integral_impl_macro(char16_t);
_is_integral_impl_macro(char32_t);
_is_integral_impl_macro(signed short);
_is_integral_impl_macro(unsigned short);
_is_integral_impl_macro(signed int);
_is_integral_impl_macro(unsigned int);
_is_integral_impl_macro(signed long);
_is_integral_impl_macro(unsigned long);
_is_integral_impl_macro(signed long long);
_is_integral_impl_macro(unsigned long long);
#undef _is_integral_impl_macro
} // namespace _impl
template<typename Arg>
using is_integral = _impl::is_integral_impl<remove_const_volatile_t<Arg>>;
template<typename Arg>
typename is_integral<Arg>::value_t is_integral_v = is_integral<Arg>::value;

namespace _impl {
template<typename Arg>
struct is_floating_point_impl : false_t {};
#ifdef _is_floating_point_impl_macro
#undef _is_floating_point_impl_macro
#endif
#define _is_floating_point_impl_macro(x) template<> struct is_floating_point_impl<x> : true_t {}
_is_floating_point_impl_macro(float);
_is_floating_point_impl_macro(double);
_is_floating_point_impl_macro(long double);
#undef _is_floating_point_impl_macro
} // namespace _impl
template<typename Arg>
using is_floating_point = _impl::is_floating_point_impl<remove_const_volatile_t<Arg>>;
template<typename Arg>
typename is_floating_point<Arg>::value_t is_floating_point_v = is_floating_point<Arg>::value;
} // namespace

#endif //STUDIOUS_EUREKA_SRC_EUREKA_TYPE_TRAITS_H_

//
// Created by mamin on 1/19/2021.
//
#ifndef STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_H_

namespace eureka {
template<typename Arg>
Arg &&declared_value() {
  return (*static_cast<Arg *>(nullptr));
}

using nullptr_t = decltype(nullptr);
using size_t = decltype(sizeof(nullptr_t));
// todo
using difference_t = long;

template<typename...>
using void_t = void;

template<typename Arithmetic, Arithmetic constant>
struct arithmetic_constant {
  using value_t = Arithmetic;
  static constexpr Arithmetic value = constant;
};

using true_t = arithmetic_constant<bool, true>;
using false_t = arithmetic_constant<bool, false>;

template<bool value>
struct boolean_constant : arithmetic_constant<bool, value> {};

template<bool If, typename Then, typename Else>
struct conditional { using type = Else; };
template<typename Then, typename Else>
struct conditional<true, Then, Else> { using type = Then; };
template<bool If, typename Then, typename Else>
using conditional_t = typename conditional<If, Then, Else>::type;

template<typename...>
struct disjunction_t;
template<>
struct disjunction_t<> : false_t {};
template<typename A, typename... B>
struct disjunction_t<A, B...> : conditional_t<A::value, true_t, disjunction_t<B...>> {};

template<typename...>
struct conjunction_t;
template<>
struct conjunction_t<> : true_t {};
template<typename A, typename... B>
struct conjunction_t<A, B...> : conditional_t<A::value, conjunction_t<B...>, false_t> {};

template<typename BoolArg>
struct negation_t : conditional_t<BoolArg::value, false_t, true_t> {};

template<typename Arg>
struct remove_const { using type = Arg; };
template<typename Arg>
struct remove_const<const Arg> { using type = Arg; };

template<typename Arg>
struct remove_volatile { using type = Arg; };
template<typename Arg>
struct remove_volatile<volatile Arg> { using type = Arg; };

template<typename Arg>
struct remove_const_volatile { using type = typename remove_const<typename remove_volatile<Arg>::type>::type; };

template<typename Arg>
struct add_volatile { using type = Arg; };
template<typename Arg>
struct add_const { using type = const Arg; };
template<typename Arg>
struct add_const_volatile { using type = typename add_const<typename add_volatile<Arg>::type>::type; };

template<typename Arg>
using remove_const_t = typename remove_const<Arg>::type;
template<typename Arg>
using remove_volatile_t = typename remove_volatile<Arg>::type;
template<typename Arg>
using remove_const_volatile_t = typename remove_const_volatile<Arg>::type;
template<typename Arg>
using add_const_t = typename add_const<Arg>::type;
template<typename Arg>
using add_volatile_t = typename add_volatile<Arg>::type;
template<typename Arg>
using add_const_volatile_t = typename add_const_volatile<Arg>::type;

template<typename...>
struct is_same_t : false_t {};
template<typename Arg>
struct is_same_t<Arg, Arg> : true_t {};
template<typename Arg, typename  ... Args>
struct is_same_t<Arg, Arg, Args...> : is_same_t<Arg, Args...> {};

namespace impl {
template<typename>
struct is_void_impl : false_t {};
template<>
struct is_void_impl<void> : true_t {};
} // namespace
template<typename Arg>
using is_void_t = impl::is_void_impl<remove_const_volatile_t<Arg>>;

namespace impl {
template<typename>
struct is_nullptr_impl : false_t {};
template<>
struct is_nullptr_impl<nullptr_t> : true_t {};
} //  namespace
template<typename Arg>
using is_nullptr_t = impl::is_nullptr_impl<remove_const_volatile_t<Arg>>;

namespace impl {
template<typename>
struct is_pointer_impl : false_t {};
template<typename Arg>
struct is_pointer_impl<Arg *> : true_t {};
}
// Note that
// is_pointer_t<nullptr_t>::value == false
// as specified in standards
template<typename Arg>
using is_pointer_t = impl::is_pointer_impl<remove_const_volatile_t<Arg>>;

template<typename>
struct is_lvalue_reference_t : false_t {};
template<typename Arg>
struct is_lvalue_reference_t<Arg &> : true_t {};
template<typename>
struct is_rvalue_reference_t : false_t {};
template<typename Arg>
struct is_rvalue_reference_t<Arg &&> : true_t {};
template<typename Arg>
struct is_reference_t : disjunction_t<is_lvalue_reference_t<Arg>, is_rvalue_reference_t<Arg>> {};

template<typename Arg>
struct is_const_t : false_t {};
template<typename Arg>
struct is_const_t<const Arg> : true_t {};

template<typename Arg>
struct is_volatile_t : false_t {};
template<typename Arg>
struct is_volatile_t<volatile Arg> : true_t {};

/**
 * const does not qualify functions & references, and functions are not references
 */
template<typename Arg>
using is_function_t = boolean_constant<!is_const_t<const Arg>::value && !is_reference_t<Arg>::value>;

template<typename Arg>
using is_object_t = negation_t<disjunction_t<is_function_t<Arg>, is_reference_t<Arg>, is_void_t<Arg>>>;

template<typename Arg>
struct remove_reference { using type = Arg; };
template<typename Arg>
struct remove_reference<Arg &> { using type = Arg; };
template<typename Arg>
struct remove_reference<Arg &&> { using type = Arg; };
} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_H_

//
// Created by mamin on 1/20/2021.
//
#ifndef STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_GENERAL_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_GENERAL_H_

namespace eureka {

using nullptr_t = decltype(nullptr);
using size_t = decltype(sizeof(nullptr_t));

template<typename Arithmetic, Arithmetic Value>
struct arithmetic_constant {
  using type = arithmetic_constant<Arithmetic, Value>;
  using value_t = Arithmetic;
  static constexpr Arithmetic value = Value;
};

// boolean constants differ in naming convention from as used in standard implementations
template<bool Value>
using boolean_constant = arithmetic_constant<bool, Value>;
using true_t = boolean_constant<true>;
using false_t = boolean_constant<false>;


// logic
template<bool If, typename Then, typename Else>
struct conditional { using type = Else; };
template<typename Then, typename Else>
struct conditional<true, Then, Else> { using type = Then; };
template<bool If, typename Then, typename Else>
using conditional_t = typename conditional<If, Then, Else>::type;

// or-ing
template<typename... Disjuncts>
struct disjunction;
template<>
struct disjunction<> : false_t {};
template<typename Disjunct, typename... Disjuncts>
struct disjunction<Disjunct, Disjuncts...> : conditional_t<Disjunct::value, true_t, disjunction<Disjuncts...>> {};
template<typename... Disjuncts>
typename disjunction<Disjuncts...>::value_t disjunction_v = disjunction<Disjuncts...>::value;

// and-ing
template<typename... Conjuncts>
struct conjunction;
template<>
struct conjunction<> : true_t {};
template<typename Conjunct, typename... Conjuncts>
struct conjunction<Conjunct, Conjuncts...> : conditional_t<Conjunct::value, conjunction<Conjuncts...>, false_t> {};
template<typename... Conjuncts>
typename conjunction<Conjuncts...>::value_t conjunction_v = conjunction<Conjuncts...>::value;

// not-ing
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
constexpr typename is_same<Args...>::value_t is_same_v = is_same<Args...>::value;

/**
 * qualifiers traits
 */
template<typename Arg>
struct is_const : false_t {};
template<typename Arg>
struct is_const<const Arg> : true_t {};
template<typename Arg>
constexpr typename is_const<Arg>::value_t is_const_v = is_const<Arg>::value;

template<typename Arg>
struct is_volatile : false_t {};
template<typename Arg>
struct is_volatile<volatile Arg> : true_t {};
template<typename Arg>
constexpr typename is_volatile<Arg>::value_t is_volatile_v = is_volatile<Arg>::value;

} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_GENERAL_H_

//
// Created by admin on 2021/1/20.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_CATEGORIZATION_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_CATEGORIZATION_H_

#include "general.h"
#include "transformation.h"

namespace eureka {
/**
 * categorization traits
 */
template<typename Arg>
struct is_void : is_same<void, remove_const_volatile_t<Arg>> {
};
template<typename Arg>
constexpr typename is_void<Arg>::value_t is_void_v = is_void<Arg>::value;

namespace _impl {
template<typename Arg>
struct is_pointer_impl : false_t {};
template<typename Arg>
struct is_pointer_impl<Arg *> : true_t {};
} // namespace _impl
template<typename Arg>
using is_pointer = _impl::is_pointer_impl<remove_const_volatile_t<Arg>>;
template<typename Arg>
constexpr typename is_pointer<Arg>::value_t is_pointer_v = is_pointer<Arg>::value;

// C++ 14
template<typename Arg>
struct is_null_pointer : is_same<nullptr_t, remove_const_volatile_t<Arg>> {
};
template<typename Arg>
constexpr typename is_null_pointer<Arg>::value_t is_null_pointer_v = is_null_pointer<Arg>::value;

template<typename Arg>
struct is_array : false_t {};
template<typename Arg>
struct is_array<Arg[]> : true_t {};
template<typename Arg, size_t Rank>
struct is_array<Arg[Rank]> : true_t {};
template<typename Arg>
constexpr typename is_array<Arg>::value_t is_array_v = is_array<Arg>::value;

template<typename Arg>
struct is_lvalue_reference : false_t {};
template<typename Arg>
struct is_lvalue_reference<Arg &> : true_t {};
template<typename Arg>
constexpr typename is_lvalue_reference<Arg>::value_t is_lvalue_reference_v = is_lvalue_reference<Arg>::value;

template<typename Arg>
struct is_rvalue_reference : false_t {};
template<typename Arg>
struct is_rvalue_reference<Arg &&> : true_t {};
template<typename Arg>
constexpr typename is_rvalue_reference<Arg>::value_t is_rvalue_reference_v = is_rvalue_reference<Arg>::value;

template<typename Arg>
using is_reference = disjunction<is_rvalue_reference<Arg>, is_lvalue_reference<Arg>>;
template<typename Arg>
constexpr typename is_reference<Arg>::value_t is_reference_v = is_reference<Arg>::value;

// begins __built_in implementation
// These are two basic categorization type traits specified by the standards
// but cannot be readily implemented using the language. Here they resort to compiler magic.
template<typename Arg>
using is_union = boolean_constant<__is_union(Arg)>;
template<typename Arg>
constexpr typename is_union<Arg>::value_t is_union_v = is_union<Arg>::value;

template<typename Arg>
using is_enum = boolean_constant<__is_enum(Arg)>;
template<typename Arg>
constexpr typename is_enum<Arg>::value_t is_enum_v = is_enum<Arg>::value;
// ends __built_in implementation

namespace _impl {
template<typename Arg>
true_t is_class_or_union_impl(int Arg::*);
template<typename Arg>
false_t is_class_or_union_impl(...);
template<typename Arg>
struct is_class_or_union : decltype(_impl::is_class_or_union_impl<Arg>(nullptr)) {};
template<typename Arg>
constexpr typename is_class_or_union<Arg>::value_t is_class_or_union_v = is_class_or_union<Arg>::value;
} // namespace _impl
template<typename Arg>
using is_class = conjunction<_impl::is_class_or_union<Arg>, negation<is_union<Arg>>>;
template<typename Arg>
constexpr typename is_class<Arg>::value_t is_class_v = is_class<Arg>::value;

template<typename Arg>
using is_function = conjunction<negation<is_const<const Arg>>, negation<is_reference<Arg>>>;
template<typename Arg>
constexpr typename is_function<Arg>::value_t is_function_v = is_function<Arg>::value;

namespace _impl {
template<typename Arg>
struct is_member_pointer_impl : false_t {};
template<typename Arg, typename Class>
struct is_member_pointer_impl<Arg Class::*> : true_t {};
template<typename Arg>
struct is_member_function_pointer_impl : false_t {};
template<typename Arg, typename Class>
struct is_member_function_pointer_impl<Arg Class::*> : is_function<Arg> {};
} // namespace _impl
template<typename Arg>
using is_member_pointer = _impl::is_member_pointer_impl<remove_const_volatile_t<Arg>>;
template<typename Arg>
constexpr typename is_member_pointer<Arg>::value_t is_member_pointer_v = is_member_pointer<Arg>::value;

template<typename Arg>
using is_member_function_pointer = _impl::is_member_function_pointer_impl<remove_const_volatile_t<Arg>>;
template<typename Arg>
constexpr typename is_member_function_pointer<Arg>::value_t
    is_member_function_pointer_v = is_member_function_pointer<Arg>::value;

template<typename Arg>
using is_member_object_pointer = conjunction<is_member_pointer<Arg>, negation<is_member_function_pointer<Arg>>>;
template<typename Arg>
constexpr typename is_member_object_pointer<Arg>::value_t
    is_member_object_pointer_v = is_member_object_pointer<Arg>::value;

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
constexpr typename is_integral<Arg>::value_t is_integral_v = is_integral<Arg>::value;

namespace _impl {
template<typename Arg, bool = is_integral_v<Arg>>
struct is_signed_impl : false_t {};
template<typename Arg>
struct is_signed_impl<Arg, true> : boolean_constant<Arg(-1) < Arg(0)> {};
} // namespace _impl
template<typename Arg>
using is_signed = _impl::is_signed_impl<Arg>;
template<typename Arg>
constexpr typename is_signed<Arg>::value_t is_signed_v = is_signed<Arg>::value;

namespace _impl {
template<typename Arg, bool = is_integral_v<Arg>>
struct is_unsigned_impl : false_t {};
template<typename Arg>
struct is_unsigned_impl<Arg, true> : boolean_constant<Arg(0) < Arg(-1)> {};
} // namespace _impl
template<typename Arg>
using is_unsigned = _impl::is_unsigned_impl<Arg>;
template<typename Arg>
constexpr typename is_unsigned<Arg>::value_t is_unsigned_v = is_unsigned<Arg>::value;

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
constexpr typename is_floating_point<Arg>::value_t is_floating_point_v = is_floating_point<Arg>::value;

template<typename Arg>
struct is_arithmetic : disjunction<is_integral<Arg>, is_floating_point<Arg>> {};
template<typename Arg>
constexpr typename is_arithmetic<Arg>::value_t is_arithmetic_v = is_arithmetic<Arg>::value;

template<typename Arg>
struct is_fundamental : disjunction<is_arithmetic<Arg>, is_void<Arg>, is_null_pointer<Arg>> {
};
template<typename Arg>
constexpr typename is_fundamental<Arg>::value_t is_fundamental_v = is_fundamental<Arg>::value;

template<typename Arg>
struct is_compound : negation<is_fundamental<Arg>> {};
template<typename Arg>
constexpr typename is_compound<Arg>::value_t is_compound_v = is_compound<Arg>::value;

template<typename Arg>
struct is_scalar : disjunction<is_arithmetic<Arg>, is_enum<Arg>,
                               is_pointer<Arg>, is_member_pointer<Arg>, is_null_pointer<Arg>> {
};
template<typename Arg>
constexpr typename is_scalar<Arg>::value_t is_scalar_v = is_scalar<Arg>::value;

template<typename Arg>
struct is_object : disjunction<is_scalar<Arg>, is_array<Arg>, is_union<Arg>, is_class<Arg>> {};
template<typename Arg>
constexpr typename is_object<Arg>::value_t is_object_v = is_object<Arg>::value;

template<typename Arg>
struct decay {
 protected:
  using unreferenced = remove_reference_t<Arg>;
 public:
  using type = conditional_t< /*if*/
      is_array_v<unreferenced>, /*is array*/
      remove_extent_t<unreferenced> *, /*then decay first dimension*/
      conditional_t< /*else if*/
          is_function_v<unreferenced>, /*is function*/
          add_pointer_t<unreferenced>, /*decay to pointer*/
          remove_const_volatile_t<unreferenced> /*remove cv qualifiers*/
      >
  >;
};
template<typename Arg>
using decay_t = typename decay<Arg>::type;

template<typename Arg>
add_rvalue_reference_t<Arg> declared_value() noexcept;
} // namespace

#endif //STUDIOUS_EUREKA_SRC_EUREKA_CATEGORIZATION_H_

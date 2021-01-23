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
eureka_value_helper_macro(is_void);

namespace _impl {
template<typename Arg>
struct is_pointer_impl : false_t {};
template<typename Arg>
struct is_pointer_impl<Arg *> : true_t {};
} // namespace _impl
template<typename Arg>
using is_pointer = _impl::is_pointer_impl<remove_const_volatile_t<Arg>>;
eureka_value_helper_macro(is_pointer);

// C++ 14
template<typename Arg>
struct is_null_pointer : is_same<nullptr_t, remove_const_volatile_t<Arg>> {
};
eureka_value_helper_macro(is_null_pointer);

template<typename Arg>
struct is_array : false_t {};
template<typename Arg>
struct is_array<Arg[]> : true_t {};
template<typename Arg, size_t N>
struct is_array<Arg[N]> : true_t {};
eureka_value_helper_macro(is_array);

template<typename Arg>
struct is_bounded_array : false_t {};
template<typename Arg, size_t N>
struct is_bounded_array<Arg[N]> : true_t {};
eureka_value_helper_macro(is_bounded_array);

template<typename Arg>
struct is_unbounded_array : false_t {};
template<typename Arg>
struct is_unbounded_array<Arg[]> : true_t {};
eureka_value_helper_macro(is_unbounded_array);

template<typename Arg>
struct is_lvalue_reference : false_t {};
template<typename Arg>
struct is_lvalue_reference<Arg &> : true_t {};
eureka_value_helper_macro(is_lvalue_reference);

template<typename Arg>
struct is_rvalue_reference : false_t {};
template<typename Arg>
struct is_rvalue_reference<Arg &&> : true_t {};
eureka_value_helper_macro(is_rvalue_reference);

template<typename Arg>
using is_reference = disjunction<is_rvalue_reference<Arg>, is_lvalue_reference<Arg>>;
eureka_value_helper_macro(is_reference);

// begins __built_in implementation
// These are two basic categorization type traits specified by the standards
// but cannot be readily implemented using the language. Here they resort to compiler magic.
template<typename Arg>
using is_union = boolean_constant<__is_union(Arg)>;
eureka_value_helper_macro(is_union);

template<typename Arg>
using is_enum = boolean_constant<__is_enum(Arg)>;
eureka_value_helper_macro(is_enum);

template<typename Arg>
using is_empty = boolean_constant<__is_empty(Arg)>;
eureka_value_helper_macro(is_empty);
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
eureka_value_helper_macro(is_class);

template<typename Arg>
using is_function = conjunction<negation<is_const<const Arg>>, negation<is_reference<Arg>>>;
eureka_value_helper_macro(is_function);

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
eureka_value_helper_macro(is_member_pointer);

template<typename Arg>
using is_member_function_pointer = _impl::is_member_function_pointer_impl<remove_const_volatile_t<Arg>>;
eureka_value_helper_macro(is_member_function_pointer);

template<typename Arg>
using is_member_object_pointer = conjunction<is_member_pointer<Arg>, negation<is_member_function_pointer<Arg>>>;
eureka_value_helper_macro(is_member_object_pointer);

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
eureka_value_helper_macro(is_integral);

namespace _impl {
template<typename Arg, bool = is_integral_v<Arg>>
struct is_signed_impl : false_t {};
template<typename Arg>
/*for signed integer, -1 < 0*/
struct is_signed_impl<Arg, true> : boolean_constant<Arg(-1) < Arg(0)> {};
} // namespace _impl
template<typename Arg>
using is_signed = _impl::is_signed_impl<Arg>;
eureka_value_helper_macro(is_signed);

namespace _impl {
template<typename Arg, bool = is_integral_v<Arg>>
struct is_unsigned_impl : false_t {};
template<typename Arg>
/*for unsigned integer, -1 underflows to a large positive number*/
struct is_unsigned_impl<Arg, true> : boolean_constant<Arg(0) < Arg(-1)> {};
} // namespace _impl
template<typename Arg>
using is_unsigned = _impl::is_unsigned_impl<Arg>;
eureka_value_helper_macro(is_unsigned);

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
eureka_value_helper_macro(is_floating_point);

template<typename Arg>
struct is_arithmetic : disjunction<is_integral<Arg>, is_floating_point<Arg>> {};
eureka_value_helper_macro(is_arithmetic);

template<typename Arg>
struct is_fundamental : disjunction<is_arithmetic<Arg>, is_void<Arg>, is_null_pointer<Arg>> {
};
eureka_value_helper_macro(is_fundamental);

template<typename Arg>
struct is_compound : negation<is_fundamental<Arg>> {};
eureka_value_helper_macro(is_compound);

template<typename Arg>
struct is_scalar : disjunction<is_arithmetic<Arg>, is_enum<Arg>,
                               is_pointer<Arg>, is_member_pointer<Arg>, is_null_pointer<Arg>> {
};
eureka_value_helper_macro(is_scalar);

template<typename Arg>
struct is_object : disjunction<is_scalar<Arg>, is_array<Arg>, is_union<Arg>, is_class<Arg>> {};
eureka_value_helper_macro(is_object);

} // namespace eureka
#endif //STUDIOUS_EUREKA_SRC_EUREKA_CATEGORIZATION_H_

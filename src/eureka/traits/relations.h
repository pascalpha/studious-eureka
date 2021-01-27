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

}  // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_TRAITS_RELATIONS_H_

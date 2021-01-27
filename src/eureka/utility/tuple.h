//
// Created by admin on 2021/1/27.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_

#include "eureka/traits/traits.h"
#include "eureka/utility/utility.h"

namespace eureka {
namespace _impl {
template<typename... Args>
struct tuple_construction_constraints {
  constexpr bool static default_constructible
	  = conjunction_v < is_default_constructible < Args >...>;

  constexpr bool static implicitly_default_constructible
	  = conjunction_v < is_implicitly_constructible < Args >...>;

  template<typename ... Params>
  constexpr bool static copy_constructible
	  = conjunction_v<is_constructible<Args, const Params &>...>;

  template<typename ... Params>
  constexpr bool static implicitly_copy_constructible
	  = conjunction_v<is_convertible<const Params &, Args>...>;

  template<typename ... Params>
  constexpr bool static move_constructible
	  = conjunction_v < is_constructible < Args, Params &&>...>;

  template<typename ... Params>
  constexpr bool static implicitly_move_constructible
	  = conjunction_v < is_convertible < Params &&, Args>...>;
};

template<size_t Index, typename ...>
struct tuple_base;

template<size_t Index, typename Param>
struct tuple_base<Index, Param> {
  using base = void;
  Param value;
};

template<size_t Index, typename Param, typename... Params>
struct tuple_base<Index, Param, Params...> : tuple_base<Index + 1, Params...> {
  using base = tuple_base<Index + 1, Params...>;
  Param value;
};

}  // namespace _impl

template<typename... Args>
struct tuple : _impl::tuple_base<0, Args...> {
 private:
  using constraints = _impl::tuple_construction_constraints<Args...>;
};
} //  namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_

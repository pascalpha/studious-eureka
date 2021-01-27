#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
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

template<size_t Index, typename Element>
struct tuple_base {
  Element value;

  constexpr tuple_base() : value() {}

  constexpr tuple_base(const Element &e) : value(e) {}

  constexpr tuple_base(const tuple_base &) = default;

  constexpr tuple_base(tuple_base &&) noexcept = default;

  template<typename Param>
  constexpr tuple_base(Param &&param) : value(forward<Param>(param)) {}

  static constexpr Element &
  element(tuple_base &base) noexcept { return base.value; }

  static constexpr const Element &
  element(const tuple_base &base) noexcept { return base.value; }
};

template<size_t Index, typename... Elements>
struct tuple_impl;

template<size_t Index, typename Element, typename ... Elements>
struct tuple_impl<Index, Element, Elements...>
	: public tuple_impl<Index + 1, Elements...>,
	  private tuple_base<Index, Element> {

  /*befriending myself*/
  template<size_t, typename ...>
  friend
  class tuple_impl;

  using base_t = tuple_base<Index, Element>;
  using inherited_t = tuple_impl<Index + 1, Elements...>;

  static constexpr Element &
  head(tuple_impl &t) noexcept { return base_t::element(t); }

  static constexpr const Element &
  head(const tuple_impl &t) noexcept { return base_t::element(t); }

  static constexpr inherited_t &
  tail(tuple_impl &t) noexcept { return t; }

  static constexpr const inherited_t &
  tail(const tuple_impl &t) noexcept { return t; }

  constexpr tuple_impl() : inherited_t(), base_t() {}

  explicit constexpr tuple_impl(const base_t &base, const inherited_t &inherited)
	  : inherited_t(inherited), base_t(base) {}

  template<typename Head, typename... Tail, typename = enable_if_t<
	  sizeof...(Tail) == sizeof...(Elements), placeholder_t>>
  explicit constexpr tuple_impl(Head &&head, Tail &&... tail)
	  : inherited_t(forward<Tail>(tail)...), base_t(forward<Head>(head)) {}

  constexpr tuple_impl(const tuple_impl &) = default;

  tuple_impl &operator=(const tuple_impl &) = delete;

  constexpr tuple_impl(tuple_impl &&t) noexcept
	  : inherited_t(move(tail(t))), base_t(forward<Element>(head(t))) {}

  template<typename... Params>
  constexpr tuple_impl(const tuple_impl<Index, Params...> &t) : inherited_t(tuple_impl<Index, Params...>::tail(t)),
																base_t(tuple_impl<Index, Params...>::head(t)) {}

  template<typename Head, typename... Tail>
  constexpr tuple_impl(tuple_impl<Index, Head, Tail...> &&t)
	  : inherited(move(tuple_impl<Index, Head, Tail...>::tail(t))),
		base_t(forward<Head>(tuple_impl<Index, Head, Tail...>::head(t))) {}
};
template<size_t Index, typename Head>
struct tuple_impl<Index, Head> : private tuple_base<Index, Head> {

};
}  // namespace _impl
template<typename... Elements>
class tuple;
} //  namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_

#pragma clang diagnostic pop
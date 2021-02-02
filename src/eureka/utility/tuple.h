#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-forwarding-reference-overload"
#pragma ide diagnostic ignored "google-explicit-constructor"
//
// Created by admin on 2021/2/1.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_

#include "eureka/traits/traits.h"
#include "eureka/utility/utility.h"

namespace eureka {

template<typename... Elements>
struct tuple;

namespace _impl {

template<typename Arg>
struct is_tuple : false_t {};
template<typename... Args>
struct is_tuple<tuple<Args...>> : true_t {};
eureka_value_helper_macro(is_tuple);
template<typename Arg>
constexpr bool is_ebo_optimized = is_empty_v < Arg > && !is_final_v < Arg > && !is_tuple_v<Arg>;

template<size_t Index, typename Element, bool = is_ebo_optimized<Element>>
struct tuple_head;
template<size_t Index, typename Element>
struct tuple_head<Index, Element, /*is_ebo_optimized = */true> : Element {
  constexpr tuple_head() = default;
  constexpr tuple_head(const Element &e) : Element(e) {}
  constexpr tuple_head(const tuple_head &) = default;
  constexpr tuple_head(tuple_head &&) noexcept = default;
  template<typename Param>
  constexpr tuple_head(Param &&param) : Element(forward<Param>(param)) {}

  static constexpr Element &get_element(tuple_head &h) { return h; }
  static constexpr const Element &get_element(const tuple_head &h) { return h; }
};

template<size_t Index, typename Element>
struct tuple_head<Index, Element, /*is_ebo_optimized = */false> {
 private:
  Element value;
 public:
  constexpr tuple_head() : value() {}
  constexpr tuple_head(const Element &e) : value(e) {}
  constexpr tuple_head(const tuple_head &) = default;
  constexpr tuple_head(tuple_head &&) noexcept = default;
  template<typename Param>
  constexpr tuple_head(Param &&param) : value(forward<Param>(param)) {}
  static constexpr Element &get_element(tuple_head &h) noexcept { return h.value; }
  static constexpr const Element &get_element(const tuple_head &h) noexcept { return h.value; }
};

template<size_t Index, typename... Elements>
struct tuple_cons;
template<size_t Index, typename Element, typename... Elements>
struct tuple_cons<Index, Element, Elements...>
	: protected tuple_head<Index, Element>,
	  public tuple_cons<Index + 1, Elements...> {
  using head_t = tuple_head<Index, Element>;
  using tail_t = tuple_cons<Index + 1, Elements...>;

  /*
   * head / tail accessors
   */
  static constexpr Element &head(tuple_cons &c) { return head_t::get_element(c); }
  static constexpr const Element &head(const tuple_cons &c) { return head_t::get_element(c); }
  static constexpr tail_t &tail(tuple_cons &c) { return c; }
  static constexpr const tail_t &tail(const tuple_cons &c) { return c; }

  constexpr tuple_cons() : head_t(), tail_t() {}

  explicit constexpr tuple_cons(const Element &element, const Elements &... elements)
	  : head_t(element), tail_t(elements...) {}

  template<typename Param, typename... Params,
	  typename = enable_if_t<sizeof...(Params) == sizeof...(Elements), placeholder_t>>
  explicit constexpr tuple_cons(Param &&param, Params &&... params)
	  : head_t(forward<Param>(param)), tail_t(forward<Params>(params)...) {}

  constexpr tuple_cons(const tuple_cons &) = default;

  constexpr tuple_cons(tuple_cons &&c) noexcept: head_t(forward<Element>(head(c))), tail_t(move(tail(c))) {}

  template<typename... Params>
  constexpr tuple_cons(const tuple_cons<Index, Params...> &c)
	  : head_t(tuple_cons<Index, Params...>::head(c)), tail_t(tuple_cons<Index, Params...>::tail(c)) {}

  template<typename Param, typename... Params>
  constexpr tuple_cons(tuple_cons<Index, Param, Params...> &&c)
	  : head_t(forward<Param>(tuple_cons<Index, Param, Params...>::head(c))),
		tail_t(move(tuple_cons<Index, Param, Params...>::tail(c))) {}
};
template<size_t Index, typename Element>
struct tuple_cons<Index, Element> : protected tuple_head<Index, Element> {
  using head_t = tuple_head<Index, Element>;
  /*
   * head / tail accessors
   */
  static constexpr Element &head(tuple_cons &c) { return head_t::get_element(c); }
  static constexpr const Element &head(const tuple_cons &c) { return head_t::get_element(c); }

  constexpr tuple_cons() : head_t() {}

  explicit constexpr tuple_cons(const Element &element)
	  : head_t(element) {}

  template<typename Param>
  explicit constexpr tuple_cons(Param &&param)
	  : head_t(forward<Param>(param)) {}

  constexpr tuple_cons(const tuple_cons &) = default;

  constexpr tuple_cons(tuple_cons &&c) noexcept: head_t(forward<Element>(head(c))) {}

  template<typename Param>
  constexpr tuple_cons(const tuple_cons<Index, Param> &c)
	  : head_t(tuple_cons<Index, Param>::head(c)) {}

  template<typename Param>
  constexpr tuple_cons(tuple_cons<Index, Param> &&c)
	  : head_t(forward<Param>(tuple_cons<Index, Param>::head(c))) {}
};

template<bool, typename... Elements>
struct tuple_construction_constraints {

  static constexpr bool explicit_default_enabler
	  = (conjunction_v<is_default_constructible < Elements>...> &&
  ! conjunction_v<is_implicitly_constructible < Elements>...>);

  static constexpr bool implicit_default_enabler
	  = (conjunction_v<is_default_constructible < Elements>...> &&
  conjunction_v<is_implicitly_constructible < Elements>...>);

  template<typename... Params>
  static constexpr bool explicit_copy_enabler
	  = (conjunction_v<is_constructible<Elements, const Params &>...> &&
  ! conjunction_v<is_convertible<const Params &, Elements>...>);

  template<typename... Params>
  static constexpr bool implicit_copy_enabler
	  = (conjunction_v<is_constructible<Elements, const Params &>...> &&
	  conjunction_v<is_convertible<const Params &, Elements>...>);

  template<typename... Params>
  static constexpr bool explicit_convert_enabler
	  = (conjunction_v < is_constructible < Elements, Params && >...> &&
  ! conjunction_v<is_convertible < Params && , Elements>...>);

  template<typename... Params>
  static constexpr bool implicit_convert_enabler
	  = (conjunction_v < is_constructible < Elements, Params && >...> &&
  conjunction_v<is_convertible < Params && , Elements>...>);

  template<typename... Params>
  static constexpr bool explicit_move_enabler
	  = (conjunction_v < is_constructible < Elements, Params && >...> &&
  ! conjunction_v<is_convertible < Params && , Elements>...>);

  template<typename... Params>
  static constexpr bool implicit_move_enabler
	  = (conjunction_v < is_constructible < Elements, Params && >...> &&
  conjunction_v<is_convertible < Params && , Elements>...>);

};

template<typename... Elements>
struct tuple_construction_constraints<false, Elements...> {
  static constexpr bool explicit_default_enabler = false;
  static constexpr bool implicit_default_enabler = false;
  template<typename... Params>
  static constexpr bool explicit_copy_enabler = false;
  template<typename... Params>
  static constexpr bool implicit_copy_enabler = false;
  template<typename... Params>
  static constexpr bool explicit_convert_enabler = false;
  template<typename... Params>
  static constexpr bool implicit_convert_enabler = false;
  template<typename... Params>
  static constexpr bool explicit_move_enabler = false;
  template<typename... Params>
  static constexpr bool implicit_move_enabler = false;
};
} // namespace _impl

template<typename... Elements>
struct tuple : _impl::tuple_cons<0, Elements...> {

  template<bool Enabled>
  using constraints = _impl::tuple_construction_constraints<Enabled, Elements...>;

  using tail_t = _impl::tuple_cons<0, Elements...>;

  template<bool enabled = true, enable_if_t<constraints<enabled>::explicit_default_enabler, bool> = false>
  explicit constexpr tuple() : tail_t() {}

  template<bool enabled = true, enable_if_t<constraints<enabled>::implicit_default_enabler, bool> = true>
  constexpr tuple() : tail_t() {}

  template<bool enabled = true, enable_if_t<
	  constraints<enabled && sizeof...(Elements) >= 1>::template explicit_copy_enabler<Elements...>, bool> = false>
  explicit constexpr tuple(const Elements &... elements) : tail_t(elements...) {}

  template<bool enabled = true, enable_if_t<
	  constraints<enabled && sizeof...(Elements) >= 1>::template implicit_copy_enabler<Elements...>, bool> = true>
  constexpr tuple(const Elements &... elements) : tail_t(elements...) {}

  template<typename... Params, enable_if_t<
	  constraints<sizeof...(Params) == sizeof...(Elements)
					  && (sizeof...(Params) > 1 ||(sizeof...(Params) == 1 && !is_same_v < tuple<Elements...>,
												   remove_const_volatile_reference_t < Params > ...>))>
  ::template explicit_move_enabler<Params...>, bool> = false>
  explicit constexpr tuple(Params &&... params) : tail_t(forward<Params>(params)...) {}

  template<typename... Params, enable_if_t<
	  constraints<sizeof...(Params) == sizeof...(Elements)
					  && (sizeof...(Params) > 1 ||(sizeof...(Params) == 1 && !is_same_v < tuple<Elements...>,
												   remove_const_volatile_reference_t < Params > ...>))>
  ::template implicit_move_enabler<Params...>, bool> = true>
  constexpr tuple(Params &&... params) : tail_t(forward<Params>(params)...) {}

  constexpr tuple(const tuple &) = default;
  constexpr tuple(tuple &&) noexcept = default;

  template<bool enabled = true, typename... Params, enable_if_t<
	  constraints<enabled>::template explicit_copy_enabler<Params...>, bool> = false>
  explicit constexpr tuple(const tuple<Params...> &t)
	  : tail_t(static_cast<const _impl::tuple_cons<0, Params...> &>(t)) {}

  template<bool enabled = true, typename... Params, enable_if_t<
	  constraints<enabled>::template implicit_copy_enabler<Params...>, bool> = true>
  constexpr tuple(const tuple<Params...> &t)
	  : tail_t(static_cast<const _impl::tuple_cons<0, Params...> &>(t)) {}

  template<bool enabled = true, typename... Params, enable_if_t<
	  constraints<enabled>::template explicit_move_enabler<Params...>, bool> = false>
  explicit constexpr tuple(tuple<Params...> &&t)
	  : tail_t(static_cast<_impl::tuple_cons<0, Params...> &&>(t)) {}

  template<bool enabled = true, typename... Params, enable_if_t<
	  constraints<enabled>::template implicit_move_enabler<Params...>, bool> = true>
  constexpr tuple(tuple<Params...> &&t)
	  : tail_t(static_cast<_impl::tuple_cons<0, Params...> &&>(t)) {}

};
} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_

#pragma clang diagnostic pop
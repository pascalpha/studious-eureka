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
#include "eureka/functional/reference_wrapper.h"

/*
 * TODO Assignment Operators & Relational Operators & Swap functionality
 */
namespace eureka {

template<typename... Elements>
struct tuple;

namespace _impl {
/*type traits for determining tuple types*/
template<typename Arg>
struct is_tuple : false_t {};
template<typename... Args>
struct is_tuple<tuple<Args...>> : true_t {};
eureka_value_helper_macro(is_tuple);
/*enabler for empty base class object optimization*/
template<typename Arg>
constexpr bool is_ebo_optimized = is_empty_v<Arg> && !is_final_v<Arg> && !is_tuple_v<Arg>;

/**
 * \brief tuple element holder
 * \tparam Index index of tuple value
 * \tparam Element type of tuple value
 *
 * This is EBO optimized.
 */
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
/**
 * \brief tuple element holder
 * \tparam Index index of tuple value
 * \tparam Element type of tuple value
 *
 * This is unoptimized.
 */
template<size_t Index, typename Element>
struct tuple_head<Index, Element, /*is_ebo_optimized = */false> {
  static constexpr Element &get_element(tuple_head &h) noexcept { return h.value; }
  Element value;
  constexpr tuple_head() : value() {}
  constexpr tuple_head(const Element &e) : value(e) {}
  constexpr tuple_head(const tuple_head &) = default;
  constexpr tuple_head(tuple_head &&) noexcept = default;
  template<typename Param>
  constexpr tuple_head(Param &&param) : value(forward<Param>(param)) {}
  static constexpr const Element &get_element(const tuple_head &h) noexcept { return h.value; }
};

/**
 * \brief tuple represented as a compile time linked list implemented with inheritance
 * \tparam Index Index of first value
 * \tparam Elements Types of remaining values
 */
template<size_t Index, typename... Elements>
struct tuple_cons;
/*tuple linked list, recursive case*/
template<size_t Index, typename Element, typename... Elements>
struct tuple_cons<Index, Element, Elements...>
	: public tuple_head<Index, Element>,
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
/*tuple linked list, end case*/
template<size_t Index, typename Element>
struct tuple_cons<Index, Element> : public tuple_head<Index, Element> {
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

/**
 * \brief construction constraints
 * \tparam Elements tuple element types
 */
template<bool, typename... Elements>
struct tuple_construction_constraints {

  static constexpr bool explicit_default_enabler
	  = (conjunction_v<is_default_constructible<Elements>...> &&
		  !conjunction_v<is_implicitly_constructible<Elements>...>);

  static constexpr bool implicit_default_enabler
	  = (conjunction_v<is_default_constructible<Elements>...> &&
		  conjunction_v<is_implicitly_constructible<Elements>...>);

  template<typename... Params>
  static constexpr bool explicit_copy_enabler
	  = (conjunction_v<is_constructible<Elements, const Params &>...> &&
		  !conjunction_v<is_convertible<const Params &, Elements>...>);

  template<typename... Params>
  static constexpr bool implicit_copy_enabler
	  = (conjunction_v<is_constructible<Elements, const Params &>...> &&
		  conjunction_v<is_convertible<const Params &, Elements>...>);

  /*for compatibility and future adaptations*/
  template<typename... Params>
  [[maybe_unused]] static constexpr bool explicit_convert_enabler
	  = (conjunction_v<is_constructible<Elements, Params &&>...> &&
		  !conjunction_v<is_convertible<Params &&, Elements>...>);

  /*for compatibility and future adaptations*/
  template<typename... Params>
  [[maybe_unused]] static constexpr bool implicit_convert_enabler
	  = (conjunction_v<is_constructible<Elements, Params &&>...> &&
		  conjunction_v<is_convertible<Params &&, Elements>...>);

  template<typename... Params>
  static constexpr bool explicit_move_enabler
	  = (conjunction_v<is_constructible<Elements, Params &&>...> &&
		  !conjunction_v<is_convertible<Params &&, Elements>...>);

  template<typename... Params>
  static constexpr bool implicit_move_enabler
	  = (conjunction_v<is_constructible<Elements, Params &&>...> &&
		  conjunction_v<is_convertible<Params &&, Elements>...>);
};

/**
 * \brief used to explicitly disable construction
 * \tparam Elements
 */
template<typename... Elements>
struct tuple_construction_constraints<false, Elements...> {
  static constexpr bool explicit_default_enabler = false;
  static constexpr bool implicit_default_enabler = false;
  template<typename... Params>
  static constexpr bool explicit_copy_enabler = false;
  template<typename... Params>
  static constexpr bool implicit_copy_enabler = false;
  template<typename... Params>
  [[maybe_unused]] static constexpr bool explicit_convert_enabler = false;
  template<typename... Params>
  [[maybe_unused]] static constexpr bool implicit_convert_enabler = false;
  template<typename... Params>
  static constexpr bool explicit_move_enabler = false;
  template<typename... Params>
  static constexpr bool implicit_move_enabler = false;
};
} // namespace _impl

/**
 * \brief tuple for holding a heterogeneous vector of values of compile-time known types.
 * \tparam Elements types of values
 */
template<typename... Elements>
struct tuple : protected _impl::tuple_cons<0, Elements...> {
  template<size_t Index, typename... Args>
  friend auto get(tuple<Args...> &t);

 protected:

  template<bool Enabled>
  using constraints = _impl::tuple_construction_constraints<Enabled, Elements...>;

  /**
   * \brief base class as next linked list node, first node being sentinel
   */
  using tail_t = _impl::tuple_cons<0, Elements...>;

 public:
  /**
   * \brief explicit default constructor
   * \tparam enabled undefined behavior if specified
   */
  template<bool enabled = true, enable_if_t<constraints<enabled>::explicit_default_enabler, bool> = false>
  explicit constexpr tuple() : tail_t() {}

  /**
   * \brief implicit default constructor
   * \tparam enabled undefined behavior if specified
   */
  template<bool enabled = true, enable_if_t<constraints<enabled>::implicit_default_enabler, bool> = true>
  constexpr tuple() : tail_t() {}

  /**
   * \brief explicit direct copy constructor
   * \tparam enabled undefined behavior if specified
   * \param elements elements to copy
   */
  template<bool enabled = true, enable_if_t<
	  constraints<enabled && sizeof...(Elements) >= 1>::template explicit_copy_enabler<Elements...>, bool> = false>
  explicit constexpr tuple(const Elements &... elements) : tail_t(elements...) {}

  /**
   * \brief implicit direct copy constructor
   * \tparam enabled undefined behavior if specified
   * \param elements elements to copy
   */
  template<bool enabled = true, enable_if_t<
	  constraints<enabled && sizeof...(Elements) >= 1>::template implicit_copy_enabler<Elements...>, bool> = true>
  constexpr tuple(const Elements &... elements) : tail_t(elements...) {}

  /**
   * \brief explicit direct forwarding constructor
   * \tparam Params deduced argument types
   * \param params elements to forward
   */
  template<typename... Params, enable_if_t<
	  constraints<sizeof...(Params) == sizeof...(Elements)
					  && (sizeof...(Params) > 1 ||
						  (sizeof...(Params) == 1 &&
							  !is_same_v<tuple<Elements...>, remove_const_volatile_reference_t<Params> ...>))>
	  ::template explicit_move_enabler<Params...>, bool> = false>
  explicit constexpr tuple(Params &&... params) : tail_t(forward<Params>(params)...) {}

  /**
   * \brief implicit direct forwarding constructor
   * \tparam Params deduced argument types
   * \param params elements to forward
   */
  template<typename... Params, enable_if_t<
	  constraints<sizeof...(Params) == sizeof...(Elements)
					  && (sizeof...(Params) > 1 ||
						  (sizeof...(Params) == 1 &&
							  !is_same_v<tuple<Elements...>, remove_const_volatile_reference_t<Params> ...>))>
	  ::template implicit_move_enabler<Params...>, bool> = true>
  constexpr tuple(Params &&... params) : tail_t(forward<Params>(params)...) {}

  /**
   * \brief default copy constructor
   */
  constexpr tuple(const tuple &) = default;

  /**
   * \brief default move constructor
   */
  constexpr tuple(tuple &&) noexcept = default;

  /**
   * \brief explicit copy constructor from another tuple
   * \tparam enabled undefined behavior of specified
   * \tparam Params
   * \param t
   */
  template<bool enabled = true, typename... Params, enable_if_t<
	  constraints<enabled>::template explicit_copy_enabler<Params...>, bool> = false>
  explicit constexpr tuple(const tuple<Params...> &t)
	  : tail_t(static_cast<const _impl::tuple_cons<0, Params...> &>(t)) {}
  /**
   * \brief implicit copy constructor from another tuple
   * \tparam enabled undefined behavior of specified
   * \tparam Params
   * \param t
   */
  template<bool enabled = true, typename... Params, enable_if_t<
	  constraints<enabled>::template implicit_copy_enabler<Params...>, bool> = true>
  constexpr tuple(const tuple<Params...> &t)
	  : tail_t(static_cast<const _impl::tuple_cons<0, Params...> &>(t)) {}
  /**
   * \brief explicit forwarding constructor from another tuple
   * \tparam enabled undefined behavior of specified
   * \tparam Params
   * \param t
   */
  template<bool enabled = true, typename... Params, enable_if_t<
	  constraints<enabled>::template explicit_move_enabler<Params...>, bool> = false>
  explicit constexpr tuple(tuple<Params...> &&t)
	  : tail_t(static_cast<_impl::tuple_cons<0, Params...> &&>(t)) {}
  /**
   * \brief implicit forwarding constructor from another tuple
   * \tparam enabled undefined behavior of specified
   * \tparam Params
   * \param t
   */
  template<bool enabled = true, typename... Params, enable_if_t<
	  constraints<enabled>::template implicit_move_enabler<Params...>, bool> = true>
  constexpr tuple(tuple<Params...> &&t)
	  : tail_t(static_cast<_impl::tuple_cons<0, Params...> &&>(t)) {}
};

namespace _impl {
template<typename Arg>
struct unwrap_reference_wrapper { using type = Arg; };
template<typename Arg>
struct unwrap_reference_wrapper<reference_wrapper<Arg>> { using type = Arg &; };
template<typename Arg>
using tuple_decay_t = typename unwrap_reference_wrapper<decay_t<Arg>>::type;
} // namespace _impl
/**
 * \brief make a tuple with provided values
 * \tparam Args
 * \param args
 * \return
 */
template<typename... Args>
auto make_tuple(Args &&... args) -> decltype(auto) {
  return tuple<_impl::tuple_decay_t<Args>...>(forward<Args>(args)...);
}

namespace _impl {
template<size_t Index, typename Arg>
struct tuple_element;
template<size_t Index, typename Arg, typename... Args>
struct tuple_element<Index, tuple<Arg, Args...>> : tuple_element<Index - 1, tuple<Args...>> {};
template<typename Arg, typename...Args>
struct tuple_element<0, tuple<Arg, Args...>> { using type = Arg; };
} // namespace _impl
template<size_t Index, typename... Args>

/**
 * \brief get the i-th element of the argument tuple as an lvalue reference
 * \tparam Index
 * \tparam Args
 * \param t
 * \return
 */
auto get(tuple<Args...> &t) {
  return _impl::tuple_head<Index, typename _impl::tuple_element<Index, tuple<Args...>>::type>::get_element(t);
}

/**
 * \brief get the i-th element of the argument tuple as a const lvalue reference
 * \tparam Index
 * \tparam Args
 * \param t
 * \return
 */
template<size_t Index, typename... Args>
auto get(const tuple<Args...> &t) {
  return _impl::tuple_head<Index, typename _impl::tuple_element<Index, tuple<Args...>>::type>::get_element(t);
}

/**
 * \brief get the i-th element of the argument tuple as an rvalue reference
 * \tparam Index
 * \tparam Args
 * \param t
 * \return
 */
template<size_t Index, typename... Args>
auto get(tuple<Args...> &&t) {
  return _impl::tuple_head<Index, typename _impl::tuple_element<Index, tuple<Args...>>::type>::get_element(t);
}

/**
 * \brief get the i-th element of the argument tuple as a const rvalue reference
 * \tparam Index
 * \tparam Args
 * \param t
 * \return
 */
template<size_t Index, typename... Args>
auto get(const tuple<Args...> &&t) {
  return _impl::tuple_head<Index, typename _impl::tuple_element<Index, tuple<Args...>>::type>::get_element(t);
}
} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_

#pragma clang diagnostic pop
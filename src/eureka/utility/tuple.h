#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
#pragma ide diagnostic ignored "bugprone-forwarding-reference-overload"
#pragma ide diagnostic ignored "google-explicit-constructor"
//
// Created by mamin on 1/28/2021.
//
#ifndef STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_

#include "eureka/utility/utility.h"
#include "eureka/traits/traits.h"

namespace eureka {

template<typename... Args>
struct tuple;

namespace _impl {
template<size_t Index, typename Arg>
struct tuple_head {
  Arg value;

  constexpr tuple_head() : value() {}

  constexpr tuple_head(const Arg &a) : value(a) {}

  constexpr tuple_head(const tuple_head &) = default;

  constexpr tuple_head(tuple_head &&) noexcept = default;

  template<typename Param>
  constexpr tuple_head(Param &&param) : value(forward<Param>(param)) {}

  constexpr Arg &head_v() noexcept { return value; }

  constexpr const Arg &head_v() const noexcept { return value; }
}; // class tuple_head

template<size_t Index, typename... Args>
struct tuple_cons;

template<size_t Index, typename Arg, typename... Args>
struct tuple_cons<Index, Arg, Args...>
	: public tuple_cons<Index + 1, Args...>,
	  private tuple_head<Index, Arg> {

  template<size_t, typename...> friend
  class tuple_cons;

  using head_t = tuple_head<Index, Arg>;
  using tail_t = tuple_cons<Index + 1, Args...>;
  using cons_t = tuple_cons<Index, Arg, Args...>;

  constexpr Arg &head() noexcept { return head_t::head_v(); }

  constexpr const Arg &head() const noexcept { return head_t::head_v(); }

  constexpr tail_t &tail() noexcept { return *this; }

  constexpr const tail_t &tail() const noexcept { return *this; }

  constexpr tuple_cons() : tail_t(), head_t() {}

  explicit constexpr tuple_cons(const Arg &arg, const Args &... args) : tail_t(args...), head_t(arg) {}

  template<typename Param, typename... Params,
	  typename = enable_if_t<sizeof...(Params) == sizeof...(Args), placeholder_t>>
  explicit constexpr tuple_cons(Param &&param, Params &&...params)
	  : tail_t(forward<Params>(params)...), head_t(forward<Param>(param)) {}

  constexpr tuple_cons(const tuple_cons &) = default;

  constexpr tuple_cons(tuple_cons &&cons) noexcept
	  : tail_t(move(cons.cons_t::tail())), head_t(forward<Arg>(cons.cons_t::head())) {}

  template<typename... Params>
  constexpr tuple_cons(const tuple_cons<Index, Params...> &cons)
	  : tail_t(cons.tuple_cons<Index, Params...>::tail()), head_t(cons.tuple_cons<Index, Params...>::head()) {}

  template<typename Param, typename... Params>
  constexpr tuple_cons(tuple_cons<Index, Arg, Params...> &&cons)
	  : tail_t(move(cons.tuple_cons<Index, Param, Params...>::tail())),
		head_t(forward<Param>(cons.tuple_cons<Index, Param, Params...>::head())) {}
};

template<size_t Index, typename Arg>
struct tuple_cons<Index, Arg> : private tuple_head<Index, Arg> {
  template<size_t, typename...> friend
  class tuple_cons;

  using head_t = tuple_head<Index, Arg>;
  using cons_t = tuple_cons<Index, Arg>;

  constexpr Arg &head() noexcept { return head_t::head_v(); }

  constexpr const Arg &head() const noexcept { return head_t::head_v(); }

  constexpr tuple_cons() : head_t() {}

  explicit constexpr tuple_cons(const Arg &arg) : head_t(arg) {}

  template<typename Param>
  explicit constexpr tuple_cons(Param &&param) : head_t(forward<Param>(param)) {}

  constexpr tuple_cons(const tuple_cons &) = default;

  constexpr tuple_cons(tuple_cons &&cons) noexcept: head_t(forward<Arg>(cons.cons_t::head())) {}

  template<typename Param>
  constexpr tuple_cons(const tuple_cons<Index, Param> &cons) : head_t(cons.tuple_cons<Index, Param>::head()) {}

  template<typename Param>
  constexpr tuple_cons(tuple_cons<Index, Arg> &&cons)
	  : head_t(forward<Param>(cons.tuple_cons<Index, Param>::head())) {}
};

template<bool, typename Placeholder, typename... Args>
struct tuple_construction_constraints {
  constexpr static bool default_constructible
	  = conjunction_v < is_default_constructible < Args >...>;

  constexpr static bool implicitly_default_constructible
	  = conjunction_v < is_implicitly_constructible < Args >...>;

  constexpr static bool direct_constructible
	  = sizeof...(Args) >= 1 && conjunction_v < is_copy_constructible < Args >...>;

  constexpr static bool implicitly_direct_constructible
	  = sizeof...(Args) >= 1 && conjunction_v<is_convertible<const Args &, Args>...>;

  template<typename... Params>
  constexpr static bool converting_constructible
	  = sizeof...(Params) == sizeof...(Args)
		  && (sizeof...(Params) > 1)
		  && conjunction_v < is_constructible < Args, Params&&>...>;

  template<typename... Params>
  constexpr static bool implicitly_converting_constructible
	  = sizeof...(Params) == sizeof...(Args) && sizeof...(Params) >= 1
		  && conjunction_v < is_convertible < Params &&, Args>...>;

  template<typename... Params>
  constexpr static bool converting_copy_constructible
	  = sizeof...(Params) == sizeof...(Args)
		  && conjunction_v < is_constructible<Args, const Params &>...> &&( sizeof...(Params) != 1 || ! disjunction_v<
	  is_convertible<const ::eureka::tuple<Params> &, Args>...,
	  is_constructible<Args, const ::eureka::tuple<Params> &>...,
	  is_same < Params, Args>...>);

  template<typename... Params>
  constexpr static bool implicitly_converting_copy_constructible
	  = sizeof...(Params) == sizeof...(Args)
		  && conjunction_v<is_convertible<const Params &, Args>...>;

  template<typename... Params>
  constexpr static bool converting_move_constructible
	  = sizeof...(Params) == sizeof...(Args)
		  && conjunction_v < is_constructible < Args, Params &&>...> &&( sizeof...(Params) != 1 ||
  ! disjunction_v<
	  is_convertible < ::eureka::tuple<Params>, Args>...,
  is_constructible<Args, ::eureka::tuple<Params>>...,
  is_same<Params, Args>...>);

  template<typename... Params>
  constexpr static bool implicitly_converting_move_constructible
	  = sizeof...(Params) == sizeof...(Args)
		  && conjunction_v < is_convertible < Params &&, Args>...>;
};

template<typename Placeholder, typename... Args>
struct tuple_construction_constraints<false, Placeholder, Args...> {
  constexpr static bool default_constructible = false;
  constexpr static bool implicitly_default_constructible = false;
  constexpr static bool direct_constructible = false;
  constexpr static bool implicitly_direct_constructible = false;
  template<typename... Params>
  constexpr static bool converting_constructible = false;
  template<typename... Params>
  constexpr static bool implicitly_converting_constructible = false;
  template<typename... Params>
  constexpr static bool converting_copy_constructible = false;
  template<typename... Params>
  constexpr static bool implicitly_converting_copy_constructible = false;
  template<typename... Params>
  constexpr static bool converting_move_constructible = false;
  template<typename... Params>
  constexpr static bool implicitly_converting_move_constructible = false;
};
} // namespace _impl

template<typename... Args>
struct tuple : _impl::tuple_cons<0, Args...> {
  using tail_t = _impl::tuple_cons<0, Args...>;

  template<typename Placeholder, typename... Params>
  using constraints = _impl::tuple_construction_constraints<
	  true,
	  Placeholder, Params...>;

  template<typename Placeholder = placeholder_t,
	  enable_if_t<constraints<Placeholder, Args...>::default_constructible &&
		  constraints<Placeholder, Args...>::implicit_default_constructible, bool> = false>
  constexpr tuple() : tail_t() {}

  template<typename Placeholder = placeholder_t,
	  enable_if_t<constraints<Placeholder, Args...>::default_constructible &&
		  !constraints<Placeholder, Args...>::implicit_default_constructible, bool> = true>
  explicit constexpr tuple() : tail_t() {}

  template<typename Placeholder = placeholder_t,
	  enable_if_t<constraints<Placeholder, Args...>::direct_constructible &&
		  constraints<Placeholder, Args...>::implicitly_direct_constructible, bool> = false>
  constexpr tuple(const Args &... args) : tail_t(args...) {}

  template<typename Placeholder = placeholder_t,
	  enable_if_t<constraints<Placeholder, Args...>::direct_constructible &&
		  !constraints<Placeholder, Args...>::implicitly_direct_constructible, bool> = true>
  explicit constexpr tuple(const Args &... args) : tail_t(args...) {}

  template<typename Placeholder = placeholder_t, typename... Params,
	  enable_if_t<constraints<Placeholder, Args...>::template converting_constructible<Params...> &&
		  constraints<Placeholder, Args...>::template implicitly_converting_constructible<Params...>, bool> = false>
  constexpr tuple(Params &&... params) : tail_t(forward<Params>(params)...) {}

  template<typename Placeholder = placeholder_t, typename... Params,
	  enable_if_t<constraints<Placeholder, Args...>::template converting_constructible<Params...> &&
		  !constraints<Placeholder, Args...>::template implicitly_converting_constructible<Params...>, bool> = true>
  explicit constexpr tuple(Params &&... params) : tail_t(forward<Params>(params)...) {}

  template<typename Placeholder = placeholder_t, typename... Params,
	  enable_if_t<constraints<Placeholder, Args...>::template converting_copy_constructible<Params...> &&
		  constraints<Placeholder, Args...>::template implicitly_converting_copy_constructible<Params...>, bool>
	  = false>
  constexpr tuple(const tuple<Params...> &other)
	  : tail_t(static_cast<const _impl::tuple_cons<0, Params...> &>(other)) {}

  template<typename Placeholder = placeholder_t, typename... Params,
	  enable_if_t<constraints<Placeholder, Args...>::template converting_copy_constructible<Params...> &&
		  !constraints<Placeholder, Args...>::template implicitly_converting_copy_constructible<Params...>, bool>
	  = true>
  explicit constexpr tuple(const tuple<Params...> &other)
	  : tail_t(static_cast<const _impl::tuple_cons<0, Params...> &>(other)) {}

  template<typename Placeholder= placeholder_t, typename... Params,
	  enable_if_t<constraints<Placeholder, Args...>::template converting_move_constructible<Params...> &&
		  constraints<Placeholder, Args...>::template implicitly_converting_move_constructible<Params...>, bool>
	  = false>
  constexpr tuple(tuple<Params...> &&other) : tail_t(static_cast< _impl::tuple_cons<0, Params...> &&>(other)) {}

  template<typename Placeholder = placeholder_t, typename... Params,
	  enable_if_t<constraints<Placeholder, Args...>::template converting_move_constructible<Params...> &&
		  !constraints<Placeholder, Args...>::template implicitly_converting_move_constructible<Params...>, bool>
	  = true>
  explicit constexpr tuple(tuple<Params...> &&other)
	  : tail_t(static_cast<_impl::tuple_cons<0, Params...> &&>(other)) {}

  constexpr tuple(const tuple &) = default;

  constexpr tuple(tuple &&) noexcept = default;
}; // class tuple
} // namespace eureka
#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_
#pragma clang diagnostic pop
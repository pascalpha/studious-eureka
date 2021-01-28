#pragma clang diagnostic push
//#pragma ide diagnostic ignored "HidingNonVirtualFunction"
//#pragma ide diagnostic ignored "bugprone-forwarding-reference-overload"

//
// Created by admin on 2021/1/27.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_

#include "eureka/traits/traits.h"

using namespace eureka;

namespace eureka_impl {
namespace _impl {
// end of cons list
static constexpr struct end_of_cons_t {} end_of_cons;

template<size_t Index, typename Element>
struct head {
  Element value;

  constexpr head() : value() {}

  constexpr head(const Element &h) : value(h) {}

  constexpr head(const head &) = default;

  constexpr head(head &&) noexcept = default;

  template<typename Param>
  using head_forwarding_construct_enabler = enable_if_t<is_same_v<remove_reference_t<Param>, head>, placeholder_t>;

  template<typename Param, typename = head_forwarding_construct_enabler<Param>>
  constexpr head(Param &&param): value(forward<Param>(param)) {}

  constexpr Element &element() noexcept { return value; }

  constexpr const Element &element() const noexcept { return value; }
};

template<size_t Index, typename... Elements>
struct cons;

template<size_t Index, typename Head, typename... Tail>
struct cons<Index, Head, Tail...>
    : public cons<Index + 1, Tail...>, private head<Index, Head> {

  template<size_t, typename...> friend
  class cons;

  using next_t = cons<Index + 1, Tail...>;
  using head_t = head<Index, Head>;
  using index_t = arithmetic_constant<size_t, Index>;

  constexpr Head &get_head(index_t) { return head_t::element(); }

  constexpr const Head &get_head(index_t) const { return head_t::element(); }

  constexpr next_t &tail(index_t) { return *this; }

  constexpr const next_t &tail(index_t) const { return *this; }

  constexpr cons() : next_t(), head_t() {}

  explicit constexpr cons(const Head &h, const Tail &...t) : next_t(t...), head_t(h) {}

  template<typename... TailParam>
  using cons_forwarding_construct_enabler = enable_if_t<sizeof...(TailParam) == sizeof...(Tail), placeholder_t>;

  template<typename HeadParam, typename... TailParam, typename = cons_forwarding_construct_enabler<TailParam...>>
  explicit constexpr cons(HeadParam &&hp, TailParam &&... tp)
      : next_t(forward<TailParam>(tp)...), head_t(forward<HeadParam>(hp)) {}

  constexpr cons(const cons &) = default;

  cons &operator=(const cons &) = delete;

  constexpr cons(cons &&c) noexcept
      : next_t(move(c.tail(cons::index_t()))), head_t(forward<Head>(c.get_head(cons::index_t()))) {}

  template<typename... Params>
  constexpr cons(const cons<Index, Params...> &c)
      : next_t(c.tail(cons::index_t())), head_t(c.get_head(cons::index_t())) {}
};

template<size_t Index, typename Head>
struct cons<Index, Head> : private head<Index, Head> {
  template<size_t, typename...> friend
  class cons;

  using head_t = head<Index, Head>;
  using index_t = arithmetic_constant<size_t, Index>;

  constexpr Head &get_head(index_t) { return head_t::element(); }

  constexpr const Head &get_head(index_t) const { return head_t::element(); }

  constexpr cons() : head_t() {}

  explicit constexpr cons(const head_t &h) : head_t{h} {}

  template<typename Param>
  explicit constexpr cons(Param &&param) : head_t(forward<Param>(param)) {}

  constexpr cons(const cons &) = default;

  cons &operator=(const cons &) = delete;

  constexpr cons(cons &&c) noexcept: head_t(forward<Head>(c.get_head(index_t()))) {}

  template<typename HeadParam>
  constexpr cons(const cons<Index, HeadParam> &c) : head_t(c.get_head(index_t())) {}

  template<typename HeadParam>
  constexpr cons(cons<Index, HeadParam> &&c) : head_t(forward<HeadParam>(c.get_head(index_t()))) {}

};
template<typename... Elements>
struct tuple;

template<bool, typename... Args>
struct tuple_construction_constraints {
  static constexpr bool default_constructible = conjunction_v<is_default_constructible<Args>...>;
  static constexpr bool implicitly_default_constructible = conjunction_v<is_implicitly_constructible<Args>...>;
  template<typename... Params>
  static constexpr bool copy_constructible = conjunction_v<is_constructible<Args, const Params &>...>;
  template<typename... Params>
  static constexpr bool implicitly_copy_constructible = conjunction_v<is_convertible<const Params &, Args>...>;
  template<typename... Params>
  static constexpr bool move_constructible = conjunction_v<is_constructible<Args, Params &&>...>;
  template<typename... Params>
  static constexpr bool implicitly_move_constructible = conjunction_v<is_convertible<Params &&, Args>...>;
};

template<typename... Args>
struct tuple_construction_constraints<false, Args...> {
  static constexpr bool default_constructible = false;
  static constexpr bool implicitly_default_constructible = false;
  template<typename...>
  static constexpr bool copy_constructible = false;
  template<typename...>
  static constexpr bool implicitly_copy_constructible = false;
  template<typename...>
  static constexpr bool move_constructible = false;
  template<typename...>
  static constexpr bool implicitly_move_constructible = false;
};
} // namespace _impl

template<typename... Elements>
struct tuple : public _impl::cons<0, Elements...> {
  using next_t = _impl::cons<0, Elements...>;

  using constraints = _impl::tuple_construction_constraints<true, Elements...>;

  template<enable_if_t<constraints::default_constructible
                           && constraints::implicitly_default_constructible, bool> = false>
  constexpr tuple() : next_t() {}

  template<enable_if_t<constraints::default_constructible
                           && !constraints::implicitly_default_constructible, bool> = true>
  explicit constexpr tuple() : next_t() {}

  template<enable_if_t<(sizeof...(Elements) >= 1)
                           && constraints::template copy_constructible<Elements...>
                           && constraints::template implicitly_copy_constructible<Elements...>, bool> = false>
  constexpr tuple(const Elements &... elements): next_t(elements...) {}

  template<enable_if_t<(sizeof...(Elements) >= 1)
                           && constraints::template copy_constructible<Elements...>
                           && !constraints::template implicitly_copy_constructible<Elements...>, bool> = true>
  explicit constexpr tuple(const Elements &... elements): next_t(elements...) {}

  constexpr tuple(const tuple &) = default;

  constexpr tuple(tuple &&) noexcept = default;
};
} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_

#pragma clang diagnostic pop
#pragma clang diagnostic pop
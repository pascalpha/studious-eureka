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

template<bool, typename... Args>
struct tuple_construction_constraints {

};
} // namespace _impl

template<typename... Args>
struct tuple : _impl::tuple_cons<0, Args...> {
  using tail_t = _impl::tuple_cons<0, Args...>;

  constexpr tuple() : tail_t() {}

  constexpr tuple(const Args &... args) : tail_t(args...) {}

  constexpr tuple(Args &&... args) : tail_t(forward<Args>(args)...) {}

  constexpr tuple(const tuple &) = default;

  constexpr tuple(tuple &&) noexcept = default;
}; // class tuple
} // namespace eureka
#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_TUPLE_H_
#pragma clang diagnostic pop
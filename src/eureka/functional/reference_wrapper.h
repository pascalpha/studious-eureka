#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-forwarding-reference-overload"
#pragma ide diagnostic ignored "google-explicit-constructor"
//
// Created by admin on 2021/1/26.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_FUNCTIONAL_REFERENCE_WRAPPER_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_FUNCTIONAL_REFERENCE_WRAPPER_H_

#include "eureka/traits/traits.h"
#include "eureka/utility/utility.h"
#include "eureka/memory/pointer.h"

namespace eureka {
namespace _impl {
template<typename Arg>
constexpr Arg &reference_wrap(Arg &arg) noexcept { return arg; }
template<typename Arg>
void reference_wrap(Arg &&) = delete;
} // namespace _impl
template<typename Arg>
class reference_wrapper {
  Arg *ptr;

  template<typename Param>
  using reference_wrapper_enabler = decltype(
  _impl::reference_wrap<Arg>(declared_value<Param>()),
	  enable_if_t<!is_same_v<reference_wrapper, remove_const_volatile_reference<Param >>, placeholder_t>());

 public:
  using type = Arg;

  /**
   * construct a reference wrapper from a reference value
   * @tparam Param
   * @param param
   */
  template<typename Param, typename = reference_wrapper_enabler<Param>>
  constexpr reference_wrapper(Param &&param)
  noexcept(noexcept(_impl::reference_wrap<Arg>(forward<Param>(param))))
	  : ptr(address_of(_impl::reference_wrap<Arg>(forward<Param>(param)))) {}

  /**
   * copy constructor
   */
  reference_wrapper(const reference_wrapper &) noexcept = default;

  /**
   * assignment
   * @return
   */
  reference_wrapper &operator=(const reference_wrapper &) noexcept = default;

  /**
   * implicit accessor
   * @return
   */
  constexpr operator Arg &() const noexcept { return *ptr; }

  /**
   * explicit accessor
   * @return
   */
  constexpr Arg &get() const noexcept { return *ptr; }

  // todo invoke interface

}; // class reference wrapper

/**
 * deduction guide
 */
template<typename Arg>
reference_wrapper(Arg &)-> reference_wrapper<Arg>;

/**
 * \brief type traits for reference_wrapper
 * \tparam Arg
 */
template<typename Arg>
struct is_reference_wrapper : false_t {};
template<typename Arg>
struct is_reference_wrapper<reference_wrapper<Arg>> : true_t {};
eureka_value_helper_macro(is_reference_wrapper);

/**
 * \brief make a reference_wrapper from an lvalue reference
 * \tparam Arg
 * \param arg
 * \return
 */
template<typename Arg>
inline reference_wrapper<Arg> ref(Arg &arg) noexcept { return reference_wrapper<Arg>(arg); }
/**
 * \brief make a reference_wrapper from a const lvalue reference
 * \tparam Arg
 * \param arg
 * \return
 */
template<typename Arg>
inline reference_wrapper<const Arg> cref(const Arg &arg) noexcept { return reference_wrapper<const Arg>(arg); }
/**
 * \brief make a reference_wrapper from an rvalue reference
 * \tparam Arg
 */
template<typename Arg>
void ref(const Arg &&) = delete;
/**
 * \brief make a reference_wrapper from a const rvalue reference
 * \tparam Arg
 */
template<typename Arg>
void cref(const Arg &&) = delete;
/**
 * \brief prevent reference_wrappers to degenerate when making new reference wrappers
 * \tparam Arg
 * \param arg
 * \return
 */
template<typename Arg>
inline reference_wrapper<Arg>
ref(reference_wrapper<Arg> arg) noexcept { return arg; }
/**
 * \brief prevent reference_wrappers to degenerate when making new reference wrappers
 * \tparam Arg
 * \param arg
 * \return
 */
template<typename Arg>
inline reference_wrapper<const Arg>
cref(reference_wrapper<Arg> arg) noexcept { return {arg.get()}; }
}  // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_FUNCTIONAL_REFERENCE_WRAPPER_H_
#pragma clang diagnostic pop
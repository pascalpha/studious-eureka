#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
//
// Created by mamin on 1/25/2021.
//
#ifndef STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_PAIR_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_PAIR_H_

#include "eureka/traits/traits.h"
#include "eureka/utility/utility.h"

namespace eureka {

namespace _impl {
template<typename First, typename Second, bool>
/**
 * \brief constants guarding the overloaded constructor resolution
 * \tparam First
 * \tparam Second
 */
struct pair_construction_constraints {
  template<typename FArg, typename SArg>
  constexpr bool static constructible
	  = conjunction_v<is_constructible<First, const FArg &>, is_constructible<Second, const SArg &>>;

  template<typename FArg, typename SArg>
  constexpr bool static implicitly_convertible
	  = conjunction_v<is_convertible<const FArg &, First>, is_convertible<const SArg &, Second>>;

  template<typename FArg, typename SArg>
  constexpr bool static move_constructible
	  = conjunction_v<is_constructible<First, FArg &&>, is_constructible<Second, SArg &&>>;

  template<typename FArg, typename SArg>
  constexpr bool static implicitly_move_convertible
	  = conjunction_v<is_convertible<FArg &&, First>, is_convertible<SArg &&, Second>>;

  template<typename FArg, typename SArg>
  constexpr bool static copy_move_constructible
	  = conjunction_v<is_constructible<First, const FArg &>, is_constructible<Second, SArg &&>>;

  template<typename FArg, typename SArg>
  constexpr bool static implicitly_copy_move_constructible
	  = conjunction_v<is_convertible<const FArg &, First>, is_convertible<SArg &&, Second>>;

  template<typename FArg, typename SArg>
  constexpr bool static move_copy_constructible
	  = conjunction_v<is_constructible<First, FArg &&>, is_constructible<Second, const SArg &>>;

  template<typename FArg, typename SArg>
  constexpr bool static implicitly_move_copy_constructible
	  = conjunction_v<is_convertible<FArg &&, First>, is_convertible<const SArg &, Second>>;
};

/**
 * \brief disable incomplete types
 * \tparam First
 * \tparam Second
 */
template<typename First, typename Second>
struct pair_construction_constraints<First, Second, false> {
  template<typename FArg, typename SArg>
  constexpr bool static constructible = false;
  template<typename FArg, typename SArg>
  constexpr bool static implicitly_convertible = false;
  template<typename FArg, typename SArg>
  constexpr bool static move_constructible = false;
  template<typename FArg, typename SArg>
  constexpr bool static implicitly_move_convertible = false;
  template<typename FArg, typename SArg>
  constexpr bool static copy_move_constructible = false;
  template<typename FArg, typename SArg>
  constexpr bool static implicitly_copy_move_constructible = false;
  template<typename FArg, typename SArg>
  constexpr bool static move_copy_constructible = false;
  template<typename FArg, typename SArg>
  constexpr bool static implicitly_move_copy_constructible = false;
};
}
/**
 * \brief representation of two values
 * \tparam First
 * \tparam Second
 */
template<typename First, typename Second>
struct pair {
  using first_t = First;
  using second_t = Second;

  first_t first;
  second_t second;

  /**
   * \brief implicit default construct a pair.
   * Present if both types are implicitly constructible.
   *
   * \tparam FArg
   * \tparam SArg
   */
  template<typename FArg = First, typename SArg = Second,
	  enable_if_t<conjunction_v<is_implicitly_constructible<FArg>,
								is_implicitly_constructible<SArg>>, bool> = true>
  constexpr pair() : first(), second() {}

  /**
   * \brief explicit default construct a pair
   * Present if both are default constructible but either one is not implicitly constructible
   *
   * \tparam FArg
   * \tparam SArg
   */
  template<typename FArg = First, typename SArg = Second,
	  enable_if_t<
		  conjunction_v<is_default_constructible<FArg>, is_default_constructible<SArg>,
						negation<conjunction<is_implicitly_constructible<FArg>,
											 is_implicitly_constructible<SArg>>>>, bool> = false>
  constexpr explicit pair() : first(), second() {};

 private:
  /**
   * constraints for constructors taking two params
   */
  using constraints = _impl::pair_construction_constraints<First, Second, true>;

  /**
   * constraints for constructors taking a pair object
   */
  template<typename FArg, typename SArg>
  using pair_constraints = _impl::pair_construction_constraints<
	  First, Second, !is_same_v<FArg, First> || !is_same_v<SArg, Second>>;

  /**
   * \brief well formed if both types are copy assignable
   */
  using copy_assignment_enabler
  = conditional_t<(is_copy_assignable_v<First> && is_copy_assignable_v<Second>),
				  pair, none_such>;

  /**
   * \brief well formed if both types are move assignable
   */
  using move_assignment_enabler
  = conditional_t<(is_move_assignable_v<First> && is_move_assignable_v<Second>),
				  pair, none_such>;

  /**
   * \brief well formed if both types of the pair are move assignable
   */
  template<typename FArg, typename SArg>
  using pair_assignment_enabler
  = conditional_t<(is_assignable_v<First &, FArg &&> && is_assignable_v<Second &, SArg &&>),
				  pair<FArg, SArg>, none_such>;

 public:
  /**
   * copy constructors taking two const references.
   * two types must be both implicitly copy constructible.
   *
   * @tparam FArg
   * @tparam SArg
   * @param f
   * @param s
   */
  template<typename FArg = First, typename SArg = Second,
	  enable_if_t<(constraints::template constructible<FArg, SArg>
		  && constraints::template implicitly_convertible<FArg, SArg>), bool> = true>
  constexpr pair(const First &f, const Second &s): first(f), second(s) {}

  /**
   * explicit copy constructor taking two const references.
   *
   * @tparam FArg
   * @tparam SArg
   * @param f
   * @param s
   */
  template<typename FArg = First, typename SArg = Second,
	  enable_if_t<(constraints::template constructible<FArg, SArg>
		  && !constraints::template implicitly_convertible<FArg, SArg>), bool> = false>
  explicit constexpr pair(const First &f, const Second &s): first(f), second(s) {}

  /**
   * copy constructor taking a const reference to a pair of values.
   * both types must be implicitly constructible.
   *
   * @tparam FArg
   * @tparam SArg
   * @param p
   */
  template<typename FArg, typename SArg,
	  enable_if_t<pair_constraints<FArg, SArg>::template constructible<FArg, SArg>
					  && pair_constraints<FArg, SArg>::template implicitly_convertible<FArg, SArg>, bool> = true>
  constexpr pair(const pair<FArg, SArg> &p) : first(p.first), second(p.second) {}

  /**
   * explicit copy constructor taking a const reference to a pair of values.
   *
   * @tparam FArg
   * @tparam SArg
   * @param p
   */
  template<typename FArg, typename SArg,
	  enable_if_t<pair_constraints<FArg, SArg>::template constructible<FArg, SArg>
					  && !pair_constraints<FArg, SArg>::template implicitly_convertible<FArg, SArg>, bool> = false>
  explicit constexpr pair(const pair<FArg, SArg> &p) : first(p.first), second(p.second) {}

  /**
   * default copy constructor
   */
  constexpr pair(const pair &) = default;

  /**
   * default move constructor
   */
  constexpr pair(pair &&) noexcept = default;

  /**
   * move copy constructor
   *
   * @tparam FArg
   * @param f
   * @param s
   */
  template<typename FArg, enable_if_t<constraints::template move_copy_constructible<FArg, Second>, bool> = true>
  constexpr pair(FArg &&f, const Second &s) : first(forward<FArg>(f)), second(s) {}

  /**
   * move copy constructor
   *
   * @tparam FArg
   * @param f
   * @param s
   */
  template<typename FArg,
	  enable_if_t<constraints::template implicitly_move_copy_constructible<FArg, Second>, bool> = false>
  explicit constexpr pair(FArg &&f, const Second &s): first(forward<FArg>(f)), second(s) {}

  /**
   * copy move constructor
   *
   * @tparam SArg
   * @param f
   * @param s
   */
  template<typename SArg, enable_if_t<constraints::template copy_move_constructible<First, SArg>, bool> = true>
  constexpr pair(const First &f, SArg &&s):first(f), second(forward<SArg>(s)) {}

  /**
   * copy move constructor
   *
   * @tparam SArg
   * @param f
   * @param s
   */
  template<typename SArg,
	  enable_if_t<constraints::template implicitly_copy_move_constructible<First, SArg>, bool> = false>
  explicit constexpr pair(const First &f, SArg &&s): first(f), second(forward<SArg>(s)) {}

  /**
   * move constructor taking two rvalue references
   *
   * @tparam FArg
   * @tparam SArg
   * @param f
   * @param s
   */
  template<typename FArg, typename SArg,
	  enable_if_t<constraints::template move_constructible<FArg, SArg>
					  && constraints::template implicitly_move_convertible<FArg, SArg>, bool> = true>
  constexpr pair(FArg &&f, SArg &&s) : first(forward<FArg>(f)), second(forward<SArg>(s)) {}

  /**
   * move constructor taking two rvalue references
   *
   * @tparam FArg
   * @tparam SArg
   * @param f
   * @param s
   */
  template<typename FArg, typename SArg,
	  enable_if_t<constraints::template move_constructible<FArg, SArg>
					  && !constraints::template implicitly_move_convertible<FArg, SArg>, bool> = false>
  explicit constexpr pair(FArg &&f, SArg &&s) : first(forward<FArg>(f)), second(forward<SArg>(s)) {}

  /**
   * move constructor taking an rvalue reference to a pair of values
   *
   * @tparam FArg
   * @tparam SArg
   * @param p
   */
  template<typename FArg, typename SArg,
	  enable_if_t<pair_constraints<FArg, SArg>::template move_constructible<FArg, SArg>
					  && pair_constraints<FArg, SArg>::template implicitly_move_convertible<FArg, SArg>, bool> = true>
  constexpr pair(pair<FArg, SArg> &&p): first(forward<FArg>(p.first)), second(forward<SArg>(p.second)) {}

  /**
   * move constructor taking an rvalue reference to a pair of values
   *
   * @tparam FArg
   * @tparam SArg
   * @param p
   */
  template<typename FArg, typename SArg,
	  enable_if_t<pair_constraints<FArg, SArg>::template move_constructible<FArg, SArg>
					  && !pair_constraints<FArg, SArg>::template implicitly_move_convertible<FArg, SArg>, bool> = false>
  explicit constexpr pair(pair<FArg, SArg> &&p): first(forward<FArg>(p.first)), second(forward<SArg>(p.second)) {}

  /*TODO piecewise construct not implemented due to lack of tuple class*/

 public:
  /**
   * \brief copy assignment operator, enabled only of both param types are copy assignable
   * \param oth
   * \return
   */
  pair &operator=(const copy_assignment_enabler &oth) {
	first = oth.first;
	second = oth.second;
	return *this;
  }

  /**
   * \brief move assignment operator, enabled only if both param types are copy assignable
   * \param oth
   * \return
   */
  pair &operator=(move_assignment_enabler &&oth) {
	first = forward<First>(oth.first);
	second = forward<Second>(oth.second);
	return *this;
  }

  /**
   * \brief move assignment operator from an rvalue reference to a pair of move assignable values
   * \tparam FArg
   * \tparam SArg
   * \param oth
   * \return
   */
  template<typename FArg, typename SArg>
  pair &operator=(pair_assignment_enabler<FArg, SArg> &&oth) {
	first = forward<FArg>(oth.first);
	second = forward<SArg>(oth.second);
	return *this;
  }

  /**
   * \brief swap *this with oth
   * \param oth
   */
  void swap(pair &oth) {
	eureka::swap(first, oth.first);
	eureka::swap(second, oth.second);
  }
}; // class pair

namespace _impl {
template<typename First, typename Second>
using pair_swap_enabler = enable_if_t<is_swappable_v<First> && is_swappable_v<Second>, void>;
} // namespace _impl

template<typename First, typename Second>
constexpr _impl::pair_swap_enabler<First, Second> swap(pair<First, Second> &x, pair<First, Second> &y) noexcept {
  x.swap(y);
}

template<typename First, typename Second>
constexpr auto make_pair(First &&f, Second &&s) -> decltype(auto) {
  return pair(forward<First>(f), forward<Second>(s));
}

/**
 * \brief equality operator, true if both corresponding values are equal
 * \tparam First
 * \tparam Second
 * \param x
 * \param y
 * \return
 */
template<typename First, typename Second>
inline constexpr bool
operator==(const pair<First, Second> &x, const pair<First, Second> &y) {
  return x.first == y.first && x.second == y.second;
}

/**
 * \brief inequality operator
 * \tparam First
 * \tparam Second
 * \param x
 * \param y
 * \return
 */
template<typename First, typename Second>
inline constexpr bool
operator!=(const pair<First, Second> &x, const pair<First, Second> &y) {
  return !(x == y);
}

/**
 * \brief less than operator
 * \tparam First
 * \tparam Second
 * \param x
 * \param y
 * \return
 */
template<typename First, typename Second>
inline constexpr bool
operator<(const pair<First, Second> &x, const pair<First, Second> &y) {
  return x.first < y.first
	  || (!(y.first < x.first) && (x.second < y.second));
}

/**
 * \brief greater than operator
 * \tparam First
 * \tparam Second
 * \param x
 * \param y
 * \return
 */
template<typename First, typename Second>
inline constexpr bool
operator>(const pair<First, Second> &x, const pair<First, Second> &y) {
  return y < x;
}

/**
 * \brief less than or equal operator
 * \tparam First
 * \tparam Second
 * \param x
 * \param y
 * \return
 */
template<typename First, typename Second>
inline constexpr bool
operator<=(const pair<First, Second> &x, const pair<First, Second> &y) {
  return !(y < x);
}

/**
 * \brief greater than or equal operator
 * \tparam First
 * \tparam Second
 * \param x
 * \param y
 * \return
 */
template<typename First, typename Second>
inline constexpr bool
operator>=(const pair<First, Second> &x, const pair<First, Second> &y) {
  return !(x < y);
}

} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_UTILITY_PAIR_H_

#pragma clang diagnostic pop
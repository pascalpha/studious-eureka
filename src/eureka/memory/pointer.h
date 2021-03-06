//
// Created by admin on 2021/1/22.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_POINTER_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_POINTER_H_

#include "eureka/traits/traits.h"

#include "general.h"

namespace eureka {

namespace _impl {
/*element is Ptr::element_t if present*/
template<typename Ptr>
auto pointer_access_element_impl(nullptr_t) -> typename Ptr::element_t;
/*or Arg if Ptr = Template<Arg, ...>*/
template<typename Ptr>
auto pointer_access_element_impl(nullptr_t *) -> first_template_argument_t<Ptr>;
/*else ill-formed*/

/*difference is Ptr::difference_t if present*/
template<typename Ptr>
auto pointer_access_difference_impl(nullptr_t) -> typename Ptr::difference_t;
/*or ptrdiff_t if not*/
template<typename Ptr>
auto pointer_access_difference_impl(nullptr_t *) -> ptrdiff_t;

template<typename Ptr, typename Rebind>
auto pointer_access_rebind_impl(nullptr_t) -> typename Ptr::template rebind<Rebind>;
template<typename Ptr, typename Rebind>
auto pointer_access_rebind_impl(nullptr_t *) -> rebind_first_template_argument_t<Ptr, Rebind>;
} //  namespace _impl

template<typename Ptr>
struct pointer_access {
  using pointer_t = Ptr;
  using element_t = decltype(_impl::pointer_access_element_impl<Ptr>(nullptr));
  using difference_t = decltype(_impl::pointer_access_difference_impl<Ptr>(nullptr));

  template<typename Rebind>
  using rebind = decltype(_impl::pointer_access_rebind_impl<Ptr, Rebind>(nullptr));

  static pointer_t pointer_to(element_t &v) {
    return Ptr::pointer_to(v);
  }
};

template<typename Arg>
struct pointer_access<Arg *> {
  using pointer_t = Arg *;
  using element_t = Arg;
  using difference_t = ptrdiff_t;

  template<typename Rebind>
  using rebind = Rebind *;

  static constexpr pointer_t pointer_to(element_t &v) {
    return address_of(v);
  }
};
} //  namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_POINTER_H_

//
// Created by admin on 2021/1/22.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_POINTER_ACCESS_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_POINTER_ACCESS_H_

namespace eureka {

template<typename Ptr>
pointer_access {
using pointer_t = Ptr;
using element_t = Ptr::element_t;
using difference_t = Ptr::difference_t;
}

} //  namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_MEMORY_POINTER_ACCESS_H_

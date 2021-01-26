#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
//
// Created by admin on 2021/1/26.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_TEST_TEST_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_TEST_TEST_H_

namespace eureka::test {

struct default_constructible_t {};
template<typename... Args>
struct implicit_constructible_t { using types = Arg... };
template<typename... Args>
struct explicit_constructible_t { using types = Arg... };

template<typename>
struct generic_class_t {

};

} // namespace eureka::test

#endif //STUDIOUS_EUREKA_SRC_EUREKA_TEST_TEST_H_

#pragma clang diagnostic pop
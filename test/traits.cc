#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by admin on 2021/1/27.
//

#include <iostream>
#include <memory>
#include <gtest/gtest.h>
#include <eureka/traits/traits.h>
#include <eureka/utility/pair.h>
#include <eureka/utility/tuple.h>
#include <eureka/functional/reference_wrapper.h>

using namespace eureka;

namespace {

class traits_test : ::testing::Test {
  std::tuple<int> t;
  std::unique_ptr<int> p;
};

TEST(traits_test, general) {
  EXPECT_TRUE((is_same_v<conditional_t<true, void, void *>, void>));
  EXPECT_TRUE((is_same_v<conditional_t<false, void, void *>, void *>));
  EXPECT_TRUE((is_same_v<enable_if_t<true, void>, void>));
  EXPECT_TRUE((conjunction_v<true_t, true_t>));
  EXPECT_TRUE((conjunction_v<true_t, true_t, true_t>));
  EXPECT_FALSE((conjunction_v<true_t, false_t>));
  EXPECT_FALSE((conjunction_v<false_t, false_t>));

  EXPECT_TRUE((disjunction_v<true_t, true_t>));
  EXPECT_TRUE((conjunction_v<true_t, true_t, true_t>));
  EXPECT_FALSE((conjunction_v<true_t, false_t>));
  EXPECT_FALSE((conjunction_v<false_t, true_t>));
  EXPECT_FALSE((conjunction_v<false_t, false_t>));

  EXPECT_TRUE(negation_v<false_t>);
  EXPECT_TRUE(!negation_v<true_t>);

  EXPECT_TRUE(is_const_v<const int>);
  EXPECT_FALSE(is_const_v<volatile int>);
  EXPECT_TRUE(is_const_v<const volatile int>);
  EXPECT_FALSE(is_const_v<int>);

  EXPECT_FALSE(is_volatile_v<const int>);
  EXPECT_TRUE(is_volatile_v<volatile int>);
  EXPECT_TRUE(is_volatile_v<const volatile int>);
  EXPECT_FALSE(is_volatile_v<int>);

  EXPECT_EQ(rank_v<int[][2][2][2]>, 4);
  EXPECT_EQ(rank_v<int[][2][2]>, 3);
  EXPECT_EQ(rank_v<int[][2]>, 2);
  EXPECT_EQ(rank_v<int[]>, 1);

  EXPECT_EQ(rank_v<int[1][2][2][2]>, 4);
  EXPECT_EQ(rank_v<int[1][2][2]>, 3);
  EXPECT_EQ(rank_v<int[1][2]>, 2);
  EXPECT_EQ(rank_v<int[1]>, 1);

  EXPECT_EQ((extent_v<int[1][2][3][4], 0>), 1);
  EXPECT_EQ((extent_v<int[1][2][3][4], 1>), 2);
  EXPECT_EQ((extent_v<int[1][2][3][4], 2>), 3);
  EXPECT_EQ((extent_v<int[1][2][3][4], 3>), 4);
  EXPECT_EQ((extent_v<int[][2][3][4], 0>), 0);
}

TEST(traits_test, categorization) {
  EXPECT_TRUE((is_void_v<void>));
  EXPECT_TRUE((is_void_v<const void>));
  EXPECT_TRUE((is_void_v<const volatile void>));
  EXPECT_FALSE((is_void_v<void *>));
  EXPECT_TRUE(is_pointer_v<int *>);
  EXPECT_TRUE(is_pointer_v<double *>);
  EXPECT_FALSE(is_pointer_v<nullptr_t>);
  EXPECT_TRUE(is_null_pointer_v<nullptr_t>);
  EXPECT_FALSE(is_null_pointer_v<int *>);

  EXPECT_FALSE(is_array_v<int>);
  EXPECT_FALSE(is_array_v<int *>);
  EXPECT_TRUE((is_array_v<int[]>));
  EXPECT_TRUE((is_array_v<int[5]>));
  EXPECT_TRUE((is_array_v<int[5][8]>));
  EXPECT_TRUE((is_array_v<int[][8]>));

  EXPECT_FALSE(is_unbounded_array_v<int>);
  EXPECT_FALSE(is_unbounded_array_v<int *>);
  EXPECT_TRUE((is_unbounded_array_v<int[]>));
  EXPECT_FALSE((is_unbounded_array_v<int[5]>));
  EXPECT_FALSE((is_unbounded_array_v<int[5][8]>));
  EXPECT_TRUE((is_unbounded_array_v<int[][8]>));

  EXPECT_FALSE(is_bounded_array_v<int>);
  EXPECT_FALSE(is_bounded_array_v<int *>);
  EXPECT_FALSE((is_bounded_array_v<int[]>));
  EXPECT_TRUE((is_bounded_array_v<int[5]>));
  EXPECT_TRUE((is_bounded_array_v<int[5][8]>));
  EXPECT_FALSE((is_bounded_array_v<int[][8]>));

  EXPECT_FALSE(is_lvalue_reference_v<int>);
  EXPECT_TRUE(is_lvalue_reference_v<int &>);
  EXPECT_TRUE(is_lvalue_reference_v<const int &>);
  EXPECT_FALSE(is_lvalue_reference_v<int &&>);
  EXPECT_FALSE(is_lvalue_reference_v<const int &&>);

  EXPECT_FALSE(is_rvalue_reference_v<int>);
  EXPECT_FALSE(is_rvalue_reference_v<int &>);
  EXPECT_FALSE(is_rvalue_reference_v<const int &>);
  EXPECT_TRUE(is_rvalue_reference_v<int &&>);
  EXPECT_TRUE(is_rvalue_reference_v<const int &&>);

  EXPECT_FALSE(is_reference_v<int>);
  EXPECT_TRUE(is_reference_v<int &>);
  EXPECT_TRUE(is_reference_v<const int &>);
  EXPECT_TRUE(is_reference_v<int &&>);
  EXPECT_TRUE(is_reference_v<const int &&>);
}

TEST(traits_test, relations) {
  struct base {
	base() {
	  std::cout << "constructing base" << std::endl;
	}
	~base() {
	  std::cout << "destructing base" << std::endl;
	}
  };
  struct derived : base {};
  EXPECT_TRUE((is_same_v<common_type_t<int, unsigned>, unsigned>));
  EXPECT_TRUE((is_same_v<common_type_t<long, long long>, long long>));
  EXPECT_TRUE((is_same_v<common_type_t<int, int>, int>));
  EXPECT_TRUE((is_same_v<common_type_t<float, double>, double>));

  EXPECT_TRUE((is_same_v<common_type_t<base &, base &&>, base>));
  EXPECT_TRUE((is_same_v<common_type_t<base &, const base &>, base>));
  EXPECT_TRUE((is_same_v<common_type_t<base, const base>, base>));
  EXPECT_TRUE((is_same_v<common_type_t<base, derived>, base>));
  EXPECT_TRUE((is_same_v<common_type_t<int *, nullptr_t>, int *>));
  EXPECT_TRUE((is_same_v<common_type_t<base *, derived *>, base *>));
  EXPECT_TRUE((is_same_v<common_type_t<int[], int[5], int[8]>, int *>));

  EXPECT_TRUE((is_same_v<common_type_t<void(), void (&)(), void (*)()>, void (*)()>));

  EXPECT_TRUE((is_same_v<common_type_t<int &, reference_wrapper<int>>, int>));
  // this behavior is consistent with the standard ones, underlying theory not understood
  EXPECT_TRUE((is_same_v<common_type_t<int, reference_wrapper<double>>, int>));
  EXPECT_TRUE((is_same_v<common_type_t<int, double &>, double>));
}

TEST(traits_test, transformations) {
  EXPECT_TRUE((is_same_v<add_const_t<int>, const int>));
  EXPECT_TRUE((is_same_v<add_volatile_t<int>, volatile int>));
  EXPECT_TRUE((is_same_v<add_const_volatile_t<int>, const volatile int>));

  EXPECT_TRUE((is_same_v<remove_const_t<const int>, int>));
  EXPECT_TRUE((is_same_v<remove_volatile_t<volatile int>, int>));
  EXPECT_TRUE((is_same_v<remove_const_volatile_t<const volatile int>, int>));

  EXPECT_TRUE((is_same_v<add_pointer_t<int>, int *>));
  EXPECT_TRUE((is_same_v<add_pointer_t<void>, void *>));
  EXPECT_TRUE((is_same_v<add_pointer_t<int[]>, int (*)[]>));
  EXPECT_TRUE((is_same_v<add_pointer_t<volatile int>, volatile int *>));
  EXPECT_TRUE((is_same_v<add_pointer_t<int &>, int *>));
  EXPECT_TRUE((is_same_v<add_pointer_t<void (*)()>, void (**)()>));

  EXPECT_TRUE((is_same_v<add_lvalue_reference_t<void()>, void (&)()>));
  EXPECT_TRUE((is_same_v<add_lvalue_reference_t<int>, int &>));
  EXPECT_TRUE((is_same_v<add_lvalue_reference_t<void>, void>));
  EXPECT_TRUE((is_same_v<add_lvalue_reference_t<int[]>, int (&)[]>));

  EXPECT_TRUE((is_same_v<add_rvalue_reference_t<void()>, void (&&)()>));
  EXPECT_TRUE((is_same_v<add_rvalue_reference_t<int>, int &&>));
  EXPECT_TRUE((is_same_v<add_rvalue_reference_t<void>, void>));
  EXPECT_TRUE((is_same_v<add_rvalue_reference_t<int[]>, int (&&)[]>));

  EXPECT_TRUE((is_same_v<remove_reference_t<int &>, int>));
  EXPECT_TRUE((is_same_v<remove_reference_t<int &&>, int>));
  EXPECT_TRUE((is_same_v<remove_reference_t<int (&)[]>, int[]>));
  EXPECT_TRUE((is_same_v<remove_reference_t<void (*)()>, void (*)()>));
  EXPECT_TRUE((is_same_v<remove_reference_t<int *&>, int *>));

  EXPECT_TRUE((is_same_v<remove_const_volatile_t<const volatile void>, void>));
  EXPECT_TRUE((is_same_v<remove_const_volatile_t<const volatile int &>, const volatile int &>));
  EXPECT_TRUE((is_same_v<remove_const_volatile_t<const volatile int &&>, const volatile int &&>));
  EXPECT_TRUE((is_same_v<remove_const_volatile_t<const volatile int>, int>));
  EXPECT_TRUE((is_same_v<remove_const_volatile_t<void (&)()>, void (&)()>));

  EXPECT_TRUE((is_same_v<remove_const_volatile_reference_t<const volatile void>, void>));
  EXPECT_TRUE((is_same_v<remove_const_volatile_reference_t<const volatile int &>, int>));
  EXPECT_TRUE((is_same_v<remove_const_volatile_reference_t<const volatile int &&>, int>));
  EXPECT_TRUE((is_same_v<remove_const_volatile_reference_t<const volatile int &&>, int>));
  EXPECT_TRUE((is_same_v<remove_const_volatile_reference_t<void (&)()>, void()>));

  EXPECT_TRUE((is_same_v<remove_const_t<const volatile void>, volatile void>));
  EXPECT_TRUE((is_same_v<remove_const_t<const int *const>, const int *>));

  EXPECT_TRUE((is_same_v<remove_pointer_t<const int *>, const int>));
  EXPECT_TRUE((is_same_v<remove_pointer_t<volatile void *>, volatile void>));
  EXPECT_TRUE((is_same_v<remove_pointer_t<int ***const volatile>, int **>));
  EXPECT_TRUE((is_same_v<remove_pointer_t<int (*const)[]>, int[]>));

  EXPECT_TRUE((is_same_v<remove_extent_t<int[8]>, int>));
  EXPECT_TRUE((is_same_v<remove_extent_t<int[]>, int>));
  EXPECT_TRUE((is_same_v<remove_extent_t<int[][8]>, int[8]>));
  EXPECT_TRUE((is_same_v<remove_extent_t<int[8][8][8]>, int[8][8]>));
  EXPECT_TRUE((is_same_v<remove_all_extents_t<int[8][8][8]>, int>));
}

struct Explicit {
  explicit Explicit() = default;
  Explicit(const Explicit &) = default;
  Explicit(Explicit &&) noexcept = default;
  Explicit &operator=(const Explicit &) = default;
  Explicit(int &&x) {}
  Explicit(const eureka::nullptr_t &) {}
};

void func(tuple<int, int, Explicit>) {}

TEST(traits_test, categorization_further) {

  EXPECT_TRUE((is_default_constructible_v<Explicit>));
  EXPECT_FALSE((is_implicitly_constructible_v<Explicit>));

  EXPECT_TRUE((is_default_constructible_v<tuple<int, int, long>>));
  EXPECT_TRUE((is_default_constructible_v<tuple<int, int, Explicit>>));
  EXPECT_TRUE((is_implicitly_constructible_v<tuple<int, int, long>>));
  EXPECT_FALSE((is_implicitly_constructible_v<tuple<int, int, Explicit>>));

  EXPECT_TRUE((is_constructible_v<std::tuple<int, int, int>, const int &, const int &, const int &>));
  EXPECT_TRUE((is_constructible_v<std::tuple<int, int, Explicit>, const int &, const int &, const Explicit &>));
  EXPECT_TRUE(
	  (is_constructible_v<std::tuple<int, Explicit, Explicit>, const int &, const Explicit &, const Explicit &>));

  EXPECT_TRUE((is_constructible_v<std::tuple<int, int, int>, int &&, int &&, int &&>));
  EXPECT_TRUE((is_constructible_v<std::tuple<int, int, Explicit>, int &&, int &, Explicit &&>));
  EXPECT_TRUE(
	  (is_constructible_v<std::tuple<int, Explicit, Explicit>, int &&, int &&, int &&>));
}
} // namespace


#pragma clang diagnostic pop
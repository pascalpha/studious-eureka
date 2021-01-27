#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by admin on 2021/1/27.
//

#include <iostream>
#include <memory>
#include <gtest/gtest.h>
#include <eureka/traits/traits.h>
#include <eureka/functional/reference_wrapper.h>

using namespace eureka;

namespace {

class traits_test : ::testing::Test {
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
  struct base {};
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
  EXPECT_TRUE((is_same_v<common_type_t<int, reference_wrapper<double>>, int>));

}
} // namespace


#pragma clang diagnostic pop
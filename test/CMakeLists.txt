enable_testing()

find_package(GTest REQUIRED)
add_executable(traits-test traits.cc)
target_link_libraries(traits-test eureka GTest::GTest GTest::Main)
add_test(traits-test traits-test)


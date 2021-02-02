//
// Created by admin on 2021/2/2.
//

#ifndef STUDIOUS_EUREKA_SRC_EUREKA_EXCEPTION_EXCEPTION_H_
#define STUDIOUS_EUREKA_SRC_EUREKA_EXCEPTION_EXCEPTION_H_

namespace eureka {
class exception {
 public:
  explicit exception() noexcept = default;
  exception(const exception &) = default;
  exception(exception &&) noexcept = default;
  exception &operator=(const exception &) = default;
  exception &operator=(exception &&) noexcept = default;
  virtual ~exception() = default;

  [[nodiscard]] virtual const char *what() const noexcept { return ""; }
};
} // namespace eureka

#endif //STUDIOUS_EUREKA_SRC_EUREKA_EXCEPTION_EXCEPTION_H_

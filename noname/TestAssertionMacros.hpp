#ifndef NONAME_ASSERTION_MACROS
#define NONAME_ASSERTION_MACROS

#include <cstdlib>
#include <iostream>

#define ASSERT_NO_THROW(expression)                                            \
  {                                                                            \
    try {                                                                      \
      expression;                                                              \
    } catch (...) {                                                            \
      std::cerr << __FILE__ << ":" << __LINE__ << " " << #expression           \
                << " did throw an error\n";                                    \
      std::abort();                                                            \
    }                                                                          \
  }
#define ASSERT_THROW(expression, exception_type)                               \
  {                                                                            \
    try {                                                                      \
      expression;                                                              \
      std::cerr << __FILE__ << ":" << __LINE__ << " " << #expression           \
                << " did not throw " << #exception_type << "\n";               \
      std::abort();                                                            \
    } catch (exception_type const &) {                                         \
      /*success*/                                                              \
    } catch (...) {                                                            \
      std::cerr << __FILE__ << ":" << __LINE__ << " " << #expression           \
                << " did throw but exception type was not " << #exception_type \
                << "\n";                                                       \
      std::abort();                                                            \
    }                                                                          \
  }
#define ASSERT(condition)                                                      \
  {                                                                            \
    if (!bool(condition)) {                                                    \
      std::cerr << __FILE__ << ":" << __LINE__ << " assertion `" << #condition \
                << "' failed\n";                                               \
      std::abort();                                                            \
    }                                                                          \
  }

#endif

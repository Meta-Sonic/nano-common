/*
 * nano library
 *
 * Copyright (C) 2022, Meta-Sonic
 * All rights reserved.
 *
 * Proprietary and confidential.
 * Any unauthorized copying, alteration, distribution, transmission,
 * performance, display or other use of this material is strictly prohibited.
 *
 * Written by Alexandre Arsenault <alx.arsenault@gmail.com>
 */

#pragma once

/*!
 * @file      nano/common.h
 * @brief     nano common
 * @copyright Copyright (C) 2022, Meta-Sonic
 * @author    Alexandre Arsenault alx.arsenault@gmail.com
 * @date      Created 25/07/2022
 */

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>

//
// MARK: - Macros -
//

#ifdef _MSVC_LANG
#define NANO_CPP_VERSION _MSVC_LANG
#else
#define NANO_CPP_VERSION __cplusplus
#endif

#if NANO_CPP_VERSION >= 202002L
#define NANO_CPP_20
#define NANO_IF_CPP_20(X) X
#else
#define NANO_IF_CPP_20(X)
#endif

#if NANO_CPP_VERSION >= 201703L
#define NANO_CPP_17
#define NANO_IF_CPP_17(X) X
#else
#define NANO_IF_CPP_17(X)
#endif

/// @def NANO_INLINE
#define NANO_INLINE inline

/// @def NANO_CXPR
#define NANO_CXPR constexpr

/// @def NANO_NOEXCEPT
#define NANO_NOEXCEPT noexcept

/// @def NANO_NODISCARD
#define NANO_NODISCARD [[nodiscard]]

/// @def NANO_INLINE_CXPR
#define NANO_INLINE_CXPR NANO_INLINE NANO_CXPR

/// @def NANO_NODC_INLINE
#define NANO_NODC_INLINE NANO_NODISCARD inline

/// @def NANO_NODC_INLINE_CXPR
#define NANO_NODC_INLINE_CXPR NANO_NODISCARD NANO_INLINE_CXPR

/// @def NANO_UNUSED
#define NANO_UNUSED(x) (void)x

/// @def NANO_LIKELY
#define NANO_LIKELY(EXPR) NANO_LIKELY_IMPL(EXPR)

/// @def NANO_UNLIKELY
#define NANO_UNLIKELY(EXPR) NANO_UNLIKELY_IMPL(EXPR)

/// @def NANO_DEBUGTRAP
/// On compilers which support it, expands to an expression which causes the
/// program to break while running under a debugger.
#define NANO_DEBUGTRAP() NANO_DEBUGTRAP_IMPL()

/// @def NANO_ASSERT
#define NANO_ASSERT(Expr, Msg) NANO_ASSERT_IMPL(Expr, Msg)

/// @def NANO_ERROR
#define NANO_ERROR(Msg) NANO_ERROR_IMPL(Msg)

/// @def NANO_STRINGIFY
#define NANO_STRINGIFY(X) NANO_STR(X)

/// @def NANO_STR
#define NANO_STR(X) #X

//
// MARK: Platform type.
//

// Android.
#if defined(__ANDROID__)
#define NANO_ANDROID 1

// Linux.
#elif defined(__linux__) || defined(__linux) || defined(linux)
#define NANO_LINUX 1

// Apple macos or ios.
#elif defined(__APPLE__)
// Apple.
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#define NANO_IOS 1

#elif TARGET_OS_MAC
#define NANO_MACOS 1

#else
#warning "nano unsupported apple platform."
#endif

// BSD.
#elif defined(BSD) || defined(__DragonFly__) || defined(__FreeBSD__) ||        \
    defined(__NetBSD__) || defined(__OpenBSD__)
#define NANO_BSD 1

// Solaris.
#elif defined(__sun) && defined(__SVR4)
#define NANO_SOLARIS 1

// Windows.
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) ||               \
    defined(__NT__) || defined(_WIN64)
#define NANO_WINDOWS 1

// Unknown platform.
#else
#warning "nano unsupported platform."
#endif

//
// MSVC
//

#ifdef _MSC_VER
#define NANO_MSVC_PRAGMA(X) __pragma(X)
#else
#define NANO_MSVC_PRAGMA(X)
#endif

#define NANO_MSVC_DIAGNOSTIC_PUSH() NANO_MSVC_PRAGMA(warning(push))
#define NANO_MSVC_DIAGNOSTIC_POP() NANO_MSVC_PRAGMA(warning(pop))
#define NANO_MSVC_DIAGNOSTIC(X) NANO_MSVC_PRAGMA(warning(disable : X))

#define NANO_MSVC_PUSH_WARNING(X)                                              \
  NANO_MSVC_DIAGNOSTIC_PUSH() NANO_MSVC_PRAGMA(warning(disable : X))
#define NANO_MSVC_POP_WARNING() NANO_MSVC_DIAGNOSTIC_POP()

//
// clang
//

#ifdef __clang__
#define NANO_CLANG_PRAGMA(X) _Pragma(X)
#else
#define NANO_CLANG_PRAGMA(X)
#endif

#define NANO_CLANG_DIAGNOSTIC_PUSH() NANO_CLANG_PRAGMA("clang diagnostic push")
#define NANO_CLANG_DIAGNOSTIC_POP() NANO_CLANG_PRAGMA("clang diagnostic pop")
#define NANO_CLANG_DIAGNOSTIC(TYPE, X)                                         \
  NANO_CLANG_PRAGMA(NANO_STRINGIFY(clang diagnostic TYPE X))

#define NANO_CLANG_PUSH_WARNING(X)                                             \
  NANO_CLANG_DIAGNOSTIC_PUSH()                                                 \
  NANO_CLANG_PRAGMA(NANO_STRINGIFY(clang diagnostic ignored X))

#define NANO_CLANG_POP_WARNING() NANO_CLANG_DIAGNOSTIC_POP()

/// @def NANO_USING_TYPE
#define NANO_USING_TYPE(x) template <class T> using x = decltype(T::x)

/// @def NANO_ENUM_CLASS_FLAGS
#define NANO_ENUM_CLASS_FLAGS(enum_class)                                      \
  inline enum_class operator|(enum_class lhs, enum_class rhs) {                \
    using type = std::underlying_type_t<enum_class>;                           \
    return static_cast<enum_class>(static_cast<type>(lhs) |                    \
                                   static_cast<type>(rhs));                    \
  }                                                                            \
                                                                               \
  inline enum_class operator&(enum_class lhs, enum_class rhs) {                \
    using type = std::underlying_type_t<enum_class>;                           \
    return static_cast<enum_class>(static_cast<type>(lhs) &                    \
                                   static_cast<type>(rhs));                    \
  }                                                                            \
                                                                               \
  inline enum_class &operator|=(enum_class &lhs, enum_class rhs) {             \
    return lhs = (lhs | rhs);                                                  \
  }                                                                            \
  inline enum_class &operator&=(enum_class &lhs, enum_class rhs) {             \
    return lhs = (lhs & rhs);                                                  \
  }                                                                            \
                                                                               \
  inline enum_class operator~(const enum_class &lhs) {                         \
    using type = std::underlying_type_t<enum_class>;                           \
    return static_cast<enum_class>(~type(lhs));                                \
  }                                                                            \
                                                                               \
  inline bool operator==(enum_class lhs,                                       \
                         std::underlying_type_t<enum_class> value) {           \
    return std::underlying_type_t<enum_class>(lhs) == value;                   \
  }                                                                            \
                                                                               \
  inline bool operator!=(enum_class lhs,                                       \
                         std::underlying_type_t<enum_class> value) {           \
    return std::underlying_type_t<enum_class>(lhs) != value;                   \
  }

// MARK: - Macros -

#ifndef __has_feature
#define __has_feature(x) 0
#endif

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin_expect)
#define NANO_LIKELY_IMPL(EXPR) __builtin_expect(static_cast<bool>(EXPR), true)
#define NANO_UNLIKELY_IMPL(EXPR)                                               \
  __builtin_expect(static_cast<bool>(EXPR), false)
#else
#define NANO_LIKELY_IMPL(EXPR) (EXPR)
#define NANO_UNLIKELY_IMPL(EXPR) (EXPR)
#endif

#if __has_builtin(__builtin_debugtrap)
#define NANO_DEBUGTRAP_IMPL() __builtin_debugtrap()

#elif defined(_WIN32)
// The __debugbreak intrinsic is supported by MSVC and breaks while
// running under the debugger, and also supports invoking a debugger
// when the OS is configured appropriately.
#define NANO_DEBUGTRAP_IMPL() __debugbreak()

#elif defined(__clang__) && (defined(unix) || defined(__unix__) ||             \
                             defined(__unix) || defined(__MACH__))
#include <signal.h>
#define NANO_DEBUGTRAP_IMPL() raise(SIGTRAP)

#else
#define NANO_DEBUGTRAP_IMPL() std::abort()
#endif

//
// MARK: - assert -
//

#ifdef NDEBUG
#define NANO_ASSERT_IMPL(Expr, Msg)
#define NANO_ERROR_IMPL(Msg)

#else
#define NANO_ASSERT_IMPL(Expr, Msg)                                            \
  nano::assert_detail::custom_assert(#Expr, (Expr), __FILE__, __LINE__, Msg)
#define NANO_ERROR_IMPL(Msg)                                                   \
  nano::assert_detail::custom_error(__FILE__, __LINE__, Msg)

namespace nano::assert_detail {
NANO_INLINE void custom_assert(const char *expr_str, bool expr,
                               const char *file, int line, const char *msg) {
  if (NANO_LIKELY(expr)) {
    return;
  }

  std::cerr << "Assert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";

  NANO_DEBUGTRAP();
}

NANO_INLINE void custom_error(const char *file, int line, const char *msg) {
  std::cerr << "Assert failed:\t" << msg << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";

  NANO_DEBUGTRAP();
}
} // namespace nano::assert_detail.
#endif // NDEBUG.

namespace nano {
template <typename _FlagType = int>
inline constexpr bool has_flag(_FlagType flags_to_verity,
                               _FlagType flags) noexcept {
  return ((flags & flags_to_verity) != 0);
}

template <typename _FlagType>
inline constexpr void set_flag(_FlagType flag_to_set, bool value,
                               _FlagType &flags) noexcept {
  flags = value ? static_cast<_FlagType>(flags | flag_to_set)
                : static_cast<_FlagType>(flags & ~flag_to_set);
  //  if (value) {
  //    flags = static_cast<_FlagType>(flags | flag_to_set);
  //    return;
  //  }
  //
  //  flags = static_cast<_FlagType>(flags & ~flag_to_set);
}

//
// MARK: - Traits -
//

///
struct nonesuch {
  nonesuch(const nonesuch &) = delete;
  ~nonesuch() = delete;
  void operator=(const nonesuch &) = delete;
};

template <class Default, class AlwaysVoid, template <class...> class Op,
          class... Args>
struct detector_value : std::false_type {};

///
template <class Default, template <class...> class Op, class... Args>
struct detector_value<Default, std::void_t<Op<Args...>>, Op, Args...>
    : std::true_type {};

template <class T, template <class...> class... Ops>
using has_members = std::conjunction<detector_value<nonesuch, void, Ops, T>...>;

template <class T, template <class...> class... Ops>
using enable_if_has_members_t =
    std::enable_if_t<has_members<T, Ops...>::value, std::nullptr_t>;

template <class T, template <class...> class... Ops>
using if_members = enable_if_has_members_t<T, Ops...>;

template <typename T1, typename T2,
          std::enable_if_t<std::is_floating_point_v<std::common_type_t<T1, T2>>,
                           int> = 0>
NANO_NODC_INLINE_CXPR bool fcompare(T1 a, T2 b) NANO_NOEXCEPT {
  using ftype = std::common_type_t<T1, T2>;
  const ftype fa = static_cast<ftype>(a);
  const ftype fb = static_cast<ftype>(b);
  const ftype t = static_cast<ftype>(std::numeric_limits<ftype>::epsilon());
  const ftype dt = std::abs(fa - fb);
  return dt <= t || dt < std::max(std::abs(fa), std::abs(fb)) * t;

  //  using T = std::common_type_t<T1, T2>;
  //  return std::abs(static_cast<T>(a) - static_cast<T>(b)) <=
  //  std::numeric_limits<T>::epsilon();
}

namespace cxpr {
template <typename Tp, typename = std::enable_if_t<std::is_arithmetic_v<Tp>>>
inline constexpr Tp abs(Tp n) {
  if constexpr (std::is_signed_v<Tp>) {
    return n >= 0 ? n : -n;
  } else {
    return n;
  }
}
} // namespace cxpr.
} // namespace nano.

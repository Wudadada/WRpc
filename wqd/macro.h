#ifndef __WQD_MACRO_H__
#define __WQD_MACRO_H__

#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#   define WQD_LIKELY(x)       __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#   define WQD_UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#   define WQD_LIKELY(x)      (x)
#   define WQD_UNLIKELY(x)      (x)
#endif

/// 断言宏封装
#define WQD_ASSERT(x) \
    if(WQD_UNLIKELY(!(x))) { \
        WQD_LOG_ERROR(WQD_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << wqd::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

/// 断言宏封装
#define WQD_ASSERT2(x, w) \
    if(WQD_UNLIKELY(!(x))) { \
        WQD_LOG_ERROR(WQD_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << wqd::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#endif

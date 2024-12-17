#pragma once

#ifdef _WIN32
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN 4321
#define __BYTE_ORDER __LITTLE_ENDIAN  // Windows is always little endian
#elif defined(__linux__)
#include <features.h>
#include <endian.h>
#elif defined(__APPLE__)
#include <machine/endian.h>
#define __BYTE_ORDER BYTE_ORDER
#define __BIG_ENDIAN BIG_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#endif

#ifdef __GLIBC__
#include <ieee754.h>
#else
union ieee754_double {
        double d;
        struct {
#if __BYTE_ORDER == __BIG_ENDIAN
                unsigned int negative : 1;
                unsigned int exponent : 11;
                unsigned int mantissa0 : 20;
                unsigned int mantissa1 : 32;
#else  // Little endian
                unsigned int mantissa1 : 32;
                unsigned int mantissa0 : 20;
                unsigned int exponent : 11;
                unsigned int negative : 1;
#endif
        } ieee;
};
#endif

#include <limits>
static_assert(std::numeric_limits<double>::is_iec559);
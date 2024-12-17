#include "network/NetworkDouble.hpp"
#include "network/ieee754.h"

namespace tetriq {
    NetworkDouble::NetworkDouble(double value)
    {
        ieee754_double network{.d = value};
        mantissa1 = network.ieee.mantissa1;
        mantissa0 = network.ieee.mantissa0;
        exponent = network.ieee.exponent;
        negative = network.ieee.negative;
    }

    double NetworkDouble::toHostDouble() const
    {
        ieee754_double host{.ieee = {
                                .mantissa1 = mantissa1,
                                .mantissa0 = mantissa0,
                                .exponent = exponent,
                                .negative = negative,
                            }};
        return host.d;
    }
} // namespace tetriq

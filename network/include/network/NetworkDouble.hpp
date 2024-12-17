#pragma once

namespace tetriq {
    /* ieee754 little endian double type */
    struct NetworkDouble {
            /* Together these comprise the mantissa.  */
            unsigned int mantissa1:32;
            unsigned int mantissa0:20;
            unsigned int exponent:11;
            unsigned int negative:1;

            NetworkDouble(double value);
            double toHostDouble() const;
    };
}

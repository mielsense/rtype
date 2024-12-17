#include "network/NetworkDouble.hpp"

#include <cmath>
#include <gtest/gtest.h>

TEST(network_double, conversion1)
{
    tetriq::NetworkDouble d{M_PI};
    double pi = d.toHostDouble();

    ASSERT_EQ(pi, M_PI);
}

TEST(network_double, conversion2)
{
    tetriq::NetworkDouble d{M_E};
    double pi = d.toHostDouble();

    ASSERT_EQ(pi, M_E);
}

TEST(network_double, conversion3)
{
    tetriq::NetworkDouble d{-M_E};
    double pi = d.toHostDouble();

    ASSERT_EQ(pi, -M_E);
}

TEST(network_double, conversion4)
{
    tetriq::NetworkDouble d{-456674324432333234.0};
    double pi = d.toHostDouble();

    ASSERT_EQ(pi, -456674324432333234.0);
}

TEST(network_double, size)
{
    ASSERT_EQ(sizeof(tetriq::NetworkDouble), sizeof(double));
}

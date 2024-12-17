#include "network/NetworkStream.hpp"
#include "network/packets/PingPacket.hpp"

#include <gtest/gtest.h>

TEST(network_packets_stream, PingPacket)
{
    std::time_t time;
    std::time(&time);
    tetriq::PingPacket packet{time};
    tetriq::NetworkOStream os{packet.getNetworkSize()};
    os << packet;
    tetriq::NetworkIStream is{&os};
    tetriq::PingPacket received{is};
    ASSERT_EQ(packet.time, received.time);
}

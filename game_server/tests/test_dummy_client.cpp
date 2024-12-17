#include "DummyClient.hpp"
#include "network/packets/PingPacket.hpp"
#include "network/transport/TransportLayerFactory.hpp"

#include <enet/enet.h>
#include <gtest/gtest.h>
#include <unistd.h>

#define SERVER_IP "localhost"
#define SERVER_PORT 4242

TEST(DummyClientTest, PingPacket)
{
    std::unique_ptr<tetriq::ITransportLayer> transport =
        tetriq::TransportLayerFactory::createClient(SERVER_IP, SERVER_PORT, "DummyClient");
    DummyClient client{transport};

    const tetriq::PingPacket packet{time(nullptr)};

    while (!client.isConnected()) {
        client.loop();
    }

    client.sendPacket(packet);
    client.assertReceivePacket(tetriq::PacketId::PING);
    
    while (!client.assertionsCompleted()) {
        client.loop();
        usleep(100000);
        // TODO : timeout
    }

    SUCCEED();
}

TEST(DummyClientTest, ServerListPacket)
{
    std::unique_ptr<tetriq::ITransportLayer> transport =
        tetriq::TransportLayerFactory::createClient(SERVER_IP, SERVER_PORT, "DummyClient");
    DummyClient client{transport};

    const tetriq::ServerListRequestPacket packet;

    while (!client.isConnected()) {
        client.loop();
    }

    client.sendPacket(packet);
    client.assertReceivePacket(tetriq::PacketId::S_SERVER_LIST_RESPONSE);

    while (!client.assertionsCompleted()) {
        client.loop();
        usleep(100000);
        // TODO : timeout
    }

    SUCCEED();
}
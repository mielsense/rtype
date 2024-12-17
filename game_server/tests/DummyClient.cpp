/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DummyClient.cpp
*/

#include <gtest/gtest.h>

#include "DummyClient.hpp"

DummyClient::DummyClient(std::unique_ptr<tetriq::ITransportLayer> &layer) : AClient(layer)
{}

void DummyClient::onDisconnection(tetriq::DisconnectionEvent &)
{
    FAIL() << "Disconnected from server";
}

void DummyClient::sendPacket(const tetriq::APacket &packet)
{
    if (!_connected)
        FAIL() << "Packet sent before connection";
    _transport->sendPacket(packet, _transport->getPeer(_server_id));
}

void DummyClient::assertReceivePacket(const tetriq::PacketId type)
{
    _assertions.push(type);
}

bool DummyClient::assertionsCompleted() const
{
    return _assertions.empty();
}

bool DummyClient::handlePacket(const tetriq::APacket &packet, tetriq::ITransportPeer &)
{
    tetriq::PacketId assertion = _assertions.front();
    tetriq::PacketId id = packet.getId();
    _assertions.pop();

    EXPECT_EQ(assertion, id);
    return true;
}

//bool DummyClient::handleServerListResponsePacket(const tetriq::ServerListResponsePacket &server_list_response_packet,
//    tetriq::ITransportPeer &transport_peer)
//{
//    return true;
//}

// bool DummyClient::handlePacket(const tetriq::APacket &packet)
//{
//
// }

// bool DummyClient::handlePingPacket(const tetriq::PingPacket &)
//{
//     LOG_INFO("Received a ping packet");
//     return true;
// }

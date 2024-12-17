/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** DummyClient.hpp
*/

#pragma once

#include "network/AClient.hpp"
#include "network/PacketId.hpp"

class DummyClient final : public tetriq::AClient
{
public:
    DummyClient(std::unique_ptr<tetriq::ITransportLayer>& layer);

    void sendPacket(const tetriq::APacket& packet);
    void assertReceivePacket(const tetriq::PacketId type);
    bool assertionsCompleted() const;

    void onDisconnection(tetriq::DisconnectionEvent& event) override;
    bool handlePacket(const tetriq::APacket& packet, tetriq::ITransportPeer& peer) override;
    //bool handleServerListResponsePacket(const tetriq::ServerListResponsePacket&, tetriq::ITransportPeer&) override;
    // bool handlePingPacket(const tetriq::PingPacket &) override;
private:
    std::queue<tetriq::PacketId> _assertions;
};

#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include "network/NetworkObject.hpp"

namespace tetriq
{
    using Position = std::array<double, 2>;
    using Velocity = std::array<double, 2>;
    using Color = std::array<uint8_t, 3>;

    class Player : public NetworkObject
    {
    public:
        Player() = default;
        Player(const std::string& name, Color color, Position position);
        size_t getNetworkSize() const override;

        std::string name;
        std::array<uint8_t, 3> color;
        Position position;

    protected:
        NetworkOStream& write(NetworkOStream& os) const override;
        NetworkIStream& read(NetworkIStream& is) override;
    };

    class ServerInfo final : public NetworkObject
    {
    public:
        ServerInfo() = default;
        ServerInfo(const std::string& name, const std::string& address, uint16_t port, uint64_t player_count,
                   uint64_t max_players);
        size_t getNetworkSize() const override;

        std::string name;
        std::string address;
        uint16_t port{};
        uint64_t player_count{};
        uint64_t max_players{};

    protected:
        NetworkOStream& write(NetworkOStream& os) const override;
        NetworkIStream& read(NetworkIStream& is) override;
    };
} // namespace tetriq

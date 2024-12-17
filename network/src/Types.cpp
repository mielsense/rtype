#include "network/Types.hpp"

namespace tetriq {
    Player::Player(const std::string &name, Color color, Position position) :
        name(name)
        , color(color)
        , position(position)
    {
    }

    size_t Player::getNetworkSize() const
    {
        return NetworkObject::getNetworkSize(name)
            + NetworkObject::getNetworkSize(color)
            + NetworkObject::getNetworkSize(position);
    }

    NetworkOStream &Player::write(NetworkOStream &os) const
    {
        os << name;
        std::array c{color};
        os << c;
        os << position;
        return os;
    }

    NetworkIStream &Player::read(NetworkIStream &is)
    {
        return is >> name >> color >> position;
    }

    ServerInfo::ServerInfo(const std::string &name, const std::string &address, uint16_t port, uint64_t player_count,
                           uint64_t max_players) :
        name(name), address(address), port(port), player_count(player_count), max_players(max_players)
    {
    }

    size_t ServerInfo::getNetworkSize() const
    {
        return NetworkObject::getNetworkSize(name)
            + NetworkObject::getNetworkSize(address)
            + NetworkObject::getNetworkSize(port)
            + NetworkObject::getNetworkSize(player_count)
            + NetworkObject::getNetworkSize(max_players);
    }

    NetworkOStream &ServerInfo::write(NetworkOStream &os) const
    {
        return os << name << address << port << player_count << max_players;
    }

    NetworkIStream &ServerInfo::read(NetworkIStream &is)
    {
        return is >> name >> address >> port >> player_count >> max_players;
    }
}

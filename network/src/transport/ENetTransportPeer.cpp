#include "network/transport/ENetTransportPeer.hpp"

namespace tetriq {
    ENetTransportPeer::ENetTransportPeer(ENetPeer *peer, uint64_t id)
        : _id(id)
        , _peer(peer, &enet_peer_reset)
    {}

    uint64_t ENetTransportPeer::getId() const
    {
        return _id;
    }

    ENetPeer *ENetTransportPeer::getPeer()
    {
        return _peer.get();
    }
}

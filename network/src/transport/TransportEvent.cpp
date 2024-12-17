#include "network/transport/TransportEvent.hpp"

namespace tetriq {
    TransportEvent::TransportEvent(Type type)
        : type(type)
    {}
    
    ConnectionEvent::ConnectionEvent(ITransportPeer &peer)
        : TransportEvent(Type::CONNECTION)
        , peer(peer)
    {}

    DisconnectionEvent::DisconnectionEvent(uint64_t peer_id)
        : TransportEvent(Type::DISCONNECTION)
        , peer_id(peer_id)
    {}

    PacketEvent::PacketEvent(ITransportPeer &peer, std::unique_ptr<APacket> &packet_ref)
        : TransportEvent(Type::PACKET)
        , peer(peer)
        , packet(std::move(packet_ref))
    {}
}

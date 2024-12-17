#pragma once

#include <memory>
#include "network/APacket.hpp"
#include "network/transport/ITransportPeer.hpp"

namespace tetriq {    
    class TransportEvent {
        public:
            enum class Type {
                CONNECTION,
                DISCONNECTION,
                PACKET,
            };
            TransportEvent(Type type);
            virtual ~TransportEvent() = default;

            Type type;
    };

    struct ConnectionEvent : public TransportEvent {
            ConnectionEvent(ITransportPeer &peer);
            ITransportPeer &peer;
    };
    struct DisconnectionEvent : public TransportEvent {
            DisconnectionEvent(uint64_t peer_id);
            uint64_t peer_id;
    };
    struct PacketEvent : public TransportEvent {
            PacketEvent(ITransportPeer &peer, std::unique_ptr<APacket> &packet);
            ITransportPeer &peer;
            std::unique_ptr<APacket> packet;
    };
}

#pragma once

#include <queue>
#include "network/APacket.hpp"
#include "network/transport/ITransportPeer.hpp"
#include "network/transport/TransportEvent.hpp"

namespace tetriq {
    class ITransportLayer {
        public:
            virtual ~ITransportLayer() = default;

            virtual void sendPacket(const APacket &packet, ITransportPeer &peer) = 0;
            /**
             * Should be called often to keep the connection alive and do the
             * background transfer work.
             */
            virtual void update() = 0;
            virtual std::queue<std::unique_ptr<TransportEvent>> &getEvents() = 0;
            virtual ITransportPeer &getPeer(uint64_t id) = 0;
            /**
             * Used for clients to connect to servers
             */
            virtual void connect(const std::string &ip, uint16_t port) = 0;
            virtual void kickPeer(uint64_t id) = 0;
            virtual std::string getServerIp() = 0;
            virtual uint16_t getServerPort() = 0;
    };
}

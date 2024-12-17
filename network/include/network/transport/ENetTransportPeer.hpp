#pragma once

#include <enet/enet.h>
#include <memory>

#include "network/transport/ITransportPeer.hpp"

namespace tetriq {
    class ENetTransportPeer : public ITransportPeer {
        public:
            ENetTransportPeer(ENetPeer *peer, uint64_t id);
            ~ENetTransportPeer() override = default;

            uint64_t getId() const override;
            ENetPeer *getPeer();
        private:
            using _Peer = std::unique_ptr<ENetPeer, decltype(&enet_peer_reset)>;

            uint64_t _id;
            _Peer _peer;
    };
}

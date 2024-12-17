#pragma once

#include <cstdint>

namespace tetriq {
    class ITransportPeer {
        public:
            virtual ~ITransportPeer() = default;

            virtual uint64_t getId() const = 0;
    };
}

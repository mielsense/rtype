// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/NetworkObject.hpp"
#include "network/PacketId.hpp"

#include <enet/enet.h>
#include <memory>

namespace tetriq {
    class APacket : public NetworkObject {
        public:
            virtual PacketId getId() const = 0;
    };

    NetworkIStream &operator>>(NetworkIStream &is, std::unique_ptr<APacket> &dest);
}

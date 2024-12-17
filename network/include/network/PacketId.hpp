// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>

namespace tetriq {
    /**
     * Packets starting with S_ are sent from server to client and packets
     * starting with C_ are sent from client to server. Others can be sent
     * either way.
     */
    enum class PacketId : uint64_t {
        #include "generated/packet-id-list"
    };
}

// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "APacket.hpp"
#include "network/packets/DestroyEntityPacket.hpp"
#include "network/packets/EntityMovementPacket.hpp"
#include "network/packets/FullSynchronisationPacket.hpp"
#include "network/packets/MovementActionPacket.hpp"
#include "network/packets/NewEnemyPacket.hpp"
#include "network/packets/NewPlayerPacket.hpp"
#include "network/packets/NewShootPacket.hpp"
#include "network/packets/SetHealthPacket.hpp"
#include "network/packets/ShootActionPacket.hpp"
#include "network/transport/ITransportPeer.hpp"
#include "packets/PingPacket.hpp"
#include "packets/QueryRequestPacket.hpp"
#include "packets/QueryResponsePacket.hpp"
#include "packets/ServerListRequestPacket.hpp"
#include "packets/ServerListResponsePacket.hpp"
#include "packets/PlayerNamePacket.hpp"

#include <enet/enet.h>

#include "network/packets/EndGamePacket.hpp"
#include "network/packets/SetScorePacket.hpp"
#include "packets/StartGamePacket.hpp"

namespace tetriq {
    class PacketHandler {
        public:
            virtual ~PacketHandler() = default;

            virtual bool handlePacket(const APacket &packet, ITransportPeer &client);

            virtual bool handlePingPacket(const PingPacket &, ITransportPeer &);
            virtual bool handleQueryRequestPacket(const QueryRequestPacket &, ITransportPeer &);
            virtual bool handleQueryResponsePacket(const QueryResponsePacket &, ITransportPeer &);
            virtual bool handleNewPlayerPacket(const NewPlayerPacket &, ITransportPeer &);
            virtual bool handleNewEnemyPacket(const NewEnemyPacket &, ITransportPeer &);
            virtual bool handleNewShootPacket(const NewShootPacket &, ITransportPeer &);
            virtual bool handleSetHealthPacket(const SetHealthPacket &, ITransportPeer &);
            virtual bool handleDestroyEntityPacket(const DestroyEntityPacket &, ITransportPeer &);
            virtual bool handleFullSynchronisationPacket(const FullSynchronisationPacket &, ITransportPeer &);
            virtual bool handleMovementActionPacket(const MovementActionPacket &, tetriq::ITransportPeer &);
            virtual bool handleShootActionPacket(const ShootActionPacket &, tetriq::ITransportPeer &);
            virtual bool handleEntityMovementPacket(const EntityMovementPacket &, tetriq::ITransportPeer &);
            virtual bool handleServerListRequestPacket(const ServerListRequestPacket &, ITransportPeer &);
            virtual bool handleServerListResponsePacket(const ServerListResponsePacket &, ITransportPeer &);
            virtual bool handleStartGamePacket(const StartGamePacket &, ITransportPeer &);
            virtual bool handleSetScorePacket(const SetScorePacket &, ITransportPeer &);
            virtual bool handlePlayerNamePacket(const PlayerNamePacket &, ITransportPeer &);
            virtual bool handleEndGamePacket(const EndGamePacket &, ITransportPeer &);
    };
} // namespace tetriq

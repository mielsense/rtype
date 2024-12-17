// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/PacketHandler.hpp"

#include <network/packets/StartGamePacket.hpp>

#include "Logging.hpp"
#include "network/APacket.hpp"
#include "network/packets/SetHealthPacket.hpp"

namespace tetriq {
    bool PacketHandler::handlePacket(const APacket &packet, ITransportPeer &client)
    {
        switch (packet.getId()) {
        case PacketId::PING:
            return handlePingPacket(dynamic_cast<const PingPacket &>(packet), client);
        case PacketId::S_QUERY_REQUEST:
            return handleQueryRequestPacket(dynamic_cast<const QueryRequestPacket &>(packet), client);
        case PacketId::C_QUERY_RESPONSE:
            return handleQueryResponsePacket(dynamic_cast<const QueryResponsePacket &>(packet), client);
        case PacketId::NEW_PLAYER:
            return handleNewPlayerPacket(dynamic_cast<const NewPlayerPacket &>(packet), client);
        case PacketId::NEW_ENEMY:
            return handleNewEnemyPacket(dynamic_cast<const NewEnemyPacket &>(packet), client);
        case PacketId::NEW_SHOOT:
            return handleNewShootPacket(dynamic_cast<const NewShootPacket &>(packet), client);
        case PacketId::SET_HEALTH:
            return handleSetHealthPacket(dynamic_cast<const SetHealthPacket &>(packet), client);
        case PacketId::DESTROY_ENTITY:
            return handleDestroyEntityPacket(dynamic_cast<const DestroyEntityPacket &>(packet), client);
        case PacketId::FULL_SYNCHRONISATION:
            return handleFullSynchronisationPacket(dynamic_cast<const FullSynchronisationPacket &>(packet), client);
        case PacketId::MOVEMENT_ACTION:
            return handleMovementActionPacket(dynamic_cast<const MovementActionPacket &>(packet), client);
        case PacketId::SHOOT_ACTION:
            return handleShootActionPacket(dynamic_cast<const ShootActionPacket &>(packet), client);
        case PacketId::ENTITY_MOVEMENT:
            return handleEntityMovementPacket(dynamic_cast<const EntityMovementPacket &>(packet), client);
        case PacketId::C_SERVER_LIST_REQUEST:
            return handleServerListRequestPacket(dynamic_cast<const ServerListRequestPacket &>(packet), client);
        case PacketId::S_SERVER_LIST_RESPONSE:
            return handleServerListResponsePacket(dynamic_cast<const ServerListResponsePacket &>(packet), client);
        case PacketId::S_START_GAME:
            return handleStartGamePacket(dynamic_cast<const StartGamePacket &>(packet), client);
        case PacketId::SET_SCORE:
                return handleSetScorePacket(dynamic_cast<const SetScorePacket &>(packet), client);
        case PacketId::PLAYER_NAME:
            return handlePlayerNamePacket(dynamic_cast<const PlayerNamePacket &>(packet), client);
        case PacketId::END_GAME:
            return handleEndGamePacket(dynamic_cast<const EndGamePacket &>(packet), client);
        default:
            auto id = tetriq::to_underlying(packet.getId());
            LOG_WARNING("Unhandled packet of type ", id);
            return false;
        }
    }

    bool PacketHandler::handlePingPacket(const PingPacket &, ITransportPeer &peer)
    {
        return false;
    }

    bool PacketHandler::handleQueryRequestPacket(const QueryRequestPacket &, ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleQueryResponsePacket(const QueryResponsePacket &, ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleNewPlayerPacket(const NewPlayerPacket &, ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleNewEnemyPacket(const NewEnemyPacket &, ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleNewShootPacket(const NewShootPacket &, ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleSetHealthPacket(const SetHealthPacket &, ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleDestroyEntityPacket(const DestroyEntityPacket &, ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleFullSynchronisationPacket(const FullSynchronisationPacket &, ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleMovementActionPacket(const MovementActionPacket &, tetriq::ITransportPeer &peer)
    {
        return false;
    }

    bool PacketHandler::handleShootActionPacket(const ShootActionPacket &, tetriq::ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleEntityMovementPacket(const EntityMovementPacket &, tetriq::ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleServerListRequestPacket(const ServerListRequestPacket &, ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleServerListResponsePacket(const ServerListResponsePacket &, ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handleStartGamePacket(const StartGamePacket &, ITransportPeer &)
    {
        return false;
    }
    bool PacketHandler::handleSetScorePacket(const SetScorePacket &, ITransportPeer &)
    {
        return false;
    }

    bool PacketHandler::handlePlayerNamePacket(const PlayerNamePacket &, ITransportPeer &)
    {
        return false;
    }
    bool PacketHandler::handleEndGamePacket(const EndGamePacket &, ITransportPeer &)
    {
        return false;
    }

}

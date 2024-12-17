// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/APacket.hpp"
#include "Logging.hpp"
#include "network/packets/DestroyEntityPacket.hpp"
#include "network/packets/EndGamePacket.hpp"
#include "network/packets/EntityMovementPacket.hpp"
#include "network/packets/FullSynchronisationPacket.hpp"
#include "network/packets/MovementActionPacket.hpp"
#include "network/packets/NewEnemyPacket.hpp"
#include "network/packets/NewPlayerPacket.hpp"
#include "network/packets/NewShootPacket.hpp"
#include "network/packets/PingPacket.hpp"
#include "network/packets/QueryRequestPacket.hpp"
#include "network/packets/QueryResponsePacket.hpp"
#include "network/packets/SetHealthPacket.hpp"
#include "network/packets/SetScorePacket.hpp"
#include "network/packets/ShootActionPacket.hpp"

#include <memory>
#include <network/packets/PlayerNamePacket.hpp>
#include <network/packets/ServerListRequestPacket.hpp>
#include <network/packets/ServerListResponsePacket.hpp>
#include <network/packets/StartGamePacket.hpp>
#ifdef THROW_ERROR
#undef THROW_ERROR  // Not recommended unless you're sure you won't use Windows GDI
#endif
namespace tetriq {
    NetworkIStream &operator>>(NetworkIStream &is, std::unique_ptr<APacket> &dest)
    {
        PacketId id;
        is >> id;
        switch (id) {
            case PacketId::PING:
                dest = std::make_unique<PingPacket>(is);
                break;
            case PacketId::S_QUERY_REQUEST:
                dest = std::make_unique<QueryRequestPacket>(is);
                break;
            case PacketId::C_QUERY_RESPONSE:
                dest = std::make_unique<QueryResponsePacket>(is);
                break;
            case PacketId::FULL_SYNCHRONISATION:
                dest = std::make_unique<FullSynchronisationPacket>(is);
                break;
            case PacketId::NEW_PLAYER:
                dest = std::make_unique<NewPlayerPacket>(is);
                break;
            case PacketId::NEW_ENEMY:
                dest = std::make_unique<NewEnemyPacket>(is);
                break;
            case PacketId::NEW_SHOOT:
                dest = std::make_unique<NewShootPacket>(is);
                break;
            case PacketId::SET_HEALTH:
                dest = std::make_unique<SetHealthPacket>(is);
                break;
            case PacketId::DESTROY_ENTITY:
                dest = std::make_unique<DestroyEntityPacket>(is);
                break;
            case PacketId::MOVEMENT_ACTION:
                dest = std::make_unique<MovementActionPacket>(is);
                break;
            case PacketId::SHOOT_ACTION:
                dest = std::make_unique<ShootActionPacket>(is);
                break;
            case PacketId::ENTITY_MOVEMENT:
                dest = std::make_unique<EntityMovementPacket>(is);
                break;
            case PacketId::C_SERVER_LIST_REQUEST:
                dest = std::make_unique<ServerListRequestPacket>(is);
                break;
            case PacketId::S_SERVER_LIST_RESPONSE:
                dest = std::make_unique<ServerListResponsePacket>(is);
                break;
            case PacketId::S_START_GAME:
                dest = std::make_unique<StartGamePacket>(is);
                break;
            case PacketId::PLAYER_NAME:
                dest = std::make_unique<PlayerNamePacket>(is);
                break;
            case PacketId::SET_SCORE:
                dest = std::make_unique<SetScorePacket>(is);
                break;
            case PacketId::END_GAME:
                dest = std::make_unique<EndGamePacket>(is);
                break;
            default:
                LOG_ERROR("received packet of unknown id {}", (uint64_t) id);
                dest.reset(nullptr);
                break;
        }
        return is;
    }
} // namespace tetriq

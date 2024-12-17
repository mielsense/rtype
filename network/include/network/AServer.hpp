/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** server
*/

#pragma once

#include <chrono>

#include "network/PacketHandler.hpp"
#include "network/transport/ITransportLayer.hpp"

namespace tetriq {
    class AServer : public tetriq::PacketHandler {
        public:
            AServer(std::unique_ptr<tetriq::ITransportLayer> &layer);
            virtual ~AServer() = default;

            void loop(float delta_time);

            virtual void onTick(float delta_time);
            virtual void onConnection(tetriq::ConnectionEvent &event);
            virtual void onDisconnection(tetriq::DisconnectionEvent &event);
        protected:
            std::unique_ptr<tetriq::ITransportLayer> _transport;
    };
}

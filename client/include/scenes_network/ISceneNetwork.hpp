#pragma once

#include "Registry.hpp"
#include "network/PacketHandler.hpp"
#include "network/transport/ITransportLayer.hpp"

class ISceneNetwork: public tetriq::PacketHandler {
    public:
        ISceneNetwork(ecs::Registry &registry, std::unique_ptr<tetriq::ITransportLayer> &transport);
        virtual ~ISceneNetwork() = default;

        virtual void update(float deltaTime) = 0;
    protected:
        std::unique_ptr<tetriq::ITransportLayer> &_transport;
        ecs::Registry &_registry;
};

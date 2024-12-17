#pragma once

#include <display/scenes/Menu.hpp>

#include "scenes_network/ISceneNetwork.hpp"

class MenuNetwork final: public ISceneNetwork {
    public:
        MenuNetwork(ecs::Registry &registry, std::unique_ptr<tetriq::ITransportLayer> &transport, Menu &menu);

        void update(float deltaTime);
        bool handleServerListResponsePacket(const tetriq::ServerListResponsePacket &,
                                            tetriq::ITransportPeer &) override;

    private:
        Menu &_menu;
};

#include "scenes_network/MenuNetwork.hpp"
#include "scenes_network/ISceneNetwork.hpp"

MenuNetwork::MenuNetwork(ecs::Registry &registry, std::unique_ptr<tetriq::ITransportLayer> &transport, Menu &menu) :
    ISceneNetwork(registry, transport), _menu(menu)
{}

void MenuNetwork::update(float)
{
    auto &actions = _menu.getActionQueue();
    while (!actions.empty()) {
        switch (actions.front()) {
            case Menu::MenuAction::QueryServerList:
                //* Send a request to get the server list
                _transport->sendPacket(tetriq::ServerListRequestPacket{}, _transport->getPeer(0));
                break;
            default:
                break;
        }
        actions.pop();
    }
}

bool MenuNetwork::handleServerListResponsePacket(const tetriq::ServerListResponsePacket &packet, tetriq::ITransportPeer &)
{
    //* Update the menu with the received server list
    _menu.setServers(packet.servers);
    return true;
}

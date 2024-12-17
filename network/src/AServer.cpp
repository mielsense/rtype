#include "network/AServer.hpp"
#include "Logging.hpp"

namespace tetriq {

    AServer::AServer(std::unique_ptr<tetriq::ITransportLayer> &transport_layer)
        : _transport(std::move(transport_layer))
    {}

    void AServer::loop(float delta_time)
    {
        onTick(delta_time);
        _transport->update();
        std::queue<std::unique_ptr<tetriq::TransportEvent>> &events = _transport->getEvents();
        while (!events.empty()) {
            std::unique_ptr<tetriq::TransportEvent> &event = events.front();
            switch (event->type) {
            case tetriq::TransportEvent::Type::CONNECTION:
                onConnection(dynamic_cast<tetriq::ConnectionEvent &>(*event));
                break;
            case tetriq::TransportEvent::Type::DISCONNECTION:
                onDisconnection(dynamic_cast<tetriq::DisconnectionEvent &>(*event));
                break;
            case tetriq::TransportEvent::Type::PACKET:
            {
                tetriq::PacketEvent &packet_event = static_cast<tetriq::PacketEvent &>(*event);
                if (!handlePacket(*packet_event.packet, packet_event.peer))
                    LOG_WARNING("packet unhandled");
                break;
            }
            }
            events.pop();
        }
    }

    void AServer::onTick(float delta_time)
    {}

    void AServer::onConnection(tetriq::ConnectionEvent &)
    {}

    void AServer::onDisconnection(tetriq::DisconnectionEvent &)
    {}
}

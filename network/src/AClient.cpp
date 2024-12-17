#include "network/AClient.hpp"
#include "Logging.hpp"

namespace tetriq {
    AClient::AClient(std::unique_ptr<tetriq::ITransportLayer> &layer) : AServer(layer)
    {}

    void AClient::onConnection(tetriq::ConnectionEvent &event)
    {
        LOG_INFO("Connected to server");
        _server_id = event.peer.getId();
        _connected = true;
    }

    bool AClient::isConnected() const
    {
        return _connected;
    }
} // namespace tetriq

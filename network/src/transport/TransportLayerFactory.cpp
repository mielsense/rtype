#include "network/transport/TransportLayerFactory.hpp"
#include <memory>
#include "network/transport/ENetServer.hpp"

namespace tetriq {
    std::unique_ptr<ITransportLayer> TransportLayerFactory::create(const std::string &ip,
                                                                   uint16_t port,
                                                                   const std::string &server_name)
    {
        ENetServer::Config config = {
            .listen_address = ip,
            .listen_port = port,
            .max_clients = 64,
            .max_channels = 1,
        };
        return std::make_unique<ENetServer>(config, server_name);
    }

    std::unique_ptr<ITransportLayer> TransportLayerFactory::createClient(const std::string &ip,
                                                                         uint16_t port,
                                                                         const std::string &server_name)
    {
        ENetServer::Config config = {
            .listen_address = {},
            .listen_port = 0,
            .max_clients = 64,
            .max_channels = 1,
        };
        std::unique_ptr<ITransportLayer> layer = std::make_unique<ENetServer>(config, server_name);
        layer->connect(ip, port);
        return layer;
    }
}

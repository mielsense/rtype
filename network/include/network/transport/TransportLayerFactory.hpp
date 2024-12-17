#pragma once

#include "network/transport/ITransportLayer.hpp"
namespace tetriq {
    class TransportLayerFactory {
        public:
            /**
             * Creates a server listening on IP and PORT.
             */
            static std::unique_ptr<ITransportLayer> create(const std::string &ip, uint16_t port, const std::string &server_name);
            /**
             * Creates a host and connects to the server with IP and PORT.
             */
            static std::unique_ptr<ITransportLayer> createClient(const std::string &ip, uint16_t port, const std::string &server_name);
    };
}

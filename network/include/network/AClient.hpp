#include "network/AServer.hpp"

namespace tetriq {
    class AClient: public AServer {
        public:
            AClient(std::unique_ptr<tetriq::ITransportLayer> &layer);
            virtual ~AClient() = default;

            void onConnection(tetriq::ConnectionEvent &event) override;
            bool isConnected() const;

        protected:
            bool _connected{false};
            uint64_t _server_id;
    };
} // namespace tetriq

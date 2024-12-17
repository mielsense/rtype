#include "network/transport/ENetException.hpp"

namespace tetriq {
    ENetException::ENetException(std::string &&message)
        : _message(message)
    {}

    const char *ENetException::what() const noexcept
    {
        return _message.data();
    }
}

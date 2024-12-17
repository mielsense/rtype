#include "network/NetworkObject.hpp"
#include <string>
#include "network/NetworkDouble.hpp"

namespace tetriq {
    size_t NetworkObject::getNetworkSize(const NetworkObject &object)
    {
        return object.getNetworkSize();
    }

    size_t NetworkObject::getNetworkSize(const std::string &string)
    {
        return sizeof(uint64_t) + sizeof(char) * string.size();
    }

    size_t NetworkObject::getNetworkSize(double value)
    {
        return sizeof(tetriq::NetworkDouble);
    }

    NetworkOStream &operator<<(NetworkOStream &os, const NetworkObject &object)
    {
        return object.write(os);
    }

    NetworkIStream &operator>>(NetworkIStream &is, NetworkObject &object)
    {
        return object.read(is);
    }
} // namespace tetriq

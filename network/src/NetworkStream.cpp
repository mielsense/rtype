// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "network/NetworkStream.hpp"
#include <cstddef>
#include <cstdint>
#include "Logging.hpp"
#include "network/NetworkDouble.hpp"

namespace tetriq {
    NetworkOStream::NetworkOStream(size_t size) : _size(size), _buf(new uint8_t[size]), _cursor(0)
    {}

    const uint8_t *NetworkOStream::getData() const
    {
        if (_size != _cursor)
            LOG_WARNING("NetworkOStream was not completely filled before getting its data ({}/{} bytes)", _cursor,
                        _size);
        return _buf.get();
    }

    size_t NetworkOStream::getSize() const
    {
        return _size;
    }

    NetworkOStream &NetworkOStream::operator<<(const std::string &value)
    {
        uint64_t len = value.size();
        *this << len;
        if (_cursor > _size - len)
            throw NetworkStreamOverflowException();
        std::memcpy(&_buf[_cursor], value.data(), len);
        _cursor += len;
        return *this;
    }

    NetworkOStream &NetworkOStream::operator<<(double host_value)
    {
        NetworkDouble network_value{host_value};
        std::memcpy(&_buf[_cursor], &network_value, sizeof(NetworkDouble));
        _cursor += sizeof(NetworkDouble);
        return *this;
    }

    NetworkIStream::NetworkIStream(ENetPacket *packet) : _is_packet(true), _packet(packet), _cursor(0)
    {}

    NetworkIStream::NetworkIStream(NetworkOStream *os) : _is_packet(false), _os(os), _cursor(0)
    {}

    NetworkIStream::~NetworkIStream()
    {
        if (_is_packet)
            enet_packet_destroy(_packet);
    }

    const uint8_t *NetworkIStream::getData() const
    {
        if (_is_packet)
            return _packet->data;
        else
            return _os->getData();
    }

    size_t NetworkIStream::getSize() const
    {
        if (_is_packet)
            return _packet->dataLength;
        else
            return _os->getSize();
    }

    NetworkIStream &NetworkIStream::operator>>(std::string &value)
    {
        uint64_t len;
        *this >> len;
        if (_cursor > getSize() - len)
            throw NetworkStreamOverflowException();
        value.assign(reinterpret_cast<const char *>(&getData()[_cursor]), len);
        _cursor += len;
        return *this;
    }

    NetworkIStream &NetworkIStream::operator>>(double &value)
    {
        NetworkDouble network{0};
        std::memcpy(&network, &getData()[_cursor], sizeof(NetworkDouble));
        _cursor += sizeof(NetworkDouble);
        value = network.toHostDouble();
        return *this;
    }
} // namespace tetriq

// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "network/NetworkStream.hpp"
#include <bitset>
#include <enet/enet.h>
#include <iostream>

namespace tetriq {
    class NetworkObject {
        public:
            virtual ~NetworkObject() = default;

            /**
             * @returns the size of the object as it will be on the network.
             */
            virtual size_t getNetworkSize() const = 0;

            friend NetworkOStream &operator<<(NetworkOStream &os, const NetworkObject &object);
            friend NetworkIStream &operator>>(NetworkIStream &os, NetworkObject &object);

            template <typename N>
            requires std::integral<N>
            static size_t getNetworkSize(N object);
            template <typename T>
            requires IsEnum<T>
            static size_t getNetworkSize(T object);
            template <typename T, std::size_t N>
            static size_t getNetworkSize(const std::array<T, N> &array);
            template <typename C, typename It = C::iterator>
            static size_t getNetworkSize(const C &object);
            static size_t getNetworkSize(const NetworkObject &object);
            static size_t getNetworkSize(const std::string &string);
            static size_t getNetworkSize(double value);
            template <std::size_t N>
            static size_t getNetworkSize(const std::bitset<N> set);
            template <typename A, typename B>
            static size_t getNetworkSize(const std::pair<A, B> &pair);
            // template <typename T>
            // static size_t getNetworkSize(const std::unordered_map<T> &map);
        protected:
            /**
             * @brief Writes the object to a stream.
             */
            virtual NetworkOStream &write(NetworkOStream &os) const = 0;

            /**
             * @brief Reads the object from a stream.
             */
            virtual NetworkIStream &read(NetworkIStream &is) = 0;
    };
    
    NetworkOStream &operator<<(NetworkOStream &os, const NetworkObject &object);
    NetworkIStream &operator>>(NetworkIStream &is, NetworkObject &object);

    template <typename N>
    requires std::integral<N>
    size_t NetworkObject::getNetworkSize(N object)
    {
        return sizeof(object);
    }

    template <typename T>
    requires IsEnum<T>
    size_t NetworkObject::getNetworkSize(T value)
    {
        return sizeof(std::underlying_type<T>::type);
    }

    template <typename T, std::size_t N>
    size_t NetworkObject::getNetworkSize(const std::array<T, N> &)
    {
        return sizeof(T) * N;
    }

    template <typename C, typename It>
    size_t NetworkObject::getNetworkSize(const C &object)
    {
        // We assume all objects are the same size for performance
        if (object.size() == 0)
            return sizeof(uint64_t);
        return sizeof(uint64_t) + object.size() * getNetworkSize(*object.begin());
    }

    template<std::size_t N>
    size_t NetworkObject::getNetworkSize(const std::bitset<N>)
    {
        return sizeof(uint64_t);
    }

    template<typename A, typename B>
    size_t NetworkObject::getNetworkSize(const std::pair<A, B> &pair)
    {
        return getNetworkSize(pair.first)
            + getNetworkSize(pair.second);
    }
}

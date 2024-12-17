// SPDX-FileCopyrightText: 2024 The TetriQ authors
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <enet/enet.h>
#include <exception>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#ifdef _WIN32
    #include <winsock2.h>
    #include <stdlib.h>

    // Define endian conversion functions for Windows
    #if BYTE_ORDER == LITTLE_ENDIAN
    #define htole64(x) (x)
    #define le64toh(x) (x)
    #else
    #define htole64(x) _byteswap_uint64(x)
    #define le64toh(x) _byteswap_uint64(x)
    #endif

#elif defined(__APPLE__)
    #include <libkern/OSByteOrder.h>
    #include <machine/endian.h>

    #define htole64(x) OSSwapHostToLittleInt64(x)
    #define le64toh(x) OSSwapLittleToHostInt64(x)

    // Detect endianness
    #if BYTE_ORDER == LITTLE_ENDIAN
    #define LITTLE_ENDIAN 1234
    #define BIG_ENDIAN 4321
    #define BYTE_ORDER LITTLE_ENDIAN
    #elif BYTE_ORDER == BIG_ENDIAN
    #define LITTLE_ENDIAN 1234
    #define BIG_ENDIAN 4321
    #define BYTE_ORDER BIG_ENDIAN
    #else
    #error "Unable to determine endianness"
    #endif

#else
    #include <endian.h>
#endif

namespace tetriq {
    template <typename T>
    concept IsEnum = std::is_enum_v<T>;
    class NetworkStreamOverflowException: public std::exception {};

    template <typename Enum>
    constexpr std::underlying_type_t<Enum> to_underlying( Enum e ) noexcept
    {
        return static_cast<std::underlying_type_t<Enum>>(e);
    }
    
    /**
     * @brief Binary stream for a buffer of known size to be sent over the wire.
     * The stream should use the network endianness for all values.
     */
    class NetworkOStream {
        public:
            NetworkOStream(size_t size);

            const uint8_t *getData() const;
            size_t getSize() const;

            template <typename N>
                requires std::integral<N>
            NetworkOStream &operator<<(N value);
            template <typename T>
                requires IsEnum<T>
            NetworkOStream &operator<<(T value);
            template <typename T, std::size_t N>
            NetworkOStream &operator<<(const std::array<T, N> &array);
            template <typename C, typename It = C::iterator>
            NetworkOStream &operator<<(const C &container);
            NetworkOStream &operator<<(const std::string &value);
            NetworkOStream &operator<<(double value);
            template<std::size_t N>
            NetworkOStream &operator<<(const std::bitset<N> &set);
            template<typename A, typename B>
            NetworkOStream &operator<<(const std::pair<A, B> &pair);
        private:
            size_t _size;
            std::unique_ptr<uint8_t[]> _buf;
            size_t _cursor;
    };

    template <typename N>
        requires std::integral<N>
    NetworkOStream &NetworkOStream::operator<<(N value)
    {
        if (_cursor > _size - sizeof(N))
            throw NetworkStreamOverflowException();
        uint64_t flipped = htole64(value);
        *(N *) &_buf[_cursor] = flipped;
        _cursor += sizeof(N);
        return *this;
    }

    template <typename T>
        requires IsEnum<T>
    NetworkOStream &NetworkOStream::operator<<(T value)
    {
        return *this << tetriq::to_underlying(value);
    }

    template <typename T, std::size_t N>
    NetworkOStream &NetworkOStream::operator<<(const std::array<T, N> &array)
    {
        for (const T &elem : array) {
            *this << elem;
        }
        return *this;
    }

    template <typename C, typename It>
    NetworkOStream &NetworkOStream::operator<<(const C &container)
    {
        uint64_t len = container.size();
        *this << len;
        for (auto pp = container.begin(); pp != container.end(); ++pp) {
            *this << *pp;
        }
        return *this;
    }

    template<std::size_t N>
    NetworkOStream &NetworkOStream::operator<<(const std::bitset<N> &set)
    {
        static_assert(sizeof(unsigned long long) <= sizeof(uint64_t));
        return *this << static_cast<uint64_t>(set.to_ullong());
    }

    template<typename A, typename B>
    NetworkOStream &NetworkOStream::operator<<(const std::pair<A, B> &pair)
    {
        return *this << pair.first
                     << pair.second;
    }

    /**
     * @brief Binary stream to read a packet received from the wire.
     * The stream should use the network endianness for all values.
     */
    class NetworkIStream {
        public:
            NetworkIStream(ENetPacket *packet);
            NetworkIStream(NetworkOStream *os);
            ~NetworkIStream();

            const uint8_t *getData() const;
            size_t getSize() const;

            template <typename T>
            T read();

            template <typename N>
                requires std::integral<N>
            NetworkIStream &operator>>(N &value);
            template <typename T>
                requires IsEnum<T>
            NetworkIStream &operator>>(T &value);
            template <typename T>
            NetworkIStream &operator>>(std::vector<T> &container);
            template <typename T, std::size_t N>
            NetworkIStream &operator>>(std::array<T, N> &array);
            template <typename C, typename It = C::iterator>
            NetworkIStream &operator>>(C &container);
            template <typename A, typename B>
            NetworkIStream &operator>>(std::unordered_map<A, B> &container);
            NetworkIStream &operator>>(std::string &value);
            NetworkIStream &operator>>(double &value);
            template<std::size_t N>
            NetworkIStream &operator>>(std::bitset<N> &set);
            template<typename A, typename B>
            NetworkIStream &operator>>(std::pair<A, B> &pair);
        private:
            bool _is_packet;
            union {
                    ENetPacket *const _packet;
                    NetworkOStream *_os;
            };
            size_t _cursor;
    };

    template <typename T>
    T NetworkIStream::read()
    {
        T res;
        *this >> res;
        return res;
    }

    template <typename N>
        requires std::integral<N>
    NetworkIStream &NetworkIStream::operator>>(N &value)
    {
        if (_cursor > getSize() - sizeof(N))
            throw NetworkStreamOverflowException();
        value = le64toh(*(N *) &getData()[_cursor]);
        _cursor += sizeof(N);
        return *this;
    }

    template <typename T>
        requires IsEnum<T>
    NetworkIStream &NetworkIStream::operator>>(T &value)
    {
        typename std::underlying_type<T>::type v;
        *this >> v;
        value = static_cast<T>(v);
        return *this;
    }

    template <typename T>
    NetworkIStream &NetworkIStream::operator>>(std::vector<T> &container)
    {
        uint64_t len;
        *this >> len;
        container.clear();
        container.reserve(len);
        for (uint64_t i = 0; i < len; i++) {
            container.emplace_back();
            *this >> container.back();
        }
        return *this;
    }

    template <typename T, std::size_t N>
    NetworkIStream &NetworkIStream::operator>>(std::array<T, N> &array)
    {
        for (T &elem : array) {
            *this >> elem;
        }
        return *this;
    }

    template <typename C, typename It>
    NetworkIStream &NetworkIStream::operator>>(C &container)
    {
        uint64_t len;
        *this >> len;
        container.clear();
        for (uint64_t i = 0; i < len; i++) {
            container.emplace_back();
            *this >> container.back();
        }
        return *this;
    }

    template <typename A, typename B>
    NetworkIStream &NetworkIStream::operator>>(std::unordered_map<A, B> &container)
    {
        uint64_t len;
        *this >> len;
        container.clear();
        for (uint64_t i = 0; i < len; i++) {
            std::pair<A, B> pair;
            *this >> pair;
            container.emplace(pair);
        }
        return *this;
    }

    template<std::size_t N>
    NetworkIStream &NetworkIStream::operator>>(std::bitset<N> &set)
    {
        static_assert(sizeof(unsigned long long) <= sizeof(uint64_t));
        uint64_t n;
        *this >> n;
        set = static_cast<unsigned long long>(n);
        return *this;
    }

    template<typename A, typename B>
    NetworkIStream &NetworkIStream::operator>>(std::pair<A, B> &pair)
    {
        return *this >> pair.first
                     >> pair.second;
    }
} // namespace tetriq

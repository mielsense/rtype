#include "network/NetworkStream.hpp"
#include "network/Types.hpp"

#include <cmath>
#include <gtest/gtest.h>
#include <deque>

TEST(network_stream, bool)
{
    tetriq::NetworkOStream os{2};
    os << true;
    os << false;

    ASSERT_EQ(os.getData()[0], true);
    ASSERT_EQ(os.getData()[1], false);
}

TEST(network_stream, integers)
{
    uint8_t uint8{234};
    uint16_t uint16{56898};
    uint32_t uint32{897987979};
    uint64_t uint64{9088989898765458887};
    tetriq::NetworkOStream os{1 + 2 + 4 + 8};
    os << uint8 << uint16 << uint32 << uint64;

    tetriq::NetworkIStream is{&os};
    is >> uint8 >> uint16 >> uint32 >> uint64;

    ASSERT_EQ(uint8, 234);
    ASSERT_EQ(uint16, 56898);
    ASSERT_EQ(uint32, 897987979);
    ASSERT_EQ(uint64, 9088989898765458887);
}

TEST(network_stream, floating_point)
{
    double d{M_E};
    
    tetriq::NetworkOStream os{tetriq::NetworkObject::getNetworkSize(d)};
    os << d;

    tetriq::NetworkIStream is{&os};
    is >> d;

    ASSERT_LT(std::fabs(d - static_cast<double>(M_E)), 0.001);
}

TEST(network_stream, enum)
{
    enum class TestEnum : uint16_t {
        BANANA,
        APPLE,
        KIWI,
        ORANGE,
    };
    TestEnum banana;

    tetriq::NetworkOStream os{sizeof(std::underlying_type<TestEnum>::type) * 4};
    os << TestEnum::ORANGE << TestEnum::BANANA << TestEnum::APPLE << TestEnum::KIWI;

    tetriq::NetworkIStream is{&os};
    is >> banana >> banana;
    ASSERT_EQ(banana, TestEnum::BANANA);
    is >> banana >> banana;
    ASSERT_EQ(banana, TestEnum::KIWI);
}

TEST(network_stream, vector)
{
    std::vector<uint64_t> triangles = { 3, 9, 81, 6561, 43046721, 1853020188851841 };
    tetriq::NetworkOStream os{sizeof(uint64_t) * 7};
    os << triangles;
    tetriq::NetworkIStream is{&os};
    std::vector<uint64_t> out;
    is >> out;
    ASSERT_EQ(out, triangles);
}

TEST(network_stream, deque)
{
    std::deque<uint64_t> triangles = { 3, 9, 81, 6561, 43046721, 1853020188851841 };
    tetriq::NetworkOStream os{sizeof(uint64_t) * 7};
    os << triangles;
    tetriq::NetworkIStream is{&os};
    std::deque<uint64_t> out;
    is >> out;
    ASSERT_EQ(out, triangles);
}

TEST(network_stream, array)
{
    std::array<uint64_t, 6> triangles = { 3, 9, 81, 6561, 43046721, 1853020188851841 };
    tetriq::NetworkOStream os{sizeof(uint64_t) * 6};
    os << triangles;
    tetriq::NetworkIStream is{&os};
    std::array<uint64_t, 6> out;
    is >> out;
    ASSERT_EQ(out, triangles);
}

TEST(network_stream, bitset)
{
    std::bitset<64> set = 0b1001100101010100100110010101010010011001010101001001100101010100;
    ASSERT_EQ(tetriq::NetworkObject::getNetworkSize(set),
              sizeof(uint64_t));
    tetriq::NetworkOStream os{tetriq::NetworkObject::getNetworkSize(set)};
    os << set;
    tetriq::NetworkIStream is{&os};
    std::bitset<64> out;
    is >> out;
    ASSERT_EQ(out, set);
}

TEST(network_stream, pair)
{
    std::pair<uint64_t, bool> pair = { 1853020188851841, true };
    ASSERT_EQ(tetriq::NetworkObject::getNetworkSize(pair),
              sizeof(uint64_t) + sizeof(uint8_t));
    tetriq::NetworkOStream os{tetriq::NetworkObject::getNetworkSize(pair)};
    os << pair;
    tetriq::NetworkIStream is{&os};
    std::pair<uint64_t, bool> out;
    is >> out;
    ASSERT_EQ(out, pair);
}

TEST(network_stream, unordered_map)
{
    std::unordered_map<uint64_t, bool> map = { { 1853020188851841, true},
                                               { 34, false } };
    ASSERT_EQ(tetriq::NetworkObject::getNetworkSize(map),
              sizeof(uint64_t) + (sizeof(uint64_t) + sizeof(uint8_t)) * 2);
    tetriq::NetworkOStream os{tetriq::NetworkObject::getNetworkSize(map)};
    os << map;
    tetriq::NetworkIStream is{&os};
    std::unordered_map<uint64_t, bool> out;
    is >> out;
    ASSERT_EQ(out, map);
}

TEST(network_stream, position)
{
    tetriq::Position pos = { 12, 34 };
    tetriq::NetworkOStream os{tetriq::NetworkObject::getNetworkSize(pos)};
    os << pos;
    tetriq::NetworkIStream is{&os};
    tetriq::Position out;
    is >> out;
    ASSERT_EQ(out, pos);
}

TEST(network_stream, color)
{
    tetriq::Color color = { 12, 34, 234 };
    tetriq::NetworkOStream os{sizeof(uint8_t) * 3};
    os << color;
    tetriq::NetworkIStream is{&os};
    tetriq::Color out;
    is >> out;
    ASSERT_EQ(out, color);
}

TEST(network_stream, player)
{
    tetriq::Player player = { "julia", { 10, 4, 5 }, { 3, 2 } };
    tetriq::NetworkOStream os{tetriq::NetworkObject::getNetworkSize(player)};
    os << player;
    tetriq::NetworkIStream is{&os};
    tetriq::Player out = { "", {}, {} };
    std::string str;
    tetriq::Color color;
    tetriq::Position position;
    is >> str;
    is >> color;
    is >> position;
    ASSERT_EQ(str, player.name);
    ASSERT_EQ(color, player.color);
    ASSERT_EQ(position, player.position);
}

TEST(network_stream, write_overflow)
{
    tetriq::NetworkOStream os{6};
    uint8_t a{8};
    os << a << a << a << a << a << a;
    try {
        os << a;
    } catch(const tetriq::NetworkStreamOverflowException &exception) {
        return;
    }
    FAIL() << "Expected NetworkStreamOverflowException";
}

TEST(network_stream, read_overflow)
{
    tetriq::NetworkOStream os{6};
    uint8_t a{8};
    os << a << a << a << a << a << a;
    tetriq::NetworkIStream is{&os};
    is >> a >> a >> a >> a >> a >> a;
    try {
        is >> a;
    } catch (const tetriq::NetworkStreamOverflowException &exception) {
        return;
    }
    FAIL() << "Expected NetworkStreamOverflowException";
}

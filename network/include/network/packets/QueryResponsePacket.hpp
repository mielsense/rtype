// This file was auto-generated by generate-packets.scm, do not modify it manually.
#pragma once

#include "network/APacket.hpp"
#include "network/Types.hpp"

namespace tetriq {
	class QueryResponsePacket : public APacket {
	public:
		QueryResponsePacket(uint64_t player_count, uint64_t max_players, uint16_t tick_rate, std::string server_name, uint64_t current_level, std::string server_ip, uint16_t server_port);
		QueryResponsePacket(NetworkIStream &is);

		PacketId getId() const override;
		size_t getNetworkSize() const override;
		// variables
		uint64_t player_count;
		uint64_t max_players;
		uint16_t tick_rate;
		std::string server_name;
		uint64_t current_level;
		std::string server_ip;
		uint16_t server_port;

	protected:
		NetworkOStream &write(NetworkOStream &os) const override;
		NetworkIStream &read(NetworkIStream &is) override;
	};
}

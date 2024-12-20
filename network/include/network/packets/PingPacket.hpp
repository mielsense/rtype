// This file was auto-generated by generate-packets.scm, do not modify it manually.
#pragma once

#include "network/APacket.hpp"
#include "network/Types.hpp"

namespace tetriq {
	class PingPacket : public APacket {
	public:
		PingPacket(time_t time);
		PingPacket(NetworkIStream &is);

		PacketId getId() const override;
		size_t getNetworkSize() const override;
		// variables
		time_t time;

	protected:
		NetworkOStream &write(NetworkOStream &os) const override;
		NetworkIStream &read(NetworkIStream &is) override;
	};
}

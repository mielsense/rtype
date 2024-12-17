// This file was auto-generated by generate-packets.scm, do not modify it manually.
#pragma once

#include "network/APacket.hpp"
#include "network/Types.hpp"

namespace tetriq {
	class MovePlayerPacket : public APacket {
	public:
		MovePlayerPacket(Position new_pos);
		MovePlayerPacket(NetworkIStream &is);

		size_t getNetworkSize() const override;
		// variables
		Position new_pos;

	protected:
		NetworkOStream &write(NetworkOStream &os) const override;
		NetworkIStream &read(NetworkIStream &is) override;
		PacketId getId() const override;
	};
}
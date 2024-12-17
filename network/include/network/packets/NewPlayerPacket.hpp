// This file was auto-generated by generate-packets.scm, do not modify it manually.
#pragma once

#include "network/APacket.hpp"
#include "network/Types.hpp"

namespace tetriq {
	class NewPlayerPacket : public APacket {
	public:
		NewPlayerPacket(uint64_t id, Position position);
		NewPlayerPacket(NetworkIStream &is);

		PacketId getId() const override;
		size_t getNetworkSize() const override;
		// variables
		uint64_t id;
		Position position;

	protected:
		NetworkOStream &write(NetworkOStream &os) const override;
		NetworkIStream &read(NetworkIStream &is) override;
	};
}

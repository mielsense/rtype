// This file was auto-generated by generate-packets.scm, do not modify it manually.
#pragma once

#include "network/APacket.hpp"
#include "network/Types.hpp"

namespace tetriq {
	class StartGamePacket : public APacket {
	public:
		StartGamePacket();
		StartGamePacket(NetworkIStream &is);

		PacketId getId() const override;
		size_t getNetworkSize() const override;
		// variables

	protected:
		NetworkOStream &write(NetworkOStream &os) const override;
		NetworkIStream &read(NetworkIStream &is) override;
	};
}

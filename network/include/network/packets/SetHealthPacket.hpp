// This file was auto-generated by generate-packets.scm, do not modify it manually.
#pragma once

#include "network/APacket.hpp"
#include "network/Types.hpp"

namespace tetriq {
	class SetHealthPacket : public APacket {
	public:
		SetHealthPacket(double health, double max_health);
		SetHealthPacket(NetworkIStream &is);

		PacketId getId() const override;
		size_t getNetworkSize() const override;
		// variables
		double health;
		double max_health;

	protected:
		NetworkOStream &write(NetworkOStream &os) const override;
		NetworkIStream &read(NetworkIStream &is) override;
	};
}

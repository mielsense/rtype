// This file was auto-generated by generate-packets.scm, do not modify it manually.
#include "network/packets/SpecialAbilityActionPacket.hpp"
namespace tetriq {
SpecialAbilityActionPacket::SpecialAbilityActionPacket(uint8_t ability, Position position)
	:ability(ability)
	,position(position)
{}
SpecialAbilityActionPacket::SpecialAbilityActionPacket(NetworkIStream &is)
	:ability(is.read<uint8_t>())
	,position(is.read<Position>())
{}
size_t SpecialAbilityActionPacket::getNetworkSize() const {
	return 0 + NetworkObject::getNetworkSize(ability) + NetworkObject::getNetworkSize(position);
}
NetworkOStream &SpecialAbilityActionPacket::write(NetworkOStream &stream) const {
	return stream << ability << position;
}
NetworkIStream &SpecialAbilityActionPacket::read(NetworkIStream &stream) {
	return stream >> ability >> position;
}
PacketId SpecialAbilityActionPacket::getId() const {
	return PacketId::SPECIAL_ABILITY_ACTION;
}
}
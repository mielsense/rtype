// This file was auto-generated by generate-packets.scm, do not modify it manually.
#include "network/packets/EntityMovementPacket.hpp"
namespace tetriq {
EntityMovementPacket::EntityMovementPacket(uint64_t entity_id, Position position)
	:entity_id(entity_id)
	,position(position)
{}
EntityMovementPacket::EntityMovementPacket(NetworkIStream &is)
	:entity_id(is.read<uint64_t>())
	,position(is.read<Position>())
{}
size_t EntityMovementPacket::getNetworkSize() const {
	return 0 + NetworkObject::getNetworkSize(entity_id) + NetworkObject::getNetworkSize(position);
}
NetworkOStream &EntityMovementPacket::write(NetworkOStream &stream) const {
	return stream << entity_id << position;
}
NetworkIStream &EntityMovementPacket::read(NetworkIStream &stream) {
	return stream >> entity_id >> position;
}
PacketId EntityMovementPacket::getId() const {
	return PacketId::ENTITY_MOVEMENT;
}
}
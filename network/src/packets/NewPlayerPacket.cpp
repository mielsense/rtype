// This file was auto-generated by generate-packets.scm, do not modify it manually.
#include "network/packets/NewPlayerPacket.hpp"
namespace tetriq {
NewPlayerPacket::NewPlayerPacket(uint64_t id, Position position)
	:id(id)
	,position(position)
{}
NewPlayerPacket::NewPlayerPacket(NetworkIStream &is)
	:id(is.read<uint64_t>())
	,position(is.read<Position>())
{}
size_t NewPlayerPacket::getNetworkSize() const {
	return 0 + NetworkObject::getNetworkSize(id) + NetworkObject::getNetworkSize(position);
}
NetworkOStream &NewPlayerPacket::write(NetworkOStream &stream) const {
	return stream << id << position;
}
NetworkIStream &NewPlayerPacket::read(NetworkIStream &stream) {
	return stream >> id >> position;
}
PacketId NewPlayerPacket::getId() const {
	return PacketId::NEW_PLAYER;
}
}

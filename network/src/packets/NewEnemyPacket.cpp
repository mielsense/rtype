// This file was auto-generated by generate-packets.scm, do not modify it manually.
#include "network/packets/NewEnemyPacket.hpp"
namespace tetriq {
NewEnemyPacket::NewEnemyPacket(uint64_t id, Position position, Velocity velocity, uint64_t type)
	:id(id)
	,position(position)
	,velocity(velocity)
	,type(type)
{}
NewEnemyPacket::NewEnemyPacket(NetworkIStream &is)
	:id(is.read<uint64_t>())
	,position(is.read<Position>())
	,velocity(is.read<Velocity>())
	,type(is.read<uint64_t>())
{}
size_t NewEnemyPacket::getNetworkSize() const {
	return 0 + NetworkObject::getNetworkSize(id) + NetworkObject::getNetworkSize(position) + NetworkObject::getNetworkSize(velocity) + NetworkObject::getNetworkSize(type);
}
NetworkOStream &NewEnemyPacket::write(NetworkOStream &stream) const {
	return stream << id << position << velocity << type;
}
NetworkIStream &NewEnemyPacket::read(NetworkIStream &stream) {
	return stream >> id >> position >> velocity >> type;
}
PacketId NewEnemyPacket::getId() const {
	return PacketId::NEW_ENEMY;
}
}

// This file was auto-generated by generate-packets.scm, do not modify it manually.
#include "network/packets/StartGamePacket.hpp"
namespace tetriq {
StartGamePacket::StartGamePacket()
{}
StartGamePacket::StartGamePacket(NetworkIStream &is)
{}
size_t StartGamePacket::getNetworkSize() const {
	return 0;
}
NetworkOStream &StartGamePacket::write(NetworkOStream &stream) const {
	return stream;
}
NetworkIStream &StartGamePacket::read(NetworkIStream &stream) {
	return stream;
}
PacketId StartGamePacket::getId() const {
	return PacketId::S_START_GAME;
}
}

// This file was auto-generated by generate-packets.scm, do not modify it manually.
#include "network/packets/PlayerNamePacket.hpp"
namespace tetriq {
PlayerNamePacket::PlayerNamePacket(std::string player_name, uint64_t player_id)
	:player_name(player_name)
	,player_id(player_id)
{}
PlayerNamePacket::PlayerNamePacket(NetworkIStream &is)
	:player_name(is.read<std::string>())
	,player_id(is.read<uint64_t>())
{}
size_t PlayerNamePacket::getNetworkSize() const {
	return 0 + NetworkObject::getNetworkSize(player_name) + NetworkObject::getNetworkSize(player_id);
}
NetworkOStream &PlayerNamePacket::write(NetworkOStream &stream) const {
	return stream << player_name << player_id;
}
NetworkIStream &PlayerNamePacket::read(NetworkIStream &stream) {
	return stream >> player_name >> player_id;
}
PacketId PlayerNamePacket::getId() const {
	return PacketId::PLAYER_NAME;
}
}

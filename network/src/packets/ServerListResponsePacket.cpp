// This file was auto-generated by generate-packets.scm, do not modify it manually.
#include "network/packets/ServerListResponsePacket.hpp"
namespace tetriq {
ServerListResponsePacket::ServerListResponsePacket(std::vector<ServerInfo> servers)
	:servers(servers)
{}
ServerListResponsePacket::ServerListResponsePacket(NetworkIStream &is)
	:servers(is.read<std::vector<ServerInfo>>())
{}
size_t ServerListResponsePacket::getNetworkSize() const {
	return 0 + NetworkObject::getNetworkSize(servers);
}
NetworkOStream &ServerListResponsePacket::write(NetworkOStream &stream) const {
	return stream << servers;
}
NetworkIStream &ServerListResponsePacket::read(NetworkIStream &stream) {
	return stream >> servers;
}
PacketId ServerListResponsePacket::getId() const {
	return PacketId::S_SERVER_LIST_RESPONSE;
}
}

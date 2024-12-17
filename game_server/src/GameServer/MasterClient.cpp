#include "GameServer/MasterClient.hpp"
#include "Logging.hpp"

MasterClient::MasterClient(GameServer &game_server, std::unique_ptr<tetriq::ITransportLayer> &transport)
    : tetriq::AServer(transport)
    , server(game_server)
{}

bool MasterClient::handleQueryRequestPacket(const tetriq::QueryRequestPacket &, tetriq::ITransportPeer &peer)
{
    //LOG_INFO("[CLIENT] Received query request packet");
    const tetriq::QueryResponsePacket query_response_packet(server.getPlayerCount(),
                                                            server.getMaxClients(),
                                                            server.getTickRate(),
                                                            server.getServerName(),
                                                            server.getLevel(),
                                                            server.getServerIp(),
                                                            server.getServerPort()
                                                            );
    _transport->sendPacket(query_response_packet, peer);
    return true;
}

void MasterClient::onConnection(tetriq::ConnectionEvent &)
{
    LOG_INFO("[CLIENT] Connected to master server");
}

void MasterClient::onTick(float)
{}

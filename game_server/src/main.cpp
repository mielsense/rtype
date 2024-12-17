/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main.cpp
*/

#include <csignal>

#include "GameServer/GameServer.hpp"
#include "GameServer/MasterClient.hpp"
#include "Logging.hpp"
#include "Tickable.hpp"
#include "argparse/argparse.hpp"
#include "network/transport/TransportLayerFactory.hpp"

#ifdef _WIN32
#define SIGKILL SIGTERM
#endif

bool should_exit = false;

void catch_sigint()
{
    signal(SIGINT,
           [](int)
           {
               if (should_exit)
                   raise(SIGKILL);
               should_exit = true;
               LOG_WARNING("Caught SIGINT, exiting...");
           });
}

//  4242 -> MasterServer
//  4243 -> InterServer
//  4244 -> GameServer
int main(const int argc, char *argv[])
{
    catch_sigint();

    enet_initialize();

    argparse::ArgumentParser program("server");

    program.add_argument("--map-path")
           .help("Path to the map")
           .default_value(std::string("client/assets/maps/map-1.json"))
           .action([](const std::string &value)
           {
               return value;
           });

    program.add_argument("--server-name")
           .help("Name of the game server")
           .default_value(std::string("GameServer"))
           .action([](const std::string &value)
           {
               return value;
           });

    program.add_argument("--max-players")
           .help("Maximum number of players")
           .default_value(1)
           .scan<'i', int>();

    program.add_argument("--server-ip")
           .help("IP address for the game server")
           .default_value(std::string("0.0.0.0"))
           .action([](const std::string &value)
           {
               return value;
           });

    program.add_argument("--server-port").help("Port for the game server").default_value(4244).scan<'i', int>();

    program.add_argument("--master-ip")
           .help("IP address for the master server")
           .default_value(std::string("localhost"))
           .action([](const std::string &value)
           {
               return value;
           });

    program.add_argument("--master-port").help("Port for the master server").default_value(4243).scan<'i', int>();

    program.add_argument("--tickrate").help("Tickrate for the server").default_value(60).scan<'i', int>();

    program.add_argument("--disable-console-logging")
           .help("Disable console logging")
           .default_value(false)
           .implicit_value(true);

    program.add_argument("--logfile")
           .help("Log file, empty for no log file")
           .default_value(std::string(""))
           .action([](const std::string &value)
           {
               return value;
           });

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    // GameServer
    if (!program.get<std::string>("--logfile").empty()) {
        Logging::getInstance().setLogFile(program.get<std::string>("--logfile"));
    }
    Logging::getInstance().enableConsoleLogging(!program.get<bool>("--disable-console-logging"));

    const auto server_ip = program.get<std::string>("--server-ip");
    const uint16_t server_port = program.get<int>("--server-port");
    const auto server_name = program.get<std::string>("--server-name");
    uint64_t max_players = program.get<int>("--max-players");

    std::unique_ptr<tetriq::ITransportLayer> transport_layer =
        tetriq::TransportLayerFactory::create(server_ip, server_port, server_name);
    GameServer game_server{
        server_name,
        60,
        transport_layer,
        max_players,
    };

    game_server.setMapName(program.get<std::string>("--map-path"));

    const auto master_ip = program.get<std::string>("--master-ip");
    const uint16_t master_port = program.get<int>("--master-port");
    std::unique_ptr<tetriq::ITransportLayer> master_transport =
        tetriq::TransportLayerFactory::createClient(master_ip, master_port, "MasterClient");
    MasterClient master_client{game_server, master_transport};

    Tickable server_tick(program.get<int>("--tickrate"));

    server_tick.reset();
    auto delta = server_tick.getDeltaTime();
    while (!should_exit) {
        server_tick.tick();
        delta = server_tick.getDeltaTime();
        float dl = delta.count();
        dl /= 1000;
        game_server.loop(dl);
        master_client.loop(dl);
    }

    return 0;
}

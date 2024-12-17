/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main.cpp
*/

#include <iostream>
#include <csignal>

#include "Logging.hpp"
#include "argparse/argparse.hpp"
#include "MasterServer/MasterServer.hpp"
#include "Tickable.hpp"
#include "network/transport/ITransportLayer.hpp"
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

    argparse::ArgumentParser program("master_server");

    program.add_argument("--server-ip")
        .help("IP address for the master server")
        .default_value(std::string("0.0.0.0"))
        .action([](const std::string &value) { return value; });

    program.add_argument("--server-port").help("Port for the master server").default_value(4242).scan<'i', int>();

    program.add_argument("--inter-server-ip")
        .help("IP address for the inter server")
        .default_value(std::string("0.0.0.0"))
        .action([](const std::string &value) { return value; });

    program.add_argument("--inter-server-port").help("Port for the inter server").default_value(4243).scan<'i', int>();

    program.add_argument("--tickrate").help("Tickrate for the server").default_value(60).scan<'i', int>();

    program.add_argument("--disable-console-logging").help("Disable console logging").default_value(false).implicit_value(true);

    program.add_argument("--logfile")
        .help("Log file, empty for no log file")
        .default_value(std::string(""))
        .action([](const std::string &value) { return value; });

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    // MasterServer
    try {
        if (!program.get<std::string>("--logfile").empty()) {
            Logging::getInstance().setLogFile(program.get<std::string>("--logfile"));
        }
        Logging::getInstance().enableConsoleLogging(!program.get<bool>("--disable-console-logging"));
        const auto server_server_ip = program.get<std::string>("--inter-server-ip");
        const uint16_t server_server_port = program.get<int>("--inter-server-port");

        std::unique_ptr<tetriq::ITransportLayer> server_server_transport =
            tetriq::TransportLayerFactory::create(server_server_ip, server_server_port, "Master4Servers");

        const auto player_server_ip = program.get<std::string>("--server-ip");
        const uint16_t player_server_port = program.get<int>("--server-port");

        std::unique_ptr<tetriq::ITransportLayer> player_server_transport =
            tetriq::TransportLayerFactory::create(player_server_ip, player_server_port, "Master4Players");

        Tickable server_tick(program.get<int>("--tickrate"));
        MasterServer master_server;

        MasterServerForServers master_server_server{master_server, server_server_transport};
        MasterServerForPlayers master_players_server{master_server, player_server_transport};

        server_tick.reset();
        auto delta = server_tick.getDeltaTime();
        while (!should_exit) {
            server_tick.tick();
            delta = server_tick.getDeltaTime();
            float dl = delta.count() * 1000;
            master_server_server.loop(dl);
            master_players_server.loop(dl);
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    return 0;
}

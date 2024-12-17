/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <iostream>

#include "Logging.hpp"
#include "argparse/argparse.hpp"
#include "core/Core.hpp"
#include "network/transport/ITransportLayer.hpp"
#include "network/transport/TransportLayerFactory.hpp"
#include "utils/ConnectionConfig.hpp"
#include "utils/Error.hpp"

int main(const int ac, char **av)
{
    // arguments
    argparse::ArgumentParser program("client");

    // window details

    program.add_argument("--window-width").help("Default width od the window").default_value(1500).scan<'i', int>();

    program.add_argument("--window-height").help("Default height of the window").default_value(843).scan<'i', int>();

    program.add_argument("--window-title")
        .help("Title of the window")
        .default_value(std::string("R-Type"))
        .action([](const std::string &value) { return value; });

    program.add_argument("--fps-limit").help("Limit the frame rate").default_value(0).scan<'i', int>();

    // logger details

    program.add_argument("--console-logging").help("Enable console logging").default_value(false).implicit_value(true).default_value(true);

    program.add_argument("--log-file")
        .help("Log file path")
        .default_value(std::string(""))
        .action([](const std::string &value) { return value; });

    // lobby and master server details

    program.add_argument("--lobby").help("Use lobbies or not").default_value(false).implicit_value(true);

    program.add_argument("--master-ip")
        .help("IP address for the master server")
        .default_value(std::string("localhost"))
        .action([](const std::string &value) { return value; });

    program.add_argument("--master-port").help("Port for the master server").default_value(4242).scan<'i', int>();

    try {
        program.parse_args(ac, av);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return FAILURE;
    }

    // logger
    auto console_logging = program.get<bool>("--console-logging");
    auto log_file = program.get<std::string>("--log-file");

    if (!log_file.empty()) {
        Logging::getInstance().setLogFile(log_file);
    }
    if (!console_logging) {
        Logging::getInstance().enableConsoleLogging(false);
    }

    Logging::getInstance().enableConsoleLogging(true);

    //* Store connection details in global config
    auto& config = ConnectionConfig::getInstance();
    config.masterIp = program.get<std::string>("--master-ip");
    config.masterPort = static_cast<uint16_t>(program.get<int>("--master-port"));
    config.clientName = "GameClient";

    // main
    // TODO : handle case of master server (lobby) now that there is program arguments
    LOG(LogLevel::INFO, "Client application started...");
    try {
        //* Create transport layer for master server connection
        std::unique_ptr<tetriq::ITransportLayer> transport =
            tetriq::TransportLayerFactory::createClient(
                config.masterIp,
                config.masterPort,
                config.clientName
            );

        //* Create core
        Core core{
            transport,
            {
                static_cast<float>(program.get<int>("--window-width")),
                static_cast<float>(program.get<int>("--window-height"))
            },
            program.get<std::string>("--window-title"),
            static_cast<unsigned int>(program.get<int>("--fps-limit"))
        };

        //!Start game loop
        LOG_INFO("Starting game with {}:{}", config.masterIp, config.masterPort);
        core.run();
    } catch (const Error &e) {
        std::cerr << e.what() << std::endl;
        return FAILURE;
    } catch (const ecs::ECSException &e) {
        LOG_ERROR("{}", e.what());
        return FAILURE;
    } catch (...) {
        std::cerr << "An unexpected error occurred !" << std::endl;
        LOG_ERROR("An unexpected error occurred !");
        return FAILURE;
    }

    LOG(LogLevel::INFO, "Client application stopped.");
    return SUCCESS;
}

/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ConnectionConfig
*/

#ifndef CONNECTION_CONFIG_HPP
#define CONNECTION_CONFIG_HPP

#include <string>

struct ConnectionConfig {
        std::string masterIp;
        uint16_t masterPort;
        std::string clientName;

        static ConnectionConfig& getInstance() {
            static ConnectionConfig instance;
            return instance;
        }
};

#endif // CONNECTION_CONFIG_HPP
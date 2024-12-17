/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** vim on top !
*/

#ifndef ERROR_HPP
#define ERROR_HPP

#pragma once

#include <iostream>
#include "Logging.hpp"

#define FAILURE 84
#define SUCCESS 0

#define THROW_ERROR(message)                                                                                                 \
    do {                                                                                                               \
        LOG(LogLevel::ERROR_LOG, "{}", message);                                                                           \
        throw Error(message);                                                                                          \
    } while (0)

class Error: public std::exception {
    private:
        std::string message;

    public:
        explicit Error(const std::string &message = "Unknown error!") :
            message("\033[1;31m[error] : \033[0m" + message + "!")
        {}
        ~Error() = default;

        const char *what() const noexcept override
        {
            return message.c_str();
        }
};

#endif // ERROR_HPP

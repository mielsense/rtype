/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ECSException
*/

#ifndef ECSEXCEPTION_HPP
#define ECSEXCEPTION_HPP

#include <exception>
#include <string>
#include <utility>

namespace ecs {
    class ECSException final: public std::exception {
        private:
            std::string _message;
            int _errorCode;

        public:
            explicit ECSException(std::string msg, const int code = 1) : _message(std::move(msg)), _errorCode(code)
            {}

            [[nodiscard]] const char *what() const noexcept override
            {
                return _message.c_str();
            }

            [[nodiscard]] int getCode() const noexcept
            {
                return _errorCode;
            }
    };
} // namespace ecs

#endif // ECSEXCEPTION_HPP

#pragma once

#include <exception>
#include <string>

namespace tetriq {
    class ENetException : public std::exception {
        public:
            ENetException(std::string &&message);
            const char *what() const noexcept override;
        private:
            std::string _message;
    };
}

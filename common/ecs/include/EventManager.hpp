/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EventManager
*/

#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <functional>
#include <typeindex>
#include "Entity.hpp"

namespace ecs {
    class EventManager {
        public:
            template <typename EventType>
            using EventListener = std::function<void(const EventType &)>;

            template <typename EventType>
            void subscribe(EventListener<EventType> listener);

            template <typename EventType>
            void emit(const EventType &event);

        private:
            std::unordered_map<std::type_index, std::vector<std::function<void(const void *)>>> listeners;
    };

    template <typename EventType>
    void EventManager::subscribe(EventListener<EventType> listener)
    {
        listeners[typeid(EventType)].push_back([listener](const void *event)
                                               { listener(*static_cast<const EventType *>(event)); });
    }

    template <typename EventType>
    void EventManager::emit(const EventType &event)
    {
        auto it = listeners.find(typeid(EventType));
        if (it != listeners.end()) {
            for (auto &listener : it->second) {
                listener(&event);
            }
        }
    }

    // Event Types
    struct EntityCreatedEvent {
            Entity id;
    };

    template <typename T>
    struct ComponentAddedEvent {
            Entity id;
            T component;
    };

    template <typename T>
    struct ComponentRemovedEvent {
            Entity id;
            T component;
    };
} // namespace ecs

#endif // EVENTMANAGER_HPP
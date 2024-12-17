/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Registry
*/

#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "ECSException.hpp"
#include "Entity.hpp"
#include "EventManager.hpp"
#include "SparseSet.hpp"
#include "components.hpp"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <vector>

namespace ecs {

    using ComponentMask = std::bitset<MAX_COMPONENTS>;

#define TYPENAME typeid(T).name()

    /**
     * @brief The Registry class manages entities and their associated components.
     *
     * It provides functions to create, flag, and delete entities, manage tags and layers,
     * and handle the addition, retrieval, and removal of components.
     */
    class Registry {
        public:
            /**
             * @brief Constructs a Registry with a new EventManager instance.
             */
            Registry() : eventManager(std::make_shared<EventManager>())
            {}

            /* ENTITY MANAGEMENT */

            /**
             * @brief Creates a new entity.
             *
             * If there are available recycled entities, they are reused. Otherwise, a new ID is assigned.
             * @return The newly created entity ID.
             */
            Entity createEntity();

            /**
             * @brief Flags an entity for deletion.
             *
             * The flagged entity will be deleted on the next cleanup pass.
             * @param id The entity to flag.
             */
            void flagEntity(const Entity &id);

            /**
             * @brief Retrieves all entities flagged for deletion.
             */
            std::vector<Entity> getFlaggedEntities();

            /**
             * @brief Deletes all entities flagged for deletion.
             *
             * Cleans up components and tags associated with each flagged entity.
             */
            void deleteFlaggedEntities();

            /**
             * @brief Tags an entity with a specified string tag.
             *
             * Allows retrieval of the entity by its tag.
             * @param id The entity to tag.
             * @param tag The tag to associate with the entity.
             */
            void tagEntity(Entity id, const std::string &tag);

            /**
             * @brief Removes a tag from an entity.
             * @param id The entity to untag.
             */
            void untagEntity(Entity id);

            /**
             * @brief Retrieves the tag associated with a specified entity.
             * @param id The entity whose tag is requested.
             * @return The tag associated with the entity.
             */
            const std::string &getTagByEntity(Entity id);

            /**
             * @brief Finds an entity by its associated tag.
             * @param tag The tag associated with the entity.
             * @return The entity associated with the specified tag.
             */
            Entity getEntityByTag(const std::string &tag);

            /**
             * @brief Assigns an entity to a specific layer.
             *
             * Layers can be used to categorize entities.
             * @param id The entity to assign.
             * @param layer The layer to assign the entity to.
             */
            void setEntityLayer(Entity id, std::size_t layer);

            /**
             * @brief Removes an entity from all layers.
             * @param id The entity to remove from layers.
             */
            void unsetEntityLayer(Entity id);

            /**
             * @brief Retrieves all entities in a specified layer.
             * @param layer The layer to retrieve entities from.
             * @return A vector of entities in the specified layer.
             */
            std::vector<Entity> getEntitiesByLayer(std::size_t layer);

            /* COMPONENT MANAGEMENT */

            /**
             * @brief Registers a new component type T in the registry.
             *
             * The component is stored in an internal sparse set.
             * @tparam T The component type to register.
             */
            template <typename T>
            void add();

            /**
             * @brief Assigns a component to an entity, creating or updating it.
             *
             * @param id The entity to assign the component to.
             * @param component The component data to assign.
             * @return A reference to the newly assigned component.
             */
            template <typename T>
            T &set(Entity id, T &&component = {});

            /**
             * @brief Retrieves a component of type T associated with an entity tag.
             *
             * @param tag The tag of the entity.
             * @return A reference to the component.
             */
            template <typename T>
            T &tryGet(const std::string &tag);

            /**
             * @brief Retrieves a component of type T associated with a specific entity.
             *
             * @param id The entity ID.
             * @return A reference to the component.
             */
            template <typename T>
            T &tryGet(Entity id);

            /**
             * @brief Retrieves multiple components associated with an entity tag.
             *
             * @param tag The tag of the entity.
             * @return A tuple of references to the requested components.
             */
            template <typename... Ts>
            std::tuple<Ts &...> tryGetMany(const std::string &tag);

            /**
             * @brief Retrieves multiple components associated with a specific entity.
             *
             * @param id The entity ID.
             * @return A tuple of references to the requested components.
             */
            template <typename... Ts>
            std::tuple<Ts &...> tryGetMany(Entity id);

            /**
             * @brief Removes a component of type T from an entity.
             *
             * @param id The entity ID.
             */
            template <typename T>
            void remove(Entity id);

            /**
             * @brief Checks if an entity has a component of type T.
             *
             * @param id The entity ID.
             * @return True if the component is present, false otherwise.
             */
            template <typename T>
            bool has(Entity id);

            /**
             * @brief Checks if an entity has multiple specified components.
             *
             * @param id The entity ID.
             * @return True if all specified components are present, false otherwise.
             */
            template <typename... Ts>
            bool hasMany(Entity id);

            /**
             * @brief Retrieves all entities with the specified component types.
             *
             * @return A vector of tuples containing entities and their components.
             */
            template <typename... Ts>
            std::vector<std::tuple<Entity, Ts &...>> getAllByComponents();

            /**
             * @brief Applies a function to each entity with specified components.
             *
             * Supports lambdas with either component references or an entity ID as the first parameter.
             * @tparam Ts The component types required.
             * @param func The function to apply.
             */
            template <typename... Ts, typename Func>
            void forEach(Func &&func);

            /**
             * @brief Clears all entities and components from the registry.
             */
            void clear();

            std::shared_ptr<EventManager> eventManager;

        private:
            std::vector<Entity> _availableEntities;  ///< Recycled entity IDs available for reuse.
            std::vector<Entity> _flaggedForDeletion; ///< Entities marked for deletion on the next cleanup.

            SparseSet<ComponentMask> _componentMasks;                            ///< Component masks for each entity.
            std::unordered_map<ComponentMask, SparseSet<Entity>> _groupings;     ///< Groupings of entities by component mask.
            std::vector<std::unique_ptr<ISparseSet>> _componentPools;            ///< Pools for each registered component type.
            std::unordered_map<const char *, std::size_t> _componentBitPosition; ///< Mapping from component type to bit position.

            std::unordered_map<std::string, Entity> _tags;                ///< Mapping from tags to entities.
            std::unordered_map<std::size_t, std::vector<Entity>> _layers; ///< Mapping from layers to entities.

            Entity _maxEntity = 0; ///< Tracks the maximum entity ID assigned.

            template <typename T>
            T &get(Entity id);

            template <typename... Ts>
            ComponentMask createMask();

            ComponentMask &getComponentMask(Entity id);

            void setComponentMask(ComponentMask &mask, Entity id);

            void removeComponentMask(const ComponentMask &mask, Entity id);

            SparseSet<Entity> &getEntitiesByMask(const ComponentMask &mask);

            template <typename T>
            ComponentMask::reference getComponentBit(ComponentMask &mask);

            template <typename T>
            void setComponentBit(ComponentMask &mask, bool val);

            template <typename T>
            std::size_t getComponentBitPosition();

            template <typename T>
            SparseSet<T> &getComponentPool();

            bool isValidEntity(Entity id);

            bool isValidEntity(const std::string &tag) const;
    };

    /*
    ** ENTITIES
    */

    inline Entity Registry::createEntity()
    {
        Entity id;

        if (_availableEntities.empty()) {
            if (_maxEntity >= MAX_ENTITIES)
                throw ECSException("Unable to create Entity : limit exceeded");
            id = _maxEntity++;
        } else {
            id = _availableEntities.back();
            _availableEntities.pop_back();
        }

        _componentMasks.set(id, {});

        return id;
    }

    inline void Registry::flagEntity(const Entity &id)
    {
        if (!isValidEntity(id))
            throw ECSException("Unable to delete Entity '" + std::to_string(id) + "' : Invalid id");
        if (std::ranges::find(_flaggedForDeletion, id) != _flaggedForDeletion.end()) {
            LOG_WARNING("Unable to delete Entity '{}' : Entity already flagged for deletion", std::to_string(id));
            return;
        }

        _flaggedForDeletion.push_back(id);
    }

    inline std::vector<Entity> Registry::getFlaggedEntities()
    {
        return _flaggedForDeletion;
    }

    inline void Registry::deleteFlaggedEntities()
    {
        for (Entity &id : _flaggedForDeletion) {
            ComponentMask &mask = getComponentMask(id);
            removeComponentMask(mask, id);

            for (std::size_t i = 0; i < MAX_COMPONENTS; i++)
                if (mask[i] == 1)
                    _componentPools[i]->remove(id);

            untagEntity(id);
            unsetEntityLayer(id);
            _componentMasks.remove(id);
            _availableEntities.push_back(id);
            id = NULL_ENTITY;
        }

        _flaggedForDeletion.clear();
    }

    inline void Registry::tagEntity(const Entity id, const std::string &tag)
    {
        if (_tags.contains(tag))
            throw ECSException("Unable to tag Entity '" + std::to_string(id) + "' : Tag already exists");
        _tags[tag] = id;
    }

    inline void Registry::untagEntity(const Entity id)
    {
        for (auto &[tag, entity] : _tags)
            if (entity == id) {
                _tags.erase(tag);
                return;
            }
        // LOG_WARNING("Unable to untag Entity '{}' : Tag not found", std::to_string(id));
    }

    inline const std::string &Registry::getTagByEntity(const Entity id)
    {
        for (auto &[tag, entity] : _tags)
            if (entity == id)
                return tag;

        throw ECSException("Unable to get tag by Entity '" + std::to_string(id) + "' : Tag not found");
    }

    inline Entity Registry::getEntityByTag(const std::string &tag)
    {
        if (!_tags.contains(tag))
            return NULL_ENTITY;
        return _tags[tag];
    }

    inline void Registry::setEntityLayer(const Entity id, const std::size_t layer)
    {
        _layers[layer].push_back(id);
    }

    inline void Registry::unsetEntityLayer(const Entity id)
    {
        for (auto &entities : _layers | std::views::values)
            std::erase(entities, id);
    }

    inline std::vector<Entity> Registry::getEntitiesByLayer(const std::size_t layer)
    {
        if (!_layers.contains(layer))
            throw ECSException("Unable to get Entities by layer '" + std::to_string(layer) + "' : Layer not found");
        return _layers[layer];
    }

    /*
    ** COMPONENTS
    */

    template <typename T>
    void Registry::add()
    {
        if (_componentPools.size() >= MAX_COMPONENTS)
            throw ECSException("Unable to add Component '" + std::string(TYPENAME) + "' : Components limit exceeded");
        if (_componentBitPosition.contains(TYPENAME))
            throw ECSException("Unable to add Component '" + std::string(TYPENAME) + "' : Component already registered");

        _componentBitPosition[TYPENAME] = _componentPools.size();
        _componentPools.push_back(std::make_unique<SparseSet<T>>());
    }

    template <typename T>
    T &Registry::set(Entity id, T &&component)
    {
        if (!isValidEntity(id))
            throw ECSException("Unable to set Component '" + std::string(TYPENAME) + "' to Entity '" + std::to_string(id) +
                               "' : Invalid id");

        SparseSet<T> &pool = getComponentPool<T>();

        if (pool.get(id))
            return *pool.set(id, std::forward<T>(component));

        ComponentMask &mask = getComponentMask(id);

        removeComponentMask(mask, id);
        setComponentBit<T>(mask, 1);
        setComponentMask(mask, id);

        return *pool.set(id, std::forward<T>(component));
    }

    template <typename T>
    T &Registry::tryGet(const std::string &tag)
    {
        const Entity id = getEntityByTag(tag);

        if (!isValidEntity(id))
            throw ECSException("Unable to get Component '" + std::string(TYPENAME) + "' from Entity '" + tag + "' : Invalid id");

        if (!has<T>(id))
            throw ECSException("Unable to get Component '" + std::string(TYPENAME) + "' from Entity '" + tag + "' : Component not found");

        return get<T>(id);
    }

    template <typename T>
    T &Registry::tryGet(const Entity id)
    {
        if (!isValidEntity(id))
            throw ECSException("Unable to get Component '" + std::string(TYPENAME) + "' from Entity '" + std::to_string(id) +
                               "' : Invalid id");

        if (!has<T>(id))
            throw ECSException("Unable to get Component '" + std::string(TYPENAME) + "' from Entity '" + std::to_string(id) +
                               "' : Component not found");

        return get<T>(id);
    }

    template <typename... Ts>
    std::tuple<Ts &...> Registry::tryGetMany(const std::string &tag)
    {
        const Entity id = getEntityByTag(tag);

        if (!isValidEntity(id))
            throw ECSException("Unable to get multiple Components from Entity '" + tag + "' : Invalid id");

        if (!hasMany<Ts...>(id))
            throw ECSException("Unable to get multiple Components from Entity '" + tag + "' : Component not found");

        return std::make_tuple(get<Ts>(id)...);
    }

    template <typename... Ts>
    std::tuple<Ts &...> Registry::tryGetMany(const Entity id)
    {
        if (!isValidEntity(id))
            throw ECSException("Unable to get multiple Components from Entity '" + std::to_string(id) + "' : Invalid id");

        if (!hasMany<Ts...>(id))
            throw ECSException("Unable to get multiple Components from Entity '" + std::to_string(id) + "' : Component not found");

        return std::make_tuple(get<Ts>(id)...);
    }

    template <typename T>
    T &Registry::get(Entity id)
    {
        SparseSet<T> &pool = getComponentPool<T>();
        T *component = pool.get(id);

        return *component;
    }

    template <typename T>
    void Registry::remove(Entity id)
    {
        if (!isValidEntity(id))
            throw ECSException("Unable to remove Component '" + std::string(TYPENAME) + "' from Entity '" + std::to_string(id) +
                               "' : Invalid id");

        SparseSet<T> &pool = getComponentPool<T>();
        ComponentMask &mask = getComponentMask(id);

        removeComponentMask(mask, id);
        setComponentBit<T>(mask, 0);
        setComponentMask(mask, id);

        pool.remove(id);
    }

    template <typename T>
    bool Registry::has(const Entity id)
    {
        return getComponentBit<T>(getComponentMask(id));
    }

    template <typename... Ts>
    bool Registry::hasMany(const Entity id)
    {
        return (has<Ts>(id) && ...);
    }

    template <typename... Ts>
    std::vector<std::tuple<Entity, Ts &...>> Registry::getAllByComponents()
    {
        std::vector<std::tuple<Entity, Ts &...>> result;
        const ComponentMask targetMask = createMask<Ts...>();

        for (auto &[mask, ids] : _groupings) {
            if ((mask & targetMask) == targetMask) {
                for (Entity id : ids.getDense()) {
                    result.emplace_back(id, get<Ts>(id)...);
                }
            }
        }

        return result;
    }

    template <typename... Ts, typename Func>
    void Registry::forEach(Func &&func)
    {
        const ComponentMask targetMask = createMask<Ts...>();

        for (auto &[mask, ids] : _groupings) {
            if ((mask & targetMask) == targetMask) {
                for (Entity id : ids.getDense()) {
                    if constexpr (std::is_invocable_v<Func, Ts &...>) {
                        func(get<Ts>(id)...);
                    } else if constexpr (std::is_invocable_v<Func, Entity, Ts &...>) {
                        func(id, get<Ts>(id)...);
                    } else {
                        throw ECSException("Bad lambda provided to .forEach(), parameter pack does not match lambda args");
                    }
                }
            }
        }
    }

    inline void Registry::clear()
    {
        _availableEntities.clear();
        _flaggedForDeletion.clear();
        _componentMasks.clear();
        _groupings.clear();
        _componentPools.clear();
        _maxEntity = 0;
    }

    template <typename... Ts>
    ComponentMask Registry::createMask()
    {
        ComponentMask mask;
        (setComponentBit<Ts>(mask, 1), ...);

        return mask;
    }

    inline ComponentMask &Registry::getComponentMask(const Entity id)
    {
        ComponentMask *mask = _componentMasks.get(id);

        if (!mask)
            throw ECSException("Unable to get Entity '" + std::to_string(id) + "' mask : Invalid Entity id");

        return *mask;
    }

    inline void Registry::setComponentMask(ComponentMask &mask, const Entity id)
    {
        if (mask.none())
            return;

        _groupings.emplace(std::piecewise_construct, std::forward_as_tuple(mask), std::forward_as_tuple());

        _groupings[mask].set(id, id);
    }

    inline void Registry::removeComponentMask(const ComponentMask &mask, const Entity id)
    {
        if (mask.none())
            return;

        SparseSet<Entity> &group = getEntitiesByMask(mask);
        group.remove(id);

        if (group.isEmpty())
            _groupings.erase(mask);
    }

    inline SparseSet<Entity> &Registry::getEntitiesByMask(const ComponentMask &mask)
    {
        if (!_groupings.contains(mask))
            throw ECSException("Unable to get Entities by mask : Mask not found");

        return _groupings[mask];
    }

    template <typename T>
    ComponentMask::reference Registry::getComponentBit(ComponentMask &mask)
    {
        const std::size_t bitPos = getComponentBitPosition<T>();

        return mask[bitPos];
    }

    template <typename T>
    void Registry::setComponentBit(ComponentMask &mask, const bool val)
    {
        const std::size_t bitPos = getComponentBitPosition<T>();

        mask[bitPos] = val;
    }

    template <typename T>
    std::size_t Registry::getComponentBitPosition()
    {
        if (!_componentBitPosition.contains(TYPENAME))
            throw ECSException("Unable to get Component bit position '" + std::string(TYPENAME) + "' : Unregistered component");

        return _componentBitPosition[TYPENAME];
    }

    template <typename T>
    SparseSet<T> &Registry::getComponentPool()
    {
        const std::size_t bitPos = getComponentBitPosition<T>();

        return *static_cast<SparseSet<T> *>(_componentPools[bitPos].get());
    }

    inline bool Registry::isValidEntity(const Entity id)
    {
        return id != NULL_ENTITY && id < _maxEntity && _componentMasks.get(id) != nullptr;
    }

    inline bool Registry::isValidEntity(const std::string &tag) const
    {
        if (_tags.contains(tag))
            return true;
        return false;
    }
} // namespace ecs

#endif // REGISTRY_HPP

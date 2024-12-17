/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SparseSet
*/

#ifndef SPARSESET_HPP
#define SPARSESET_HPP

#include <sstream>
#include <vector>
#include "Entity.hpp"

namespace ecs {

    /**
     * @brief Interface class for a sparse set.
     *
     * Defines basic operations for managing entities within a sparse set,
     * including methods for removal and clearing. This is an abstract base class.
     */
    class ISparseSet {
        public:
            virtual ~ISparseSet() = default;

            /**
             * @brief Remove the specified entity from the set.
             * @param entity The entity to remove.
             */
            virtual void remove(Entity entity) = 0;

            /**
             * @brief Clear all entities from the set.
             */
            virtual void clear() = 0;

            /**
             * @brief Get the number of entities in the set.
             * @return The current number of entities in the set.
             */
            virtual std::size_t size() = 0;
    };

    /**
     * @brief A templated sparse set implementation that maps Entity IDs to data of type T.
     *
     * This class provides an efficient mapping from entity IDs to dense storage
     * for data, where entities are mapped to indices in a continuous array.
     *
     * @tparam T The type of data associated with each entity.
     */
    template <typename T>
    class SparseSet final: public ISparseSet {
        public:
            /**
             * @brief Default constructor for SparseSet, reserves space for dense data.
             */
            SparseSet();

            /**
             * @brief Adds or updates data associated with the specified entity.
             *
             * If the entity already exists in the set, updates its associated data.
             * Otherwise, inserts the new data.
             *
             * @param id The entity to associate with the data.
             * @param obj The data to associate.
             * @return Pointer to the inserted or updated data.
             */
            T *set(Entity id, T obj);

            /**
             * @brief Retrieves the data associated with a given entity.
             *
             * @param id The entity whose data is to be retrieved.
             * @return Pointer to the data, or nullptr if the entity is not in the set.
             */
            T *get(Entity id);

            /**
             * @brief Removes the data associated with the specified entity.
             *
             * Replaces the removed entity's data with the last element in the dense array
             * to keep data contiguous.
             *
             * @param id The entity to remove.
             */
            void remove(Entity id) override;

            /**
             * @brief Returns the number of entities currently in the dense array.
             *
             * @return The number of entities in the dense array.
             */
            std::size_t size() override;

            /**
             * @brief Clears all entities and associated data from the sparse set.
             */
            void clear() override;

            /**
             * @brief Checks whether the sparse set is empty.
             *
             * @return True if the set contains no entities, false otherwise.
             */
            [[nodiscard]] bool isEmpty() const;

            /**
             * @brief Gets a constant reference to the dense storage vector.
             *
             * @return Const reference to the dense array containing data associated with entities.
             */
            const std::vector<T> &getDense() const;

        private:
            using Sparse = std::vector<std::size_t>;

            /// @brief Vector of sparse pages, each storing mappings from entity IDs to dense array indices.
            std::vector<Sparse> _sparsePages;
            /// @brief Dense vector that stores data associated with entities in contiguous memory.
            std::vector<T> _dense;
            /// @brief Maps indices in the dense array to their respective entity IDs.
            std::vector<Entity> _denseToEntity;

            /// @brief Defines the maximum size for a single sparse page to prevent excessive memory use.
            const std::size_t SPARSE_MAX_SIZE = 1000;

            /**
             * @brief Maps an entity to a specific index in the dense array.
             *
             * This method sets the sparse-to-dense mapping without inserting into the dense array.
             *
             * @param id The entity to map.
             * @param index The index in the dense array to map to.
             */
            void setDenseIndex(Entity id, std::size_t index);

            /**
             * @brief Retrieves the dense array index for a given entity.
             *
             * @param id The entity to look up.
             * @return The index in the dense array, or NULL_ENTITY if the entity does not exist.
             */
            std::size_t getDenseIndex(Entity id) const;
    };

    template <typename T>
    SparseSet<T>::SparseSet()
    {
        _dense.reserve(100); // Reserving initial space for dense array
    }

    template <typename T>
    T *SparseSet<T>::set(const Entity id, T obj)
    {
        std::size_t index = getDenseIndex(id);
        if (index != NULL_ENTITY) { // If entity exists, update its data
            _dense[index] = obj;
            _denseToEntity[index] = id;
            return &_dense[index];
        }

        setDenseIndex(id, _dense.size());
        _dense.push_back(obj);
        _denseToEntity.push_back(id);

        return &_dense.back();
    }

    template <typename T>
    T *SparseSet<T>::get(const Entity id)
    {
        std::size_t index = getDenseIndex(id);
        return index != NULL_ENTITY ? &_dense[index] : nullptr;
    }

    template <typename T>
    void SparseSet<T>::remove(const Entity id)
    {
        std::size_t removedIndex = getDenseIndex(id);

        if (_dense.empty() || removedIndex == NULL_ENTITY)
            return;

        // Replace removed element with last element to keep dense array contiguous
        setDenseIndex(_denseToEntity.back(), removedIndex);
        setDenseIndex(id, NULL_ENTITY);

        std::swap(_dense.back(), _dense[removedIndex]);
        std::swap(_denseToEntity.back(), _denseToEntity[removedIndex]);

        _dense.pop_back();
        _denseToEntity.pop_back();
    }

    template <typename T>
    std::size_t SparseSet<T>::size()
    {
        return _dense.size();
    }

    template <typename T>
    void SparseSet<T>::clear()
    {
        _dense.clear();
        _sparsePages.clear();
        _denseToEntity.clear();
    }

    template <typename T>
    bool SparseSet<T>::isEmpty() const
    {
        return _dense.empty();
    }

    template <typename T>
    const std::vector<T> &SparseSet<T>::getDense() const
    {
        return _dense;
    }

    template <typename T>
    void SparseSet<T>::setDenseIndex(Entity id, std::size_t index)
    {
        const std::size_t page = id / SPARSE_MAX_SIZE;
        const std::size_t sparseIndex = id % SPARSE_MAX_SIZE;

        if (page >= _sparsePages.size())
            _sparsePages.resize(page + 1);

        Sparse &sparse = _sparsePages[page];
        if (sparseIndex >= sparse.size())
            sparse.resize(sparseIndex + 1, NULL_ENTITY);

        sparse[sparseIndex] = index;
    }

    template <typename T>
    std::size_t SparseSet<T>::getDenseIndex(const Entity id) const
    {
        const std::size_t page = id / SPARSE_MAX_SIZE;
        const std::size_t sparseIndex = id % SPARSE_MAX_SIZE;

        if (page < _sparsePages.size()) {
            const Sparse &sparse = _sparsePages[page];
            if (sparseIndex < sparse.size())
                return sparse[sparseIndex];
        }

        return NULL_ENTITY;
    }
} // namespace ecs

#endif // SPARSESET_HPP

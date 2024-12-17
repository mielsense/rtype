#include "scenes_network/ISceneNetwork.hpp"

ISceneNetwork::ISceneNetwork(ecs::Registry &registry, std::unique_ptr<tetriq::ITransportLayer> &transport)
    : _transport(transport)
    , _registry(registry)
{}

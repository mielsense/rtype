find_package(fmt CONFIG REQUIRED)

add_library(r-type_common
        debug/src/Logging.cpp
        debug/src/Tickable.cpp
        debug/src/Profiler.cpp
        ecs/src/systems/animationSystem.cpp
        ecs/src/systems/backgroundSystem.cpp
        ecs/src/systems/cameraSystem.cpp
        ecs/src/systems/cleanupSystem.cpp
        ecs/src/systems/collisionSystem.cpp
        ecs/src/systems/controlAnimationStateSystem.cpp
        ecs/src/systems/controlMoveSystem.cpp
        ecs/src/systems/controlShootSystem.cpp
        ecs/src/systems/hitboxSystem.cpp
        ecs/src/systems/playerSystem.cpp
        ecs/src/systems/positionSystem.cpp
        ecs/src/systems/renderSystem.cpp
        ecs/src/systems/UIrenderSystem.cpp
        ecs/src/systems/UISystem.cpp
        ecs/include/components/ScoreComponent.hpp
        ecs/src/systems/scoreSystem.cpp
        ecs/src/systems/shootDamageSystem.cpp
		ecs/src/systems/shootLifetimeSystem.cpp
        map/src/MapHandler.cpp
)

target_include_directories(r-type_common
        PUBLIC debug/include
        PUBLIC ecs/include
        PUBLIC ecs/include/components
	PUBLIC map/include
)

target_link_libraries(r-type_common PUBLIC fmt::fmt)
target_link_libraries(r-type_common PUBLIC r-type_network)

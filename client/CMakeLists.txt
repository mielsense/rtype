find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(r-type_client
        src/main.cpp
        src/core/Core.cpp
        src/display/SceneManager.cpp
        src/display/scenes/Game.cpp
        src/editor/Editor.cpp
        src/display/scenes/Menu.cpp
	    src/scenes_network/ISceneNetwork.cpp
	    src/scenes_network/GameNetwork.cpp
	    src/scenes_network/MenuNetwork.cpp
		src/display/scenes/Empty.cpp
		src/utils/Waitable.cpp
		src/factories/EntityFactory.cpp
		src/factories/EntityFactory.cpp
		include/utils/ConnectionConfig.hpp
)

target_include_directories(r-type_client
        PRIVATE include
)

target_link_libraries(r-type_client PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)
target_link_libraries(r-type_client PRIVATE ImGui-SFML::ImGui-SFML)
target_link_libraries(r-type_client PRIVATE fmt::fmt)

target_link_libraries(r-type_client PRIVATE r-type_common)
target_link_libraries(r-type_client PRIVATE r-type_network)

if(APPLE)
    target_link_libraries(r-type_client PRIVATE ${COREAUDIO_LIBRARY} ${AUDIOUNIT_LIBRARY})
endif()

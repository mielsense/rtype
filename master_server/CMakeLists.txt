find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

add_executable(r-type_master_server
        src/main.cpp
        src/MasterServer/MasterServer.cpp
)

target_include_directories(r-type_master_server
        PRIVATE include
)

target_link_libraries(r-type_master_server PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(r-type_master_server PRIVATE ImGui-SFML::ImGui-SFML)
target_link_libraries(r-type_master_server PRIVATE r-type_network r-type_common)

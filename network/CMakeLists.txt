file(GLOB_RECURSE NETWORK_SRC CONFIGURE_DEPENDS "${R-Type_SOURCE_DIR}/network/src/*.cpp")

add_library(r-type_network ${NETWORK_SRC})

find_package(unofficial-enet CONFIG REQUIRED)

target_include_directories(r-type_network
        PUBLIC "${R-Type_SOURCE_DIR}/network/include"
)

target_link_libraries(r-type_network PRIVATE unofficial::enet::enet)
target_link_libraries(r-type_network PUBLIC r-type_common)

if(MSVC)
else()
    target_compile_options(r-type_network
            PRIVATE -Wno-unused-parameter)
    add_compile_options(-Wall -Wextra)
endif()


# add_subdirectory(tests)

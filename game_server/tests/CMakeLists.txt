find_package(GTest CONFIG REQUIRED)

add_executable(r-type_server_tests
    test_dummy_client.cpp
    DummyClient.cpp
)

target_link_libraries(r-type_server_tests PRIVATE r-type_network)
target_link_libraries(r-type_server_tests PRIVATE GTest::gtest GTest::gtest_main)

add_test(ServerTests r-type_server_tests)

find_package(GTest CONFIG REQUIRED)

add_executable(r-type_network_tests
    test_stream.cpp
    test_packets_stream.cpp
    test_network_double.cpp
)

target_link_libraries(r-type_network_tests PRIVATE r-type_network)
target_link_libraries(r-type_network_tests PRIVATE GTest::gtest GTest::gtest_main)

add_test(NetworkTests r-type_network_tests)

add_custom_target(
    r-type_dev_docs
    "/usr/bin/env mdbook build ${CMAKE_CURRENT_SOURCE_DIR} --dest-dir ${CMAKE_CURRENT_BINARY_DIR}"
    SOURCES
    src/network_library.md
    src/network.md
    src/network_protocol.md
    src/SUMMARY.md
)

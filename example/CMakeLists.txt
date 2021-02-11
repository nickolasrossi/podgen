set(PODGEN_INCLUDES "${CMAKE_BINARY_DIR}/include")

capnp_generate_pod_cpp(example_pod include.capnp test.capnp)

add_executable(example EXCLUDE_FROM_ALL main.cpp)
target_link_libraries(example PRIVATE example_pod)
cmake_minimum_required(VERSION 3.9.4)
project(podgen)

find_package(CapnProto REQUIRED)
find_library(CAPNPC_LIBRARY capnpc)

capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS include/pod.capnp)

add_executable(podgen
	src/main.cpp
	src/PodGenerator.cpp
	src/PodGenerator.hpp
	src/reserved_words.hpp
	src/schema.cpp
	src/schema.hpp
	${CAPNP_SRCS})

target_compile_features(podgen PRIVATE cxx_std_17)

include_directories(podgen ${CAPNP_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/include)

target_link_libraries(podgen ${CAPNP_LIBRARIES} ${CAPNPC_LIBRARY})

file(GLOB include_files "${CMAKE_CURRENT_SOURCE_DIR}/include/*")
add_custom_target(copy_templates
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${include_files} ${CMAKE_CURRENT_BINARY_DIR}/include
)
add_dependencies(podgen copy_templates)

add_subdirectory(example)

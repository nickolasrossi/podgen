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

find_library(CCTZ_LIB cctz REQUIRED)
find_path(CCTZ_INCLUDE cctz/time_zone.h REQUIRED)

function(capnp_generate_pod_cpp target_name)
    set(base_dir "${CMAKE_CURRENT_BINARY_DIR}/pod_autogen")

    set(CAPNPC_IMPORT_DIRS "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include")
    set(CAPNPC_OUTPUT_DIR "${base_dir}")
    capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${ARGN})
    set(outputs)
    foreach (src ${ARGN})
        string(REGEX REPLACE "\\.capnp$" "" src_base "${src}")
        list(APPEND outputs "${base_dir}/${src_base}.pod.hpp")
        list(APPEND outputs "${base_dir}/${src_base}.convert.hpp" "${base_dir}/${src_base}.convert.cpp")
    endforeach ()
    add_custom_command(OUTPUT ${outputs}
            COMMAND podgen ${ARGN} -c "${CAPNP_INCLUDE_DIRECTORY}" -t "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include" -o "${base_dir}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            DEPENDS ${ARGN}
            VERBATIM)

    add_library(${target_name} STATIC EXCLUDE_FROM_ALL ${outputs} ${CAPNP_SRCS})
    target_include_directories(${target_name} PUBLIC "${base_dir}" "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include" ${CCTZ_INCLUDE})
    target_link_libraries(${target_name} PUBLIC CapnProto::capnp ${CCTZ_LIB})
    target_compile_features(${target_name} PUBLIC cxx_std_17)
endfunction()

add_subdirectory(example)
add_subdirectory(test)

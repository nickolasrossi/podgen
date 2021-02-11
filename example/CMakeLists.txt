set(PODGEN_INCLUDES "${CMAKE_BINARY_DIR}/include")

function(compile_cpp_pod target_name)
    set(base_dir "${CMAKE_CURRENT_BINARY_DIR}")

    set(CAPNPC_IMPORT_DIRS "${PODGEN_INCLUDES}")
    set(CAPNPC_OUTPUT_DIR "${base_dir}")
    capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${ARGN})
    set(outputs)
    foreach(src ${ARGN})
        string(REGEX REPLACE "\\.capnp$" "" src_base "${src}")
        list(APPEND outputs "${base_dir}/${src_base}.pod.hpp")
        list(APPEND outputs "${base_dir}/${src_base}.convert.hpp" "${base_dir}/${src_base}.convert.cpp")
    endforeach()
    add_custom_command(OUTPUT ${outputs}
            COMMAND podgen ${ARGN} -c "${CAPNP_INCLUDE_DIRECTORY}" -t "${PODGEN_INCLUDES}" -o "${base_dir}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            DEPENDS ${ARGN}
            VERBATIM)

    add_library(${target_name} STATIC EXCLUDE_FROM_ALL ${outputs} ${CAPNP_SRCS})
    target_include_directories(${target_name} PUBLIC "${base_dir}" "${CMAKE_BINARY_DIR}/include")
    target_link_libraries(${target_name} PUBLIC CapnProto::capnp)
    target_compile_features(${target_name} PUBLIC cxx_std_17)
endfunction()

compile_cpp_pod(example_pod include.capnp test.capnp)

add_executable(example EXCLUDE_FROM_ALL main.cpp)
target_link_libraries(example PRIVATE example_pod)
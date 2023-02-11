cmake_minimum_required(VERSION 3.5.1)

project(afs C CXX)

include(cmake/common.cmake)
include(CheckFunctionExists)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Proto file
get_filename_component(hw_proto "afs.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "./afs.pb.cc")
set(hw_proto_hdrs "./afs.pb.h")
set(hw_grpc_srcs "./afs.grpc.pb.cc")
set(hw_grpc_hdrs "./afs.grpc.pb.h")
add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "."
        --cpp_out "."
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories(".")

# hw_grpc_proto
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})


set(UNRELIABLEFS_SRC afs_client.cc wrapper.cc  conf.c unreliablefs.c unreliablefs_errinj.c unreliablefs_ops.c )

find_package(FUSE 2.9 REQUIRED)

add_executable(client ${UNRELIABLEFS_SRC})
target_link_libraries(client 
hw_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
${FUSE_LIBRARIES})

target_compile_options(client PUBLIC -D_FILE_OFFSET_BITS=64 -Wall -Wextra -Wno-unused-parameter)
target_compile_options(client PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++11 -D_FILE_OFFSET_BITS=64>)

check_function_exists(utimensat HAVE_UTIMENSAT)
check_function_exists(fallocate HAVE_FALLOCATE)
check_function_exists(fallocate HAVE_FLOCK)
if (${HAVE_UTIMENSAT})
  target_compile_definitions(client PUBLIC HAVE_UTIMENSAT)
endif ()
if (${HAVE_FALLOCATE})
    target_compile_definitions(client PUBLIC HAVE_FALLOCATE)
endif ()
if (${HAVE_FLOCK})
    target_compile_definitions(client PUBLIC HAVE_FLOCK)
endif ()


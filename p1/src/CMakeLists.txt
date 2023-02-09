cmake_minimum_required(VERSION 3.10.2)
project (AFSv1 C CXX)

include(CheckFunctionExists)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(UNRELIABLEFS_SRC conf.c unreliablefs.c unreliablefs_errinj.c unreliablefs_ops.c)

find_package(FUSE 2.9 REQUIRED)

add_executable(client ${UNRELIABLEFS_SRC})
target_link_libraries(client ${FUSE_LIBRARIES})

target_compile_options(client PUBLIC -D_FILE_OFFSET_BITS=64 -Wall -Wextra -Wno-unused-parameter)

check_function_exists(utimensat HAVE_UTIMENSAT)
if (${HAVE_UTIMENSAT})
  target_compile_definitions(client PUBLIC HAVE_UTIMENSAT)
endif ()
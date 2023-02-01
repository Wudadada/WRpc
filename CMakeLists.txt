cmake_minimum_required(VERSION 3.0)
project(wqd)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/yaml-cpp)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic  -fPIC -ggdb -O3 -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic  -fPIC -ggdb -O3 -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include_directories(.)
include_directories(/apps/wqd/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/yaml-cpp/include)
link_directories(/apps/wqd/lib)
link_directories(/apps/wqd/lib64)

option(BUILD_TEST "ON for complile test" OFF)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()


set(LIB_SRC
    wqd/address.cc
    wqd/bytearray.cc
    wqd/config.cc
    wqd/env.cc
    wqd/fd_manager.cc
    wqd/fiber.cc
    wqd/http/http.cc
    wqd/http/http_connection.cc
    wqd/http/http_parser.cc
    wqd/http/http_session.cc
    wqd/http/http_server.cc
    wqd/http/servlet.cc
    wqd/hook.cc
    wqd/iomanager.cc
    wqd/library.cc
    wqd/log.cc
    wqd/mutex.cc
    wqd/protocol.cc
    wqd/scheduler.cc
    wqd/socket.cc
    wqd/stream.cc
    wqd/tcp_server.cc
    wqd/timer.cc
    wqd/thread.cc
    wqd/util.cc
    )

ragelmaker(wqd/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/wqd/http)
ragelmaker(wqd/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/wqd/http)
ragelmaker(wqd/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/wqd)

add_library(wqd SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(wqd)
find_library(PTHREAD pthread)


set(LIBS
        wqd
        pthread
        yaml-cpp
        )

if(BUILD_TEST)
wqd_add_executable(my_http_server "samples/my_http_server.cc" wqd "${LIBS}")

endif()
wqd_add_executable(test_http_server "tests/test_http_server.cc" wqd "${LIBS}")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

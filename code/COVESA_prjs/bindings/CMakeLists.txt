cmake_minimum_required(VERSION 2.8)
project(COVESA_BINDINGS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++0x -Wl,--no-as-needed")
set(CommonAPI_Version "3.2.0")
set(vsomeip_Version "3.1.20")

set(RUNTIME_PATH "/home/xxx/workspace")
# set(COMMONAPI_INCLUDE_DIRS "${RUNTIME_PATH}/capicxx-core-runtime/include")
# set(COMMONAPI_DBUS_INCLUDE_DIRS "${RUNTIME_PATH}/capicxx-dbus-runtime/include")
# set(COMMONAPI_SOMEIP_INCLUDE_DIRS "${RUNTIME_PATH}/capicxx-someip-runtime/include")
# set(VSOMEIP_INCLUDE_DIRS "${RUNTIME_PATH}/vsomeip/interface")

set(DBus_INCLUDE_DIRS "${RUNTIME_PATH}/dbus-1.10.10")
set(PROJ_ROOT_FOLDER "${PROJECT_SOURCE_DIR}")


MESSAGE(PROJECT_BINARY_DIR "=${PROJECT_BINARY_DIR}")
MESSAGE(PROJECT_SOURCE_DIR "=${PROJECT_SOURCE_DIR}")

include(FindPkgConfig)
find_package(CommonAPI ${CommonAPI_Version} REQUIRED CONFIG NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_package(CommonAPI-DBus ${CommonAPI_Version} REQUIRED CONFIG NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_package(CommonAPI-SomeIP ${CommonAPI_Version} REQUIRED)
find_package(vsomeip3 ${vsomeip_Version} REQUIRED)
# pkg_check_modules(DBus REQUIRED dbus-1>=1.10)

include_directories(
     src-gen/core/common
     src-gen/core/proxy
     src-gen/core/stub
     src-gen/core/skel
     src-gen/dbus/common
     src-gen/dbus/proxy
     src-gen/dbus/stub
     src-gen/someip/common
     src-gen/someip/proxy
     src-gen/someip/stub
     ${COMMONAPI_INCLUDE_DIRS}
     ${COMMONAPI_DBUS_INCLUDE_DIRS}
     ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
     ${DBus_INCLUDE_DIRS}
     ${VSOMEIP3_INCLUDE_DIRS}
)

link_directories(
    ${PROJ_ROOT_FOLDER}/build
    ${DBus_INCLUDE_DIRS}/dbus/.libs
)

FILE(GLOB_RECURSE HELLOWORLD_SKEL_LIB_SRCS src-gen/core/skel/*)
add_library(helloworldskel SHARED ${HELLOWORLD_SKEL_LIB_SRCS})
set_target_properties(helloworldskel PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(helloworldskel)

FILE(GLOB_RECURSE HELLOWORLD_DBUS_COMMON_LIB_SRCS src-gen/dbus/common/*.cpp)
add_library(helloworlddbuscommon SHARED ${HELLOWORLD_DBUS_COMMON_LIB_SRCS})
target_link_libraries(helloworlddbuscommon CommonAPI-DBus dbus-1)

FILE(GLOB_RECURSE HELLOWORLD_DBUS_PROXY_LIB_SRCS src-gen/dbus/proxy/*.cpp)
add_library(helloworlddbusproxy SHARED ${HELLOWORLD_DBUS_PROXY_LIB_SRCS})
target_link_libraries(helloworlddbusproxy CommonAPI-DBus dbus-1)

FILE(GLOB_RECURSE HELLOWORLD_DBUS_STUB_LIB_SRCS src-gen/dbus/stub/*.cpp)
add_library(helloworlddbusstub SHARED ${HELLOWORLD_DBUS_STUB_LIB_SRCS})
target_link_libraries(helloworlddbusstub CommonAPI-DBus dbus-1)

FILE(GLOB_RECURSE HELLOWORLD_SOMEIP_COMMON_LIB_SRCS src-gen/someip/common/*.cpp)
add_library(helloworldsomeipcommon SHARED ${HELLOWORLD_SOMEIP_COMMON_LIB_SRCS})
target_link_libraries(helloworldsomeipcommon CommonAPI-SomeIP vsomeip3)

FILE(GLOB_RECURSE HELLOWORLD_SOMEIP_PROXY_LIB_SRCS src-gen/someip/proxy/*.cpp)
add_library(helloworldsomeipproxy SHARED ${HELLOWORLD_SOMEIP_PROXY_LIB_SRCS})
target_link_libraries(helloworldsomeipproxy CommonAPI-SomeIP vsomeip3)

FILE(GLOB_RECURSE HELLOWORLD_SOMEIP_STUB_LIB_SRCS src-gen/someip/stub/*.cpp)
add_library(helloworldsomeipstub SHARED ${HELLOWORLD_SOMEIP_STUB_LIB_SRCS})
target_link_libraries(helloworldsomeipstub CommonAPI-SomeIP vsomeip3)

add_executable(HelloWorldClient src/HelloWorldClient.cpp)
target_link_libraries(HelloWorldClient CommonAPI)

add_executable(HelloWorldService src/HelloWorldService.cpp src/HelloWorldStubImpl.cpp)
target_link_libraries(HelloWorldService CommonAPI helloworldskel)
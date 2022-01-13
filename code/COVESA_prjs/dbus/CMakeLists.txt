cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++0x")
set(RUNTIME_PATH "/home/dad2szh/samba/workspace/OSS")
set(DBUS_PATH "/home/dad2szh/samba/workspace/OSS/dbus-1.10.10")
include_directories(
    src-gen
    ${RUNTIME_PATH}/capicxx-core-runtime/include
    ${RUNTIME_PATH}/capicxx-dbus-runtime/include
    ${DBUS_PATH}
)
link_directories(
    ${RUNTIME_PATH}/capicxx-core-runtime/build
    ${RUNTIME_PATH}/capicxx-dbus-runtime/build
    ${DBUS_PATH}/dbus/.libs
)
add_executable(HelloWorldClient
	src/HelloWorldClient.cpp
	src-gen/v1/commonapi/HelloWorldDBusProxy.cpp
	src-gen/v1/commonapi/HelloWorldDBusDeployment
)
target_link_libraries(HelloWorldClient CommonAPI CommonAPI-DBus dbus-1)
add_executable(HelloWorldService
	src/HelloWorldService.cpp
	src/HelloWorldStubImpl.cpp
	src-gen/v1/commonapi/HelloWorldDBusStubAdapter.cpp
	src-gen/v1/commonapi/HelloWorldStubDefault
	src-gen/v1/commonapi/HelloWorldDBusDeployment
)
target_link_libraries(HelloWorldService CommonAPI CommonAPI-DBus dbus-1)
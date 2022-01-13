cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++0x")
set(RUNTIME_PATH "/home/dad2szh/samba/workspace/OSS")
include_directories(
    src-gen
    ${RUNTIME_PATH}/capicxx-core-runtime/include
    ${RUNTIME_PATH}/capicxx-someip-runtime/include
    ${RUNTIME_PATH}/vsomeip/interface
)
link_directories(
    ${RUNTIME_PATH}/capicxx-core-runtime/build
    ${RUNTIME_PATH}/capicxx-someip-runtime/build
    ${RUNTIME_PATH}/vsomeip/build
)
add_executable(HelloWorldClient
	src/HelloWorldClient.cpp
	src-gen/v1/commonapi/HelloWorldSomeIPProxy.cpp
	src-gen/v1/commonapi/HelloWorldSomeIPDeployment
)
target_link_libraries(HelloWorldClient CommonAPI CommonAPI-SomeIP vsomeip3)
add_executable(HelloWorldService
	src/HelloWorldService.cpp
	src/HelloWorldStubImpl.cpp
	src-gen/v1/commonapi/HelloWorldSomeIPStubAdapter.cpp
	src-gen/v1/commonapi/HelloWorldStubDefault
	src-gen/v1/commonapi/HelloWorldSomeIPDeployment
)
target_link_libraries(HelloWorldService CommonAPI CommonAPI-SomeIP vsomeip3)
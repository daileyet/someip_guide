cmake_minimum_required (VERSION 2.8)

set (CMAKE_CXX_FLAGS "-g -std=c++0x")

find_package(vsomeip3 3.1.20 REQUIRED)
find_package(Boost 1.78.0 COMPONENTS system thread log REQUIRED )

include_directories (
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
)

add_executable(service-example ../src/service_example.cpp)
target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})

add_executable(client-example ../src/client_example.cpp)
target_link_libraries(client-example vsomeip3 ${Boost_LIBRARIES})
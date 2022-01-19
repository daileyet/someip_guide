# CommonAPI C++ with D-Bus

## 前提准备

1.  CommonAPI 3.2.0
2.  Ubuntu 16
3.  Installed git and (CMake >=2.8)
4.  Java 8 runtime

可能需要安装如下软件包:

```shell
sudo apt install cmake cmake-qt-gui libexpat-dev expat default-jre
```

## 构建CommonAPI Runtime库

```shell
git clone --b 3.2.0 https://github.com/GENIVI/capicxx-core-runtime.git
```

构建CommonAPI Runtime库不需要任何外部依赖:

```shell
cd  ~/workspace/capicxx-core-runtime/
mkdir build && cd build
cmake ..
make
```

最终成功后会获得 `libCommonAPI.so`, 也可以将生成的库安装到其它目录

若需要生成文档, 请先安装 `doxygen` `sudo apt-get install doxygen`

```shell
cd  ~/workspace/capicxx-core-runtime/build
doxygen
```

## 构建CommonAPI D-Bus Runtime库

```shell
git clone -b 3.2.0 https://github.com/GENIVI/capicxx-dbus-runtime.git
```

`CommonAPI D-Bus Runtime`库构建需要依赖特定的版本的`libdbus`

```shell
cd  ~/workspace/
wget http://dbus.freedesktop.org/releases/dbus/dbus-1.10.10.tar.gz
tar -xzf dbus-1.10.10.tar.gz
cd dbus-1.10.10/
```

用`CommonAPI D-Bus Runtime`源码目录下的补丁打patch

```shell
for i in ../capicxx-dbus-runtime/src/dbus-patches/*.patch; do patch -p1 < $i; done
```

构建 `libdbus`

```shell
cd ~/workspace/dbus-1.10.10/
./configure
make
```

开始构建 `CommonAPI D-Bus Runtime`

```shell
cd  ~/workspace/capicxx-dbus-runtime
mkdir build && cd build
export PKG_CONFIG_PATH="~/workspace/dbus-1.10.10"
cmake -DUSE_INSTALLED_COMMONAPI=OFF -DUSE_INSTALLED_DBUS=OFF ..
make
```

最终成功后会获得 `libCommonAPI-DBus.so`

## 定义Franca接口文件和生成代码

```shell
cd ~/workspace && mkdir -p COVESA_prjs/dbus/fidl
cd COVESA_prjs/dbus/fidl
vi HelloWorld.fidl
```

```fidl
package commonapi

interface HelloWorld {
  version {major 1 minor 0}
  method sayHello {
    in {
      String name
    }
    out {
      String message
    }
  }
}

```

下载 [commonapi_core_generator](https://github.com/COVESA/capicxx-core-tools/releases/download/3.2.0.1/commonapi_core_generator.zip) & [commonapi_dbus_generator](https://github.com/COVESA/capicxx-dbus-tools/releases/download/3.2.0/commonapi_dbus_generator.zip) 后解压至 `~/workspace`

```shell
cd ~/workspace/COVESA_prjs/dbus
.~/workspace/commonapi_core_generator/commonapi-core-generator-windows-x86_64 -sk ./fidl/HelloWorld.fidl

.~/workspace/commonapi_dbus_generator/commonapi-dbus-generator-linux-x86_64 ./fidl/HelloWorld.fidl
```
一切顺利后会在  `fidl`的同级目录下生成 `src-gen`目录及目录里的文件

## 编写client和service程序

[HelloWorldClient.cpp](../code/COVESA_prjs/dbus/src/HelloWorldClient.cpp)

[HelloWorldService.cpp](../code/COVESA_prjs/dbus/src/HelloWorldService.cpp)

然后再编写service的实现

[HelloWorldStubImpl.hpp](../code/COVESA_prjs/dbus/src/HelloWorldStubImpl.hpp)

[HelloWorldStubImpl.cpp](../code/COVESA_prjs/dbus/src/HelloWorldStubImpl.cpp)

## 编译运行

```cmake
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++0x")
set(RUNTIME_PATH "/home/xxx/workspace")
set(DBUS_PATH "${RUNTIME_PATH}/dbus-1.10.10")
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

```

```shell
cd ~/workspace/COVESA_prjs/dbus && mkdir build && cd build
cmake ..
make
./HelloWorldService &
./HelloWorldClient
```

详见 [dbus sample](../code/COVESA_prjs/dbus/)
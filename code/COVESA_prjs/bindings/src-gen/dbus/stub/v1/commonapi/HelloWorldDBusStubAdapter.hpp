/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus 3.2.0.v202012010857.
* Used org.franca.core 0.13.1.201807231814.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V1_COMMONAPI_Hello_World_DBUS_STUB_ADAPTER_HPP_
#define V1_COMMONAPI_Hello_World_DBUS_STUB_ADAPTER_HPP_

#include <v1/commonapi/HelloWorldStub.hpp>
#include "v1/commonapi/HelloWorldDBusDeployment.hpp"

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#define HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif

#include <CommonAPI/DBus/DBusAddressTranslator.hpp>
#include <CommonAPI/DBus/DBusFactory.hpp>
#include <CommonAPI/DBus/DBusStubAdapterHelper.hpp>
#include <CommonAPI/DBus/DBusStubAdapter.hpp>
#include <CommonAPI/DBus/DBusDeployment.hpp>

#if defined (HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE)
#undef COMMONAPI_INTERNAL_COMPILATION
#undef HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif

namespace v1 {
namespace commonapi {

template <typename _Stub = ::v1::commonapi::HelloWorldStub, typename... _Stubs>
class HelloWorldDBusStubAdapterInternal
    : public virtual HelloWorldStubAdapter,
      public CommonAPI::DBus::DBusStubAdapterHelper< _Stub, _Stubs...>
     {
public:
    typedef CommonAPI::DBus::DBusStubAdapterHelper< _Stub, _Stubs...> HelloWorldDBusStubAdapterHelper;

    ~HelloWorldDBusStubAdapterInternal() {
        deactivateManagedInstances();
        HelloWorldDBusStubAdapterHelper::deinit();
    }

    virtual bool hasFreedesktopProperties() {
        return false;
    }

    inline static const char* getInterface() {
        return HelloWorld::getInterface();
    }

    void deactivateManagedInstances() {
    }

    CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v1::commonapi::HelloWorldStub,
        CommonAPI::Version
        > getHelloWorldInterfaceVersionStubDispatcher;

    
    CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
        ::v1::commonapi::HelloWorldStub,
        std::tuple< std::string>,
        std::tuple< std::string>,
        std::tuple< CommonAPI::DBus::StringDeployment>,
        std::tuple< CommonAPI::DBus::StringDeployment>
    
        > sayHelloStubDispatcher;

    HelloWorldDBusStubAdapterInternal(
        const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection,
        const std::shared_ptr<_Stub> &_stub)
    : CommonAPI::DBus::DBusStubAdapter(_address, _connection,false),
      HelloWorldDBusStubAdapterHelper(_address, _connection, false, _stub), 
      getHelloWorldInterfaceVersionStubDispatcher(&HelloWorldStub::lockInterfaceVersionAttribute, &HelloWorldStub::getInterfaceVersion, "uu", true),
      sayHelloStubDispatcher(&HelloWorldStub::sayHello, "s",
           _stub->hasElement(0), std::make_tuple(static_cast< CommonAPI::DBus::StringDeployment* >(nullptr)), std::make_tuple(static_cast< CommonAPI::DBus::StringDeployment* >(nullptr)))
    {
        HelloWorldDBusStubAdapterHelper::addStubDispatcher({ "sayHello", "s" }, &sayHelloStubDispatcher);
        HelloWorldDBusStubAdapterHelper::addStubDispatcher({ "getInterfaceVersion", "" }, &getHelloWorldInterfaceVersionStubDispatcher);
    }

protected:
    virtual const char* getMethodsDBusIntrospectionXmlData() const {
        static const std::string introspectionData =
            "<method name=\"getInterfaceVersion\">\n"
                "<arg name=\"value\" type=\"uu\" direction=\"out\" />"
            "</method>\n"
            "<method name=\"sayHello\">\n"
            "<arg name=\"_name\" type=\"s\" direction=\"in\" />\n"
            "<arg name=\"_message\" type=\"s\" direction=\"out\" />\n"
            "<annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"true\" />\n"
            "</method>\n"
            ;
        return introspectionData.c_str();
    }

private:
};



template <typename _Stub = ::v1::commonapi::HelloWorldStub, typename... _Stubs>
class HelloWorldDBusStubAdapter
    : public HelloWorldDBusStubAdapterInternal<_Stub, _Stubs...>,
      public std::enable_shared_from_this< HelloWorldDBusStubAdapter<_Stub, _Stubs...>> {
public:
    HelloWorldDBusStubAdapter(
        const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection,
        const std::shared_ptr<_Stub> &_stub)
        : CommonAPI::DBus::DBusStubAdapter(
            _address,
            _connection,
            false),
          HelloWorldDBusStubAdapterInternal<_Stub, _Stubs...>(
            _address,
            _connection,
            _stub) {
    }
};

} // namespace commonapi
} // namespace v1

#endif // V1_COMMONAPI_Hello_World_DBUS_STUB_ADAPTER_HPP_
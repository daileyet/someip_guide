#include <iostream>
#include <string>
#include <unistd.h>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/commonapi/HelloWorldProxy.hpp>

using namespace v1_0::commonapi;

int main()
{
    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();
    std::shared_ptr<HelloWorldProxy<>> myProxyDBus = runtime->buildProxy<HelloWorldProxy>("local", "testDBus");
    std::shared_ptr<HelloWorldProxy<>> myProxySomeIP = runtime->buildProxy<HelloWorldProxy>("local", "testSomeIP");
    if ((myProxyDBus == NULL) || (myProxySomeIP == NULL))
    {
        if (myProxyDBus == NULL)
        {
            std::cout << "No DBus proxy created." << std::endl;
        }
        if (myProxySomeIP == NULL)
        {
            std::cout << "No SomeIP proxy created." << std::endl;
        }
        return 0;
    }
    std::cout << "Checking availability!" << std::endl;
    while (!(myProxyDBus->isAvailable()) || !(myProxySomeIP->isAvailable()))
        usleep(10);
    std::cout << "Available..." << std::endl;

    CommonAPI::CallStatus callStatusDBus;
    std::string returnMessageDBus;
    myProxyDBus->sayHello("Bob DBus", callStatusDBus, returnMessageDBus);
    std::cout << "Got message: '" << returnMessageDBus << "'\n";

    CommonAPI::CallStatus callStatusSomeIP;
    std::string returnMessageSomeIP;
    myProxySomeIP->sayHello("Bob SomeIP", callStatusSomeIP, returnMessageSomeIP);
    std::cout << "Got message: '" << returnMessageSomeIP << "'\n";
    return 0;
}
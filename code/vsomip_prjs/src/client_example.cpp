#include <iomanip>
#include <iostream>
#include <sstream>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <vsomeip/vsomeip.hpp>
#include "common.hpp"

std::shared_ptr<vsomeip::application> app;
std::mutex mutex;
std::condition_variable condition;
std::atomic<bool> availability(false);

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);
void on_request(const std::shared_ptr<vsomeip::message> &_response);
void on_event(const std::shared_ptr<vsomeip::message> &_response);
void run();

int main()
{

    app = vsomeip::runtime::get()->create_application("Hello");
    app->init();
    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_request);
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, on_event);
    std::thread sender(run);
    app->start();
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available)
{
    std::cout << "Service ["
              << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
              << "] is " << (_is_available ? "available." : "NOT available.") << std::endl;
    availability.store(_is_available);
    if (_is_available)
    {
        condition.notify_one();
    }
}

void on_request(const std::shared_ptr<vsomeip::message> &_response)
{

    std::shared_ptr<vsomeip::payload> its_payload = _response->get_payload();
    vsomeip::length_t l = its_payload->get_length();

    // Get payload
    std::stringstream ss;
    for (vsomeip::length_t i = 0; i < l; i++)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex
           << (int)*(its_payload->get_data() + i) << " ";
    }

    std::cout << "CLIENT: Received message with Client/Session ["
              << std::setw(4) << std::setfill('0') << std::hex << _response->get_client() << "/"
              << std::setw(4) << std::setfill('0') << std::hex << _response->get_session() << "] "
              << ss.str() << std::endl;
}

void on_event(const std::shared_ptr<vsomeip::message> &_response)
{
    std::stringstream its_message;
    its_message << "CLIENT: received a notification for event ["
                << std::setw(4) << std::setfill('0') << std::hex
                << _response->get_service() << "."
                << std::setw(4) << std::setfill('0') << std::hex
                << _response->get_instance() << "."
                << std::setw(4) << std::setfill('0') << std::hex
                << _response->get_method() << "] to Client/Session ["
                << std::setw(4) << std::setfill('0') << std::hex
                << _response->get_client() << "/"
                << std::setw(4) << std::setfill('0') << std::hex
                << _response->get_session()
                << "] = ";
    std::shared_ptr<vsomeip::payload> its_payload = _response->get_payload();
    its_message << "(" << std::dec << its_payload->get_length() << ") ";
    for (uint32_t i = 0; i < its_payload->get_length(); ++i)
        its_message << std::hex << std::setw(2) << std::setfill('0')
                    << (int)its_payload->get_data()[i] << " ";
    std::cout << its_message.str() << std::endl;
}

void run()
{
    std::unique_lock<std::mutex> its_lock(mutex);
    condition.wait(its_lock);

    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(SAMPLE_EVENTGROUP_ID);
    app->request_event(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, its_groups);
    app->subscribe(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENTGROUP_ID);

    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_service(SAMPLE_SERVICE_ID);
    request->set_instance(SAMPLE_INSTANCE_ID);
    request->set_method(SAMPLE_METHOD_ID);

    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    for (vsomeip::byte_t i = 0; i < 10; i++)
    {
        its_payload_data.push_back(i % 256);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
    while (true)
    {
        if (availability.load())
            app->send(request);
        std::this_thread::sleep_for(std::chrono::milliseconds(8000));
    }
}
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <atomic>
#include <vsomeip/vsomeip.hpp>
#include "common.hpp"

std::shared_ptr<vsomeip::application> app;
std::atomic<bool> availability(false);

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);
void on_message(const std::shared_ptr<vsomeip::message> &_request);
void run();

int main()
{
    app = vsomeip::runtime::get()->create_application("World");
    app->init();
    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    app->register_message_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID, on_message);
    app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);

    std::set<vsomeip::eventgroup_t> its_groups;
    its_groups.insert(SAMPLE_EVENTGROUP_ID);
    app->offer_event(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, its_groups);
    std::thread sender(run);
    app->start();
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available)
{
    std::cout << "Service ["
              << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
              << "] is " << (_is_available ? "available." : "NOT available.") << std::endl;
    availability.store(_is_available);
}

void on_message(const std::shared_ptr<vsomeip::message> &_request)
{

    std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();
    vsomeip::length_t l = its_payload->get_length();

    // Get payload
    std::stringstream ss;
    for (vsomeip::length_t i = 0; i < l; i++)
    {
        ss << std::setw(2) << std::setfill('0') << std::hex
           << (int)*(its_payload->get_data() + i) << " ";
    }

    std::cout << "SERVICE: Received message with Client/Session ["
              << std::setw(4) << std::setfill('0') << std::hex << _request->get_client() << "/"
              << std::setw(4) << std::setfill('0') << std::hex << _request->get_session() << "] "
              << ss.str() << std::endl;

    // Create response
    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    for (int i = 9; i >= 0; i--)
    {
        its_payload_data.push_back(i % 256);
    }
    its_payload->set_data(its_payload_data);
    its_response->set_payload(its_payload);
    app->send(its_response);
}

void run()
{
    const vsomeip::byte_t its_data[] = {0x10};
    auto payload = vsomeip::runtime::get()->create_payload();
    payload->set_data(its_data, sizeof(its_data));
    while (true)
    {
        if (availability.load())
            app->notify(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, payload);
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}
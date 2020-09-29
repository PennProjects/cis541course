#include <iostream>
#include <string>
#include <mqtt/async_client.h>
#include <mqtt/connect_options.h>
#include <mqtt/ssl_options.h>

#include "vc2gps.h"

using namespace std;

/**
 * A callback class for use with the main MQTT client.
 */
class callback : public virtual mqtt::callback
{
public:
    void connection_lost(const std::string& cause) override {
        std::cout << "\nConnection lost" << std::endl;
        if (!cause.empty())
            std::cout << "\tcause: " << cause << std::endl;
    }

    void delivery_complete(mqtt::delivery_token_ptr tok) override {
        std::cout << "\tDelivery complete for token: "
                  << (tok ? tok->get_message_id() : -1) << std::endl;
    }
};


int main(int argc, char* argv[]) {

    double position = 50; // can take values from 0 to 100
    RoadIdentifier road = RoadIdentifier::BC;
    GPS_Coord c = GetLatLon(position, road);
//    printf("lat %.9f\n", c.lat);
//    printf("lon %.9f\n", c.lon);

    const string tb_uri = {"ssl://tb.precise.seas.upenn.edu:8883"};
    string client_id = {"client_id_1234"};
    string username = {"SHARED_DASHBOARD"};
    string password = {""};

    // Init Client
    cout << "Initializing for server '" << tb_uri << "'..." << endl;
    mqtt::async_client client(tb_uri, client_id);

    callback cb;
    client.set_callback(cb);

    // Username & Password
    mqtt::connect_options connopts(username, password);
    // Enable SSL
    mqtt::ssl_options sslopts;
    connopts.set_ssl(sslopts);

    // Perform connection
    client.connect(connopts)->wait();
    cout << "  ...OK" << endl;

    // publishing a message
    auto msg = mqtt::make_message("v1/gateway/connect",
                                  "{\"device\":\"8660\",\"type\":\"Assignment#4\"}", 1, false);
    client.publish(msg)->wait();

    auto msg2 = mqtt::make_message("v1/gateway/attributes",
                                  "{\"8660\":{\"latitude\":39.955861,\"longitude\":-75.191443}}", 1, false);
    client.publish(msg2)->wait();


}
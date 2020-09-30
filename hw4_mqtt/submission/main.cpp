//Standard header files for c++
#include <iostream>
#include <string>
#include <chrono>
#include "time.h"

//Header files for MQTT functions
#include <mqtt/async_client.h>
#include <mqtt/connect_options.h>
#include <mqtt/ssl_options.h>

//Header for GetLatLon
#include "vc2gps.h"

using namespace std;
using namespace std::chrono;



// A callback class for use with the main MQTT client.
class callback : public virtual mqtt::callback
{
public:
    void connection_lost(const std::string& cause) override {
        std::cout << "\nConnection lost" << std::endl;
        if (!cause.empty())
            std::cout << "\tcause: " << cause << std::endl;
    }
};




int main(int argc, char* argv[]) {

//  getting starting position of car at A
    double start_location = 0; // Start at 0
    RoadIdentifier road = RoadIdentifier::AB; //Select road AB
    GPS_Coord gps_start = GetLatLon(start_location, road);
    cout << "Starting Journey at A : " <<gps_start.lat<<","<<gps_start.lon<<endl;

//    setting loop wait time for updating location at 1s
//    the wait after sending mqtt message is set to 10ms,
//    based on repeated calculations of avg wait time, its about 10ms on codio
//    to compensate for that, the wait time here is set to 990 ms
    timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 990000000; //990 ms






//     Settting MQTT connection parameters
    const string tb_uri = {"ssl://tb.precise.seas.upenn.edu:8883"};
    string client_id = {"client_id_1234"};
    string username = {"SHARED_DASHBOARD"};
    string password = {""};

//     Init Client
    cout << "Initializing for server '" << tb_uri << "'..." << endl;
    mqtt::async_client client(tb_uri, client_id);

    callback cb;
    client.set_callback(cb);

//     Username & Password
    mqtt::connect_options connopts(username, password);
//     Enable SSL
    mqtt::ssl_options sslopts;
    connopts.set_ssl(sslopts);

//     Perform connection
    client.connect(connopts)->wait();
    cout << "Client connected" << endl;

//     publishing a message
    string mqtt_connection_topic = "v1/gateway/connect";
    string register_car_msg = "{\"device\":\"8660\",\"type\":\"Assignment#4\"}";

    auto register_msg = mqtt::make_message(mqtt_connection_topic, register_car_msg, 1, false);
    client.publish(register_msg)->wait();





//     Global variable for gps position
    GPS_Coord gps;

//     update position by 4 meters every 1s to have a drive speed of 4 m/s
    for (int i =0; i<101; i= i+4 ) {


//         getting updated gps location every second
        double position = start_location +i;
        gps = GetLatLon(position, road);
        printf("The car is at %.9f , %.9f\n", gps.lat, gps.lon);

//         message construction
        string lat = to_string(gps.lat);
        string lon = to_string(gps.lon);
        string mqtt_location_message = "{\"8660\":{\"latitude\":" + lat + ",\"longitude\":" + lon + "}}";

        string mqtt_attr_topic = "v1/gateway/attributes";

//         pulishing location message to mqtt Dashboard
        auto location_update_msg = mqtt::make_message(mqtt_attr_topic,mqtt_location_message, 1, false);
        client.publish(location_update_msg)->wait_for(10); //wait for a 10ms Timeout for acknowlodgment from broker

//         wait for 990 ms to ghave effective 1s update
        nanosleep(&sleep_time, NULL);

    }

//     Print final car position
    printf("The car has reached its destination at B : %f , %f\n", gps.lat, gps.lon);


}
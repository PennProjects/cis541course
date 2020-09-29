#include <iostream>
#include <string>
#include <mqtt/async_client.h>
#include <mqtt/connect_options.h>
#include <mqtt/ssl_options.h>

#include "vc2gps.h"

using namespace std;

const string tb_uri = {"ssl://tb.precise.seas.upenn.edu:8883"};
string client_id = {"client_id_1234"};
string username = {"SHARED_DASHBOARD"};
string password = {""};

int main(int argc, char* argv[]) {

    double position = 75; // can take values from 0 to 100
    RoadIdentifier road = RoadIdentifier::AB;
    GPS_Coord c = GetLatLon(position, road);
    printf("lat %.9f\n", c.lat);
    printf("lon %.9f\n", c.lon);

    // Init Client
    mqtt::async_client client(tb_uri, client_id);

    // Username & Password
    mqtt::connect_options connopts(username, password);
    // Enable SSL
    mqtt::ssl_options sslopts;
    connopts.set_ssl(sslopts);
    // Perform connection
    client.connect(connopts)->wait();


}
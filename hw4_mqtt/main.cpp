#include <iostream>
#include <string>
#include <mqtt/async_client.h>

#include "vc2gps.h"

int main(int argc, char* argv[]){
    double position = 75; // can take values from 0 to 100
    RoadIdentifier road = RoadIdentifier::AB;
    GPS_Coord c = GetLatLon(position, road);
    printf("lat %.9f\n", c.lat);
    printf("lon %.9f\n", c.lon);
}
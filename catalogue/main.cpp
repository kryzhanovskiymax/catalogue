//
//  main.cpp
//  catalogue
//
//  Created by Максим Крыжановский on 11.07.2021.
//

#include <iostream>
#include <iostream>
#include <fstream>

#include "transport_catalogue.hpp"
#include "json_reader.hpp"
#include "request_handler.hpp"

using namespace std;
using namespace json;
using namespace transport_catalogue;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::request_handler;
using namespace transport_catalogue::json_reader;
using namespace transport_catalogue::request_handler::detail;

/*void TestTransportCatalogue() {
    BusCommand bc1;
    bc1.name = "114";
    bc1.is_round_trip = false;
    bc1.stops.push_back("Морской вокзал");
    bc1.stops.push_back("Ривьерский мост");
    
    StopCommand sc1;
    sc1.name = "Ривьерский мост";
    sc1.coordinates.lat = 43.6;
    sc1.coordinates.lng = 39.7;
    sc1.stops_to_distances.insert({"Морской вокзал", 850});
    
    StopCommand sc2;
    sc2.name = "Морской вокзал";
    sc2.coordinates.lat = 43.5;
    sc2.coordinates.lng = 39.7;
    sc2.stops_to_distances.insert({"Ривьерский мост", 850});
    
    TransportCatalogue tc;
    tc.AddStop(sc1);
    tc.AddStop(sc2);
    tc.AddBus(bc1);
    
    std::cout << "THIS IS RESPONSE" << std::endl;
    auto response1 = tc.GetStop("Морской вокзал");
    std::cout << "------------------------------" << std::endl;
    std::cout << response1.name << std::endl;
    for(const auto& bus : response1.buses) {
        std::cout << bus << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    
    auto response2 = tc.GetBus("114");
    std::cout << "------------------------------" << std::endl;
    std::cout << response2.name << std::endl;
    std::cout << response2.stops << std::endl;
    std::cout << response2.unique_stops << std::endl;
    std::cout << response2.route_length << std::endl;
    std::cout << response2.route_distance << std::endl;
    std::cout << response2.curvature << std::endl;
}*/

std::string response_field = "/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/response.json";

void RunSystem() {
    std::cout << "------------------------------" << std::endl;
    std::cout << "Testing System" << std::endl;
    std::string path = "/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/city.json";
    std::fstream ifs;
    ifs.open(path);
    std::fstream ofs;
    ofs.open(response_field);
    
    if(!ifs.is_open() || !ofs.is_open()) {
        std::cout << "Something went Wrong" << std::endl;
    } else {
        std::cout << "File is opened..." << std::endl;

        JsonReader jr;
        TransportCatalogue tc;
        RequestHandler rh;
        
        std::cout << "System loading..." << std::endl;
        jr.ReadJson(ifs);
        jr.InitializeTransportCatalogue(tc);
        rh.InitializeRequestHandler(jr.GetRequests());
        rh.HandleRequests(tc);
        jr.WriteResponse(rh.GetResponses());
        jr.Print(ofs);
        std::cout << "PROGRAMM FINIHED SUCCESFULLY" << std::endl;
    }
}

int main(int argc, const char * argv[]) {
    RunSystem();
    return 0;
}

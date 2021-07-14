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
#include "log_duration.hpp"

using namespace std;
using namespace json;
using namespace transport_catalogue;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::request_handler;
using namespace transport_catalogue::json_reader;
using namespace transport_catalogue::request_handler::detail;

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
    LogDuration("TransportCatalogue system speed test", std::cout);
    RunSystem();
    return 0;
}

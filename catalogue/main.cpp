//
//  main.cpp
//  catalogue
//
//  Created by Максим Крыжановский on 11.07.2021.
//

#include <iostream>
#include <iostream>
#include <fstream>
#include <chrono>

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

using Clock = std::chrono::steady_clock;

void RunSystem(std::fstream& ifs, std::fstream& ofs) {
    JsonReader jr;
    TransportCatalogue tc;
    RequestHandler rh;
    jr.ReadJson(ifs);
    jr.InitializeTransportCatalogue(tc);
    rh.InitializeRequestHandler(jr.GetRequests());
    rh.HandleRequests(tc);
    jr.WriteResponse(rh.GetResponses());
    std::cout << "Buses: " << tc.GetBusCount() << std::endl;
    std::cout << "Stops: " << tc.GetStopCount() << std::endl;
    std::cout << "Requests: " << rh.GetRequestCount() << std::endl;
    jr.Print(ofs);
}

int main(int argc, const char * argv[]) {
    
    std::cout << "Tests started" << std::endl;
    
    /*std::fstream test1;
    test1.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/test1.json");
    std::fstream test1_response;
    test1_response.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/test1_response.json");
    
    std::fstream test2;
    test2.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/test2.json");
    std::fstream test2_response;
    test2_response.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/test2_response.json");
    
    std::fstream test3;
    test3.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/test3.json");
    std::fstream test3_response;
    test3_response.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/test3_response.json");
    
    std::fstream test4;
    test4.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/test4.json");
    std::fstream test4_response;
    test4_response.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/test4_response.json");*/
    
    std::fstream ifs;
    ifs.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/test5.json");
    std::fstream ofs;
    ofs.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/test5_response.json");
    
    const Clock::time_point start_time = Clock::now();
    
    RunSystem(ifs, ofs);
    
    const auto end_time = Clock::now();
    const auto dur = end_time - start_time;
    std::cout << duration_cast<std::chrono::milliseconds>(dur).count() << " ms" << std::endl;
    
    return 0;
}

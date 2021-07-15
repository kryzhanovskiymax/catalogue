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



void RunSystem(std::istream& is, std::ostream& os) {
    JsonReader jr;
    TransportCatalogue tc;
    RequestHandler rh;
    jr.ReadJson(is);
    jr.InitializeTransportCatalogue(tc);
    rh.InitializeRequestHandler(jr.GetRequests());
    rh.HandleRequests(tc);
    jr.WriteResponse(rh.GetResponses());
    jr.Print(os);
}

int main() {
    std::ifstream ifs;
    ifs.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/city.json");
    std::ofstream ofs;
    ofs.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/response.json");
    
    std::cout << "System is running" << std::endl;
    RunSystem(ifs, ofs);
    std::cout << "System run is finished" << std::endl;
    
    return 0;
}

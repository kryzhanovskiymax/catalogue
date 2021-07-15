#include <iostream>
#include <fstream>

#include "transport_catalogue.h"
#include "json_reader.h"
#include "request_handler.h"

using namespace std;
using namespace json;
using namespace transport_catalogue;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::request_handler;
using namespace transport_catalogue::json_reader;
using namespace transport_catalogue::request_handler::detail;

void RunSystem() {
    JsonReader jr;
    TransportCatalogue tc;
    RequestHandler rh;
    jr.ReadJson(std::cin);
    jr.InitializeTransportCatalogue(tc);
    rh.InitializeRequestHandler(jr.GetRequests());
    rh.HandleRequests(tc);
    jr.WriteResponse(rh.GetResponses());
    jr.Print(std::cout);
}

int main() {
    std::ifstream ifs;
    ifs.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/city.json");
    std::ofstream ofs;
    ofs.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/response.json");
    
    RunSystem();
    return 0;
}

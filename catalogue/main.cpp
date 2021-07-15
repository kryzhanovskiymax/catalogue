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
    RunSystem();
    return 0;
}

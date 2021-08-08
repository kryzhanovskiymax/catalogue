#include <iostream>
#include <fstream>

#include "transport_catalogue.hpp"
#include "json_reader.hpp"
#include "request_handler.hpp"
#include "map_renderer.hpp"
#include "svg_package.hpp"
#include "log_duration.hpp"

using namespace std;
using namespace json;
using namespace transport_catalogue;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::request_handler;
using namespace transport_catalogue::json_reader;
using namespace transport_catalogue::request_handler::detail;
using namespace transport_catalogue::map_renderer;



void RunSystem(std::istream& is, std::ostream& out_base, std::ostream& out_map) {
    JsonReader json_reader;
    TransportCatalogue transport_catalogue;
    RequestHandler request_handler;
    MapRenderer map_renderer;
    
    //Data Base Procession
    json_reader.ReadJson(is);
    json_reader.InitializeTransportCatalogue(transport_catalogue);
    request_handler.InitializeRequestHandler(json_reader.GetRequests());
    request_handler.HandleRequests(transport_catalogue);
    json_reader.WriteResponse(request_handler.GetResponses());
    json_reader.Print(out_base);
    
    //Map Procession
    json_reader.SetMapSettings(map_renderer);
    transport_catalogue.InitializeMapRenderer(map_renderer);
    request_handler.InitializeMapHandler(map_renderer);
    svg::Document map;
    map_renderer.DrawMap(map);
    map.Render(out_map);
}

int main() {
    
    
    //tc.InitializeMapRenderer(mr);
    LogDuration dur("MapRenderer Test", std::cout);
    clock_t start = clock();
    
    std::ifstream is;
    is.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/city.json");
    
    std::ifstream ifs;
    ifs.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/city_data_base.json");
    
    std::ofstream os;
    os.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/response.json");
    
    std::ofstream ofs;
    ofs.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/city.svg");
    
    JsonReader jr;
    TransportCatalogue tc;
    RequestHandler rh;
    MapRenderer mr;
    
    jr.ReadJson(is);
    jr.InitializeTransportCatalogue(tc);
    rh.InitializeRequestHandler(jr.GetRequests());
    rh.HandleRequests(tc);
    jr.WriteResponse(rh.GetResponses());
    jr.Print(os);
    
    
    
    std::cout << "System Run started" << std::endl;
    {
        RunSystem(is, os, ofs);
    }
    std::cout << "System Run finished" << std::endl;
    
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
    return 0;
}

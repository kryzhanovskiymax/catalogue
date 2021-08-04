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



void RunSystem(std::istream& is, std::ostream& os) {
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
    jr.SetMapSettings(mr);
    tc.InitializeMapRenderer(mr);
}

int main() {
    
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
        LogDuration dur("MapRenderer Test", std::cout);
        jr.SetMapSettings(mr);
        tc.InitializeMapRenderer(mr);
        svg::Document map;
        mr.DrawMap(map);
        map.Render(ofs);
    }
    std::cout << "System Run finished" << std::endl;
    //tc.InitializeMapRenderer(mr);
    return 0;
}

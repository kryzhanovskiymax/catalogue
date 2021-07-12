#include "json_reader.hpp"

using namespace json;
using namespace transport_catalogue::json_reader;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::request_handler;
using namespace transport_catalogue::request_handler::detail;

void JsonReader::ReadJson(std::istream& is) {
    Document doc = Load(is);
    Node root = doc.GetRoot();
    
    if(root.AsMap().count("base_requests") == 0) {
        std::cout << "There is no field BASE_REQUESTS in Data Base" << std::endl;
    } else {
        ReadBaseRequests(root.AsMap().at("base_requests"));
    }
    
    if(root.AsMap().count("stat_requests") == 0) {
        std::cout << "There is no field STAT_REQUESTS in Data Base" << std::endl;
    } else {
        ReadStatRequests(root.AsMap().at("stat_requests"));
    }
}

void JsonReader::Print(std::ostream& os) const {
    std::vector<Node> array;
    
    for(const auto& element : response) {
        std::map<std::string, Node> dict;
        
        if(std::holds_alternative<BusResponse>(element)) {
            
            auto bus = std::get<BusResponse>(element);
            dict.insert({"request_id", Node(bus.request_id)});
            dict.insert({"curvature", Node(bus.curvature)});
            dict.insert({"route_length", Node(bus.route_length)});
            dict.insert({"stop_count", Node(static_cast<int>(bus.stop_count))});
            dict.insert({"unique_stop_count", Node(static_cast<int>(bus.unique_stop_count))});
            
        } else if(std::holds_alternative<StopResponse>(element)) {
            
            auto stop = std::get<StopResponse>(element);
            
            dict.insert({"request_id", Node(stop.request_id)});
            
            std::vector<Node> buses_;
            for(const auto& bus : stop.buses) {
                std::string str = static_cast<std::string>(bus);
                buses_.push_back(Node(str));
            }
            dict.insert({"buses", Node(buses_)});
            
        }
        
        array.push_back(Node(dict));
    }
    
    json::Print(Document(Node(array)), os);
}

void JsonReader::InitializeTransportCatalogue(TransportCatalogue& transport_catalogue) {
    
    for(const auto& stop_command : base_requests.first) {
        transport_catalogue.AddStop(stop_command);
    }
    
    for(const auto& bus_command : base_requests.second) {
        transport_catalogue.AddBus(bus_command);
    }
    
}

std::vector<Request> JsonReader::GetRequests() {
    return requests;
}

void JsonReader::WriteResponse(std::vector<std::variant<std::nullptr_t, StopResponse, BusResponse>> answer) {
    response = answer;
}

void JsonReader::ReadBaseRequests(const Node& base_req) {
    for(const auto& element : base_req.AsArray()) {
        
        if(element.AsMap().at("type").AsString() == "Bus") {
            
            BusCommand bc;
            
            bc.name = element.AsMap().at("name").AsString();
            bc.is_round_trip = element.AsMap().at("is_roundtrip").AsBool();
            
            for(const auto& stop : element.AsMap().at("stops").AsArray()) {
                bc.stops.push_back(stop.AsString());
            }
            
            base_requests.second.push_back(bc);
            
        } else if(element.AsMap().at("type").AsString() == "Stop") {
            
            StopCommand sc;
            
            sc.name = element.AsMap().at("name").AsString();
            sc.coordinates.lat = element.AsMap().at("latitude").AsDouble();
            sc.coordinates.lng = element.AsMap().at("longitude").AsDouble();
            
            for(const auto& [stop, distance] : element.AsMap().at("road_distances").AsMap()) {
                sc.stops_to_distances.insert({stop, distance.AsDouble()});
            }
            
            base_requests.first.push_back(sc);
            
        } else {
            std::cout << element.AsMap().at("type").AsString() << " - Wrong element TYPE" << std::endl;
        }
    }
}

void JsonReader::ReadStatRequests(const Node& stat_req) {
    
    for(const auto& request : stat_req.AsArray()) {
        Request req;
        req.id = request.AsMap().at("id").AsInt();
        req.name = request.AsMap().at("name").AsString();
        
        if(request.AsMap().at("type").AsString() == "Bus") {
            req.type = QueryType::BusQuery;
        } else if(request.AsMap().at("type").AsString() == "Stop") {
            req.type = QueryType::StopQuery;
        } else {
            std::cout << request.AsMap().at("type").AsString() << " - Wrong request TYPE" << std::endl;
        }
        
        requests.push_back(req);
    }
}

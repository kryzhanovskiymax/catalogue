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
    
    if(root.AsMap().count("render_settings") == 0) {
        std::cout << "There is no field RENDER_SETTINGS in Data Base" << std::endl;
    } else {
        ReadRenderSettings(root.AsMap().at("render_settings"));
    }
}

void JsonReader::Print(std::ostream& os) const {
    std::vector<Node> array;
    
    for(const auto& element : response) {
        std::map<std::string, Node> dict;
        
        if(std::holds_alternative<BusResponse>(element)) {
            InsertBusResponse(std::get<BusResponse>(element), dict);
            
        } else if(std::holds_alternative<StopResponse>(element)) {
            InsertStopResponse(std::get<StopResponse>(element), dict);
            
        } else if(std::holds_alternative<ErrorResponse>(element)) {
            InsertErrorResponse(std::get<ErrorResponse>(element), dict);
            
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

void JsonReader::WriteResponse(std::vector<std::variant<std::nullptr_t, StopResponse, BusResponse, ErrorResponse>> answer) {
    response = answer;
}

void JsonReader::ReadBaseRequests(const Node& base_req) {
    for(const auto& element : base_req.AsArray()) {
        if(element.AsMap().at("type").AsString() == "Bus") {
            
            base_requests.second.push_back(GetBusCommandFromNode(element));
            
        } else if(element.AsMap().at("type").AsString() == "Stop") {
            
            base_requests.first.push_back(GetStopCommandFromNode(element));
            
        } else {
            std::cout << element.AsMap().at("type").AsString() << " - Wrong element TYPE" << std::endl;
        }
    }
}

void JsonReader::ReadStatRequests(const Node& stat_req) {
    for(const auto& request : stat_req.AsArray()) {
        requests.push_back(GetRequestFromNode(request));
    }
}

void JsonReader::ReadRenderSettings(const json::Node &render_settings) {
    
}

BusCommand JsonReader::GetBusCommandFromNode(const Node& node) const {
    BusCommand bc;
    
    bc.name = std::move(node.AsMap().at("name").AsString());
    bc.is_round_trip = node.AsMap().at("is_roundtrip").AsBool();
    
    if(node.AsMap().count("stops") > 0) {
        for(const auto& stop : node.AsMap().at("stops").AsArray()) {
            bc.stops.push_back(std::move(stop.AsString()));
        }
    }
    
    return bc;
}

StopCommand JsonReader::GetStopCommandFromNode(const Node& node) const {
    StopCommand sc;
    
    sc.name = std::move(node.AsMap().at("name").AsString());
    sc.coordinates.lat = node.AsMap().at("latitude").AsDouble();
    sc.coordinates.lng = node.AsMap().at("longitude").AsDouble();
    
    if(node.AsMap().count("road_distances") > 0) {
        for(const auto& [stop, distance] : node.AsMap().at("road_distances").AsMap()) {
            sc.stops_to_distances.insert({std::move(stop), distance.AsDouble()});
        }
    }
    
    return sc;
}

Request JsonReader::GetRequestFromNode(const Node& node) const {
    Request req;

    req.id = node.AsMap().at("id").AsInt();
    req.name = std::move(node.AsMap().at("name").AsString());
    
    if(node.AsMap().at("type").AsString() == "Bus") {
        req.type = QueryType::BusQuery;
    } else if(node.AsMap().at("type").AsString() == "Stop") {
        req.type = QueryType::StopQuery;
    } else {
        throw std::invalid_argument("Wrong element TYPE");
    }
    
    return req;
}

void JsonReader::InsertBusResponse(request_handler::detail::BusResponse bus, std::map<std::string, json::Node>& target) const {
    
    double id = static_cast<int>(bus.request_id)*1.0;
    target.insert({"request_id", Node(id)});
    target.insert({"curvature", Node(bus.curvature)});
    target.insert({"route_length", Node(bus.route_length)});
    target.insert({"stop_count", Node(static_cast<int>(bus.stop_count))});
    target.insert({"unique_stop_count", Node(static_cast<int>(bus.unique_stop_count))});
}

void JsonReader::InsertStopResponse(request_handler::detail::StopResponse stop, std::map<std::string, json::Node>& target) const {
    
    double id = static_cast<int>(stop.request_id)*1.0;
    target.insert({"request_id", Node(id)});
    
    std::vector<Node> buses_;
    for(const auto& bus : stop.buses) {
        std::string str = static_cast<std::string>(bus);
        buses_.push_back(Node(str));
    }
    target.insert({"buses", Node(buses_)});
}

void JsonReader::InsertErrorResponse(request_handler::detail::ErrorResponse error, std::map<std::string, json::Node>& target) const {
    double id = static_cast<int>(error.request_id)*1.0;
    target.insert({"request_id", Node(id)});
    target.insert({"error_message", Node(std::move(error.error_message))});
}

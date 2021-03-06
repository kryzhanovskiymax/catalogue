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
        } else if(std::holds_alternative<MapResponse>(element)) {
            InsertMapResponse(std::get<MapResponse>(element), dict);
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

void JsonReader::SetMapSettings(map_renderer::MapRenderer& map) {
    map.SetMapSettings(map_settings);
}

void JsonReader::WriteResponse(std::vector<Response> answer) {
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
    map_settings.width = render_settings.AsMap().at("width").AsDouble();
    map_settings.height = render_settings.AsMap().at("height").AsDouble();
    map_settings.padding = render_settings.AsMap().at("padding").AsDouble();
    map_settings.stop_radius = render_settings.AsMap().at("stop_radius").AsDouble();
    map_settings.line_width = render_settings.AsMap().at("line_width").AsDouble();
    map_settings.bus_label_font_size = render_settings.AsMap().at("bus_label_font_size").AsDouble();
    map_settings.stop_label_font_size = render_settings.AsMap().at("stop_label_font_size").AsDouble();
    map_settings.underlayer_width = render_settings.AsMap().at("underlayer_width").AsDouble();
    map_settings.bus_label_offset.push_back(render_settings.AsMap().at("bus_label_offset").AsArray()[0].AsDouble());
    map_settings.bus_label_offset.push_back(render_settings.AsMap().at("bus_label_offset").AsArray()[1].AsDouble());
    map_settings.stop_label_offset.push_back(render_settings.AsMap().at("stop_label_offset").AsArray()[0].AsDouble());
    map_settings.stop_label_offset.push_back(render_settings.AsMap().at("stop_label_offset").AsArray()[1].AsDouble());
    
    map_settings.underlayer_color = GetColorFromNode(render_settings.AsMap().at("underlayer_color"));
    
    for(const Node& color_node : render_settings.AsMap().at("color_palette").AsArray()) {
        map_settings.color_palette.push_back(GetColorFromNode(color_node));
    }
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
    if(node.AsMap().count("name") > 0) {
        req.name = std::move(node.AsMap().at("name").AsString());
    }
    
    if(node.AsMap().at("type").AsString() == "Bus") {
        req.type = QueryType::BusQuery;
    } else if(node.AsMap().at("type").AsString() == "Stop") {
        req.type = QueryType::StopQuery;
    } else if(node.AsMap().at("type").AsString() == "Map") {
        req.type = QueryType::MapQuery;
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

void JsonReader::InsertMapResponse(request_handler::detail::MapResponse map, std::map<std::string, json::Node>& target) const {
    double id = static_cast<int>(map.request_id)*1.0;
    target.insert({"request_id", Node(id)});
    target.insert({"map", Node(std::move(map.map))});
}

svg::Color JsonReader::GetColorFromNode(const json::Node& color_node) {
    svg::Color color;
    
    if(color_node.IsString()) {
        color = color_node.AsString();
    } else if(color_node.IsArray()) {
        if(color_node.AsArray().size() == 3) {
            svg::Rgb color_;

            color_.red = color_node.AsArray()[0].AsInt();
            color_.green = color_node.AsArray()[1].AsInt();
            color_.blue = color_node.AsArray()[2].AsInt();
            
            color = color_;
        } else if(color_node.AsArray().size() == 4) {
            svg::Rgba color_;
            color_.red = color_node.AsArray()[0].AsInt();
            color_.green = color_node.AsArray()[1].AsInt();
            color_.blue = color_node.AsArray()[2].AsInt();
            color_.opacity = color_node.AsArray()[3].AsDouble();
            
            color = color_;
        } else {
            std::cout << "array is too big" << std::endl;
        }
    } else {
        std::cout << "Wrong format" << std::endl;
    }
    
    return color;
}

#pragma once

#include "domain.hpp"
#include "request_handler.hpp"
#include "json.hpp"
#include "map_renderer.hpp"

namespace transport_catalogue {

namespace json_reader {

class JsonReader {
public:
    void ReadJson(std::istream& is);
    void Print(std::ostream& os) const;
    void InitializeTransportCatalogue(TransportCatalogue& transport_catalogue);
    std::vector<transport_catalogue::request_handler::detail::Request> GetRequests();
    void WriteResponse(std::vector<transport_catalogue::request_handler::Response> answer);
    void SetMapSettings(map_renderer::MapRenderer& map);
    
private:
    void ReadBaseRequests(const json::Node& base_req);
    void ReadStatRequests(const json::Node& stat_req);
    void ReadRenderSettings(const json::Node& render_settings);
    
    detail::BusCommand GetBusCommandFromNode(const json::Node& node) const;
    detail::StopCommand GetStopCommandFromNode(const json::Node& node) const;
    request_handler::detail::Request GetRequestFromNode(const json::Node& node) const;
    
    void InsertBusResponse(request_handler::detail::BusResponse bus, std::map<std::string, json::Node>& target) const;
    void InsertStopResponse(request_handler::detail::StopResponse stop, std::map<std::string, json::Node>& target) const;
    void InsertErrorResponse(request_handler::detail::ErrorResponse error, std::map<std::string, json::Node>& target) const;
    void InsertMapResponse(request_handler::detail::MapResponse map, std::map<std::string, json::Node>& target) const;
    
    svg::Color GetColorFromNode(const json::Node& color_node);
    
    std::pair<std::vector<transport_catalogue::detail::StopCommand>, std::vector<transport_catalogue::detail::BusCommand>> base_requests;
    std::vector<transport_catalogue::request_handler::detail::Request> requests;
    std::vector<transport_catalogue::request_handler::Response> response;
    transport_catalogue::map_renderer::detail::MapSettings map_settings;
};

}

}

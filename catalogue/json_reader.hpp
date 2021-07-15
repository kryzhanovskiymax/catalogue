#pragma once

#include "domain.hpp"
#include "request_handler.hpp"
#include "json.hpp"

namespace transport_catalogue {

namespace json_reader {

class JsonReader {
public:
    void ReadJson(std::istream& is);
    void Print(std::ostream& os) const;
    void InitializeTransportCatalogue(TransportCatalogue& transport_catalogue);
    std::vector<transport_catalogue::request_handler::detail::Request> GetRequests();
    void WriteResponse(std::vector<std::variant<std::nullptr_t, transport_catalogue::request_handler::detail::StopResponse, transport_catalogue::request_handler::detail::BusResponse, transport_catalogue::request_handler::detail::ErrorResponse>> answer);
    
private:
    void ReadBaseRequests(const json::Node& base_req);
    void ReadStatRequests(const json::Node& stat_req);
    
    detail::BusCommand GetBusCommandFromNode(const json::Node& node) const;
    detail::StopCommand GetStopCommandFromNode(const json::Node& node) const;
    request_handler::detail::Request GetRequestFromNode(const json::Node& node) const;
    
    void InsertBusResponse(request_handler::detail::BusResponse bus, std::map<std::string, json::Node>& target) const;
    void InsertStopResponse(request_handler::detail::StopResponse stop, std::map<std::string, json::Node>& target) const;
    void InsertErrorResponse(request_handler::detail::ErrorResponse error, std::map<std::string, json::Node>& target) const;
    
    std::pair<std::vector<transport_catalogue::detail::StopCommand>, std::vector<transport_catalogue::detail::BusCommand>> base_requests;
    std::vector<transport_catalogue::request_handler::detail::Request> requests;
    std::vector<std::variant<std::nullptr_t, transport_catalogue::request_handler::detail::StopResponse, transport_catalogue::request_handler::detail::BusResponse, transport_catalogue::request_handler::detail::ErrorResponse>> response;
};

}

}

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
    
    std::pair<std::vector<transport_catalogue::detail::StopCommand>, std::vector<transport_catalogue::detail::BusCommand>> base_requests;
    std::vector<transport_catalogue::request_handler::detail::Request> requests;
    std::vector<std::variant<std::nullptr_t, transport_catalogue::request_handler::detail::StopResponse, transport_catalogue::request_handler::detail::BusResponse, transport_catalogue::request_handler::detail::ErrorResponse>> response;
};

}

}

#pragma once

#include "transport_catalogue.hpp"

#include <vector>
#include <variant>
#include <string>
#include <memory>

namespace transport_catalogue {

namespace request_handler {

namespace detail {

enum class QueryType {
    StopQuery,
    BusQuery
};

struct BusResponse {
    size_t request_id;
    double curvature;
    double route_length;
    size_t stop_count;
    size_t unique_stop_count;
    bool exists;
};
    
struct StopResponse {
    size_t request_id;
    std::set<std::string> buses;
    bool exists;
};

struct ErrorResponse {
    size_t request_id;
    std::string error_message;
};

struct Request {
    size_t id;
    QueryType type;
    std::string name;
};

}



class RequestHandler {
public:
    void InitializeRequestHandler(const std::vector<detail::Request>& requests_);
    void HandleRequests(const transport_catalogue::TransportCatalogue& transport_catalogue_);
    std::vector<std::variant<std::nullptr_t, detail::StopResponse, detail::BusResponse, detail::ErrorResponse>> GetResponses();
    void InitializeMapHandler(std::unique_ptr<transport_catalogue::map_renderer::MapRenderer>&& map_);

private:
    std::vector<detail::Request> requests;
    std::vector<std::variant<std::nullptr_t, detail::StopResponse, detail::BusResponse, detail::ErrorResponse>> responses;
    std::unique_ptr<transport_catalogue::map_renderer::MapRenderer> map_render;
};


}

}



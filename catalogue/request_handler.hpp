#pragma once

#include "transport_catalogue.hpp"

#include <vector>
#include <variant>
#include <string>
#include <memory>
#include <sstream>

namespace transport_catalogue {

namespace request_handler {

namespace detail {

enum class QueryType {
    StopQuery,
    BusQuery,
    MapQuery
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

struct MapResponse {
    size_t request_id;
    std::string map;
};

struct Request {
    size_t id;
    QueryType type;
    std::string name;
};

}

using Response = std::variant<std::nullptr_t, detail::BusResponse, detail::StopResponse, detail::ErrorResponse, detail::MapResponse>;


class RequestHandler {
public:
    void InitializeRequestHandler(const std::vector<detail::Request>& requests_);
    void HandleRequests(const transport_catalogue::TransportCatalogue& transport_catalogue_);
    std::vector<Response> GetResponses();
    void InitializeMapHandler(std::unique_ptr<transport_catalogue::map_renderer::MapRenderer>&& map_);

private:
    std::vector<detail::Request> requests;
    std::vector<Response> responses;
    std::unique_ptr<transport_catalogue::map_renderer::MapRenderer> map_render;
};


}

}



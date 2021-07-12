#pragma once

#include "transport_catalogue.hpp"

#include <vector>
#include <variant>
#include <string>

namespace transport_catalogue {

namespace request_handler {

namespace detail {

enum class QueryType {
    StopQuery,
    BusQuery
};

struct BusResponse {
    int request_id;
    double curvature;
    double route_length;
    size_t stop_count;
    size_t unique_stop_count;
    bool exists;
};
    
struct StopResponse {
    int request_id;
    std::set<std::string_view> buses;
    bool exists;
};

struct Request {
    int id;
    QueryType type;
    std::string name;
};

}



class RequestHandler {
public:
    void InitializeRequestHandler(const std::vector<detail::Request>& requests_);
    void HandleRequests(const transport_catalogue::TransportCatalogue& transport_catalogue_);
    std::vector<std::variant<std::nullptr_t, detail::StopResponse, detail::BusResponse>> GetResponses();
    
private:
    std::vector<detail::Request> requests;
    std::vector<std::variant<std::nullptr_t, detail::StopResponse, detail::BusResponse>> responses;
};


}

}

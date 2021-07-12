#include "request_handler.hpp"

using namespace transport_catalogue;
using namespace transport_catalogue::request_handler;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::request_handler::detail;

void RequestHandler::InitializeRequestHandler(const std::vector<detail::Request>& requests_) {
    requests = requests_;
}

void RequestHandler::HandleRequests(const transport_catalogue::TransportCatalogue &transport_catalogue_) {
    
    for(const auto& request : requests) {
        std::variant<std::nullptr_t, StopResponse, BusResponse> response_;
        if(request.type == QueryType::BusQuery) {
            BusInfo bus = transport_catalogue_.GetBus(request.name);
            BusResponse bus_response{request.id, bus.curvature, bus.route_distance, bus.stops, bus.unique_stops, bus.exists};
            response_ = bus_response;
            
        } else {
            StopInfo stop = transport_catalogue_.GetStop(request.name);
            StopResponse stop_response{request.id, stop.buses, stop.exists};
            response_ = stop_response;
        }
        
        responses.push_back(response_);
    }
    
}

std::vector<std::variant<std::nullptr_t, StopResponse, BusResponse>> RequestHandler::GetResponses() {
    return responses;
}


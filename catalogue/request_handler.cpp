#include "request_handler.hpp"
#include <iostream>
using namespace transport_catalogue;
using namespace transport_catalogue::request_handler;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::request_handler::detail;

void RequestHandler::InitializeRequestHandler(const std::vector<detail::Request>& requests_) {
    requests = requests_;
}

void RequestHandler::HandleRequests(const transport_catalogue::TransportCatalogue& transport_catalogue_) {
    
    for(const auto& request : requests) {
        std::variant<std::nullptr_t, StopResponse, BusResponse, ErrorResponse, MapResponse> response_;
        if(request.type == QueryType::BusQuery) {
            BusInfo bus = transport_catalogue_.GetBus(request.name);
            if(!bus.exists) {
                ErrorResponse error_response;
                error_response.error_message = "not found";
                error_response.request_id = request.id;
                response_ = std::move(error_response);
            } else {
                BusResponse bus_response{request.id, bus.curvature, bus.route_distance, bus.stops, bus.unique_stops, bus.exists};
                response_ = std::move(bus_response);
            }
            
        } else {
            StopInfo stop = transport_catalogue_.GetStop(request.name);
            
            if(!stop.exists) {
                ErrorResponse error_response;
                error_response.error_message = "not found";
                error_response.request_id = request.id;
                response_ = std::move(error_response);
            } else {
                StopResponse stop_response{request.id, std::move(stop.buses), stop.exists};
                response_ = std::move(stop_response);
            }
        }
        
        responses.push_back(std::move(response_));
    }
    
}

std::vector<std::variant<std::nullptr_t, StopResponse, BusResponse, ErrorResponse, MapResponse>> RequestHandler::GetResponses() {
    return responses;
}

size_t RequestHandler::GetRequestCount() const {
    return requests.size();
}




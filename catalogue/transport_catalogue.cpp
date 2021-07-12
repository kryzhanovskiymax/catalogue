#include "transport_catalogue.hpp"
#include <iostream>

using namespace transport_catalogue;
using namespace transport_catalogue::detail;

void TransportCatalogue::AddBus(const BusCommand& command) {
    std::string_view name(command.name);
    std::vector<Stop*> stops_;
    for(std::string_view stop_name : command.stops) {
        if(names_to_stops_.count(stop_name) > 0) {
            stops_.push_back(names_to_stops_.at(stop_name));
        } else {
            throw std::invalid_argument("Wrong stop name");
        }
    }
    
    Bus bus{command.name, stops_, command.is_round_trip};
    buses.push_back(bus);
    names_to_buses_.insert(std::make_pair(name, &buses.back()));
}

void TransportCatalogue::AddStop(const StopCommand& command) {
    std::string_view name(command.name);
    Stop stop;
    stop.name = command.name;
    stop.coordinates.lat = command.coordinates.lat;
    stop.coordinates.lng = command.coordinates.lng;
    
    for(const auto& [stop_copy, distance] : command.stops_to_distances) {
        std::string_view stop_str = stop_copy;
        stop.stops_to_distances.insert({stop_str, distance});
    }
    
    stops.push_back(stop);

    names_to_stops_.insert(std::make_pair(name, &stops.back()));
}

BusInfo TransportCatalogue::GetBus(std::string_view name) const {
    BusInfo result;
    
    if(names_to_buses_.count(name) == 0) {
        return BusInfo{name, 0, 0, 0.0, 0.0, false};
    }
    
    auto bus = names_to_buses_.at(name);
    size_t stops;
    size_t unique_stops;
    if(bus->is_round_trip) {
        stops = bus->stops.size();
        unique_stops = stops - 1;
    } else {
        unique_stops = bus->stops.size();
        stops = unique_stops*2 - 1;
    }
 
    double distance = GetBusRouteDistance(bus);
    double length = GetBusRouteLength(bus);
    double curvature = distance / length;

    return BusInfo{name, stops, unique_stops, curvature, distance, length, true};
}

StopInfo TransportCatalogue::GetStop(std::string_view name) const {
    
    std::set<std::string_view> buses_;
    if(names_to_stops_.count(name) == 0) {
        return StopInfo{name, {}, false};
    }
    
    for(const auto& bus : buses) {
        for(auto stop : bus.stops) {
            if(name == stop->name) {
                std::string_view copy(bus.name);
                buses_.insert(std::move(copy));
                break;
            }
        }
    }

    
    return StopInfo{name, buses_, true};
}

double TransportCatalogue::GetDistance(Stop* from, Stop* to) const {
    if(from->stops_to_distances.count(to->name) > 0) {
        return from->stops_to_distances.at(to->name);
    }
        
    return to->stops_to_distances.at(from->name);
}

double TransportCatalogue::GetBusRouteDistance(Bus* bus) const {
    double distance = 0;
    for(int i = 1; i < bus->stops.size(); ++i) {
        distance += GetDistance(bus->stops[i-1], bus->stops[i]);
    }
    
    if(!bus->is_round_trip) {
        distance *= 2;
    }
    
    return distance;
}

double TransportCatalogue::GetBusRouteLength(Bus* bus) const {
    double length = 0;
    
    for(int i = 1; i < bus->stops.size(); ++i) {
        length += ComputeDistance(bus->stops[i-1]->coordinates, bus->stops[i]->coordinates);
    }
    
    if(bus->is_round_trip) {
        length += ComputeDistance(bus->stops[bus->stops.size()-1]->coordinates, bus->stops[0]->coordinates);
    } else {
        length *= 2;
    }
    
    return length;
}


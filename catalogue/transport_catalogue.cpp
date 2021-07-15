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
    
    Bus bus{std::move(command.name), stops_, command.is_round_trip};
    buses.push_back(std::move(bus));
    names_to_buses_.insert(std::make_pair(std::move(name), &buses.back()));
}

void TransportCatalogue::AddStop(const StopCommand& command) {
    std::string_view name(command.name);
    Stop stop;
    stop.name = std::move(command.name);
    stop.coordinates.lat = command.coordinates.lat;
    stop.coordinates.lng = command.coordinates.lng;
    
    for(const auto& [stop_copy, distance] : command.stops_to_distances) {
        std::string_view stop_str = stop_copy;
        stop.stops_to_distances.insert({std::move(stop_str), distance});
    }
    
    stops.push_back(std::move(stop));

    names_to_stops_.insert(std::make_pair(std::move(name), &stops.back()));
}

BusInfo TransportCatalogue::GetBus(std::string_view name) const {
    BusInfo result;
    
    if(names_to_buses_.count(name) == 0) {
        return BusInfo{name, 0, 0, 0.0, 0.0, 0.0, false};
    }
    
    auto bus = names_to_buses_.at(name);
    size_t stops;
    size_t unique_stops;
    std::unordered_set<std::string_view> unique_stops_;
    
    for(const auto& stop : bus->stops) {
        unique_stops_.insert(stop->name);
    }
    unique_stops = unique_stops_.size();
    
    if(bus->stops.size() < 2) {
        stops = bus->stops.size();
    } else {
        if(bus->is_round_trip) {
            stops = bus->stops.size();
        } else {
            stops = bus->stops.size()*2 - 1;
        }
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

size_t TransportCatalogue::GetStopCount() const {
    return stops.size();
}

size_t TransportCatalogue::GetBusCount() const {
    return buses.size();
}

double TransportCatalogue::GetBusRouteDistance(Bus* bus) const {
    double distance = 0;
    for(int i = 1; i < bus->stops.size(); ++i) {
        distance += GetDistance(bus->stops[i-1], bus->stops[i]);
    }
    
    if(!bus->is_round_trip) {
        for(int i = static_cast<int>(bus->stops.size()-2); i >= 0; --i) {
            distance += GetDistance(bus->stops[i+1], bus->stops[i]);
        }
    }
    
    return distance;
}

double TransportCatalogue::GetBusRouteLength(Bus* bus) const {
    double length = 0;
    
    if(bus->stops.size() < 2) {
        return 0;
    } else {
    
        for(int i = 1; i < bus->stops.size(); ++i) {
            length += ComputeDistance(bus->stops[i-1]->coordinates, bus->stops[i]->coordinates);
        }
    
        if(!bus->is_round_trip) {
            for(int i = static_cast<int>(bus->stops.size()-2); i >= 0; --i) {
                length += ComputeDistance(bus->stops[i+1]->coordinates, bus->stops[i]->coordinates);
            }
        }
    }
    
    return length;
}


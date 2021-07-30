#include "transport_catalogue.hpp"
#include <iostream>

using namespace transport_catalogue;
using namespace transport_catalogue::detail;

void TransportCatalogue::AddBus(const BusCommand& command) {
    std::string name(command.name);
    std::vector<Stop*> stops_;
    for(std::string stop_name : command.stops) {
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
    std::string name(command.name);
    Stop stop;
    stop.name = command.name;
    stop.coordinates.lat = command.coordinates.lat;
    stop.coordinates.lng = command.coordinates.lng;
    
    for(const auto& [stop_copy, distance] : command.stops_to_distances) {
        distances_.insert({std::make_pair(stop.name, stop_copy), distance});
    }
    
    stops.push_back(std::move(stop));

    names_to_stops_.insert(std::make_pair(std::move(name), &stops.back()));
}

BusInfo TransportCatalogue::GetBus(std::string name) const {
    BusInfo result;
    
    if(names_to_buses_.count(name) == 0) {
        return BusInfo{name, 0, 0, 0.0, 0.0, 0.0, false};
    }
    
    auto bus = names_to_buses_.at(name);
    size_t stops;
    size_t unique_stops;
    std::unordered_set<std::string> unique_stops_;
    
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
    double curvature;
    if(length == 0) {
        curvature = distance / 1;
    } else {
        curvature = distance / length;
    }

    return BusInfo{name, stops, unique_stops, curvature, distance, length, true};
}

StopInfo TransportCatalogue::GetStop(std::string name) const {
    
    std::set<std::string> buses_;
    if(names_to_stops_.count(name) == 0) {
        return StopInfo{name, {}, false};
    }
    
    for(const auto& bus : buses) {
        for(auto stop : bus.stops) {
            if(name == stop->name) {
                std::string copy(bus.name);
                buses_.insert(std::move(copy));
                break;
            }
        }
    }

    
    return StopInfo{name, buses_, true};
}

double TransportCatalogue::GetDistance(Stop* from, Stop* to) const {
    if(distances_.count(std::make_pair(from->name, to->name)) == 0) {
        return distances_.at(std::make_pair(to->name, from->name));
    }
    
    return distances_.at(std::make_pair(from->name, to->name));
}

void TransportCatalogue::FillMap(transport_catalogue::map_renderer::MapRenderer& map) const {
    std::vector<Stop> stop_container{stops.begin(), stops.end()};
    std::vector<Bus> bus_container{buses.begin(), buses.end()};
    
    map.CreateMap(stop_container, bus_container);
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


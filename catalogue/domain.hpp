#pragma once

#include "geo.h"

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>

namespace transport_catalogue {

namespace detail {

struct Stop {
    
    std::string name;
    Coordinates coordinates;
    
    Stop(std::string name_, Coordinates coordinates_);
    Stop() = default;
};

struct Bus {
    std::string name;
    std::vector<Stop*> stops;
    bool is_round_trip;
    
    Bus(std::string name_, std::vector<Stop*> stops_, bool is_round_trip_);
    Bus() = default;
};

struct StopInfo {
    std::string name;
    std::set<std::string> buses;
    bool exists;
};

struct BusInfo {
    std::string name;
    size_t stops;
    size_t unique_stops;
    double curvature;
    double route_distance;
    double route_length;
    bool exists;
};

struct StopCommand {
    std::string name;
    Coordinates coordinates;
    std::unordered_map<std::string, int> stops_to_distances;
};

struct BusCommand {
    std::string name;
    std::vector<std::string> stops;
    bool is_round_trip;
};

}

}


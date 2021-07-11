#include "domain.hpp"

using namespace transport_catalogue::detail;

Stop::Stop(std::string_view name_, Coordinates coordinates_, std::unordered_map<std::string_view, double> stops_to_distances_) :
                                name(name_),
                                coordinates(coordinates_),
                                stops_to_distances(stops_to_distances_) {}

Bus::Bus(std::string_view name_, std::vector<Stop*> stops_, bool is_round_trip_) :
                                name(name_),
                                stops(stops_),
                                is_round_trip(is_round_trip_){}

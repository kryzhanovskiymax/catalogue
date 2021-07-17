#include "domain.hpp"

using namespace transport_catalogue::detail;

Stop::Stop(std::string name_, Coordinates coordinates_) :
                                name(name_),
                                coordinates(coordinates_) {}

Bus::Bus(std::string name_, std::vector<Stop*> stops_, bool is_round_trip_) :
                                name(name_),
                                stops(stops_),
                                is_round_trip(is_round_trip_){}

#pragma once

#include "domain.hpp"

#include <deque>
#include <unordered_map>
#include <exception>

namespace transport_catalogue {

class TransportCatalogue {
public:
    void AddBus(const detail::BusCommand& command);
    void AddStop(const detail::StopCommand& command);
    detail::BusInfo GetBus(std::string_view name) const;
    detail::StopInfo GetStop(std::string_view name) const;
    double GetDistance(detail::Stop* from, detail::Stop* to) const;
    
private:
    std::deque<detail::Bus> buses;
    std::deque<detail::Stop> stops;
    std::unordered_map<std::string_view, detail::Bus*> names_to_buses_;
    std::unordered_map<std::string_view, detail::Stop*> names_to_stops_;
    
    double GetBusRouteDistance(detail::Bus* bus) const;
    double GetBusRouteLength(detail::Bus* bus) const;
};

}

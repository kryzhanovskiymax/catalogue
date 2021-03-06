#pragma once

#include "domain.hpp"
#include "map_renderer.hpp"

#include <deque>
#include <unordered_map>
#include <exception>
#include <unordered_set>

namespace transport_catalogue {

class TransportCatalogue {
public:
    void AddBus(const detail::BusCommand& command);
    void AddStop(const detail::StopCommand& command);
    detail::BusInfo GetBus(std::string name) const;
    detail::StopInfo GetStop(std::string name) const;
    double GetDistance(detail::Stop* from, detail::Stop* to) const;
    void InitializeMapRenderer(transport_catalogue::map_renderer::MapRenderer& map);
    
private:
    std::deque<detail::Bus> buses;
    std::deque<detail::Stop> stops;
    std::unordered_map<std::string, detail::Bus*> names_to_buses_;
    std::unordered_map<std::string, detail::Stop*> names_to_stops_;
    std::map<std::pair<std::string, std::string>, int> distances_;
    
    double GetBusRouteDistance(detail::Bus* bus) const;
    double GetBusRouteLength(detail::Bus* bus) const;
    std::set<std::string> FindStopBuses(std::string name) const;
};

}

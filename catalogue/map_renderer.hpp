#pragma once

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>

#include "domain.hpp"
#include "geo.h"
#include "svg_package.hpp"


namespace transport_catalogue {

namespace map_renderer {

namespace detail {

struct MapSettings {
    double width;
    double height;
    double padding;
    double line_width;
    double stop_radius;
    double bus_label_font_size;
    std::vector<double> bus_label_offset;
    double stop_label_font_size;
    std::vector<double> stop_label_offset;
    double underlayer_width;
    svg::Color underlayer_color;
    std::vector<svg::Color> color_palette;
};

struct MapCoefficients {
    double min_lat = 0;
    double max_lat = 0;
    double min_lng = 0;
    double max_lng = 0;
    double zoom_coef = 0;
};

}

class MapRenderer {
public:
    void CreateMap(std::vector<transport_catalogue::detail::Stop> stops, std::vector<transport_catalogue::detail::Bus> buses);
    void DrawMap(svg::Document& map);
    void SetMapSettings(detail::MapSettings settings_);
    void Render(std::ostream& os) const;
    
    //Вспомогательные методы
    detail::MapSettings GetMapSettings() const;
    void ShowContainers() const;
    void ShowCoefficients() const;
private:
    svg::Document map_document;
    std::map<std::string, std::pair<bool, std::vector<std::string>>> bus_to_stops;
    std::map<std::string, svg::Point> stop_to_position;
    std::map<std::string, svg::Color> bus_route_to_color;
    detail::MapSettings settings;
    detail::MapCoefficients coefficients;
    
    svg::Point TranslateCoordinatesToPoint(transport_catalogue::detail::Coordinates coordinates);
    void CalculateCoefficients(std::vector<transport_catalogue::detail::Coordinates> stops_coordinates);
    svg::Color GetColor();
    void DrawBusRoutes(svg::Document& map);
    void DrawBusNames(svg::Document& map);
    void DrawStopMarks(svg::Document& map);
    void DrawStopNames(svg::Document& map);
    
    int color_idx = 0;
};

}

}

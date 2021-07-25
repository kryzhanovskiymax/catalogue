#pragma once

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#include "svg.hpp"
#include "geo.h"

namespace transport_catalogue {

namespace map_renderer {

class MapRenderer {
public:
    void CreateMap(std::vector<std::string, std::pair<bool, std::vector<std::string>>> buses, std::vector<std::pair<std::string, transport_catalogue::detail::Coordinates>> stops);
private:
    void CalculateCoefficients(std::vector<transport_catalogue::detail::Coordinates> coordinates_container);
    svg::Point TransformCoordinateToPoint(transport_catalogue::detail::Coordinates coordinates);
    
    std::map<std::string, std::pair<bool, std::vector<std::string>>> bus_to_stops;
    std::map<std::string, svg::Point> stop_to_position;
    
    double width;
    double height;
    double padding;
    double line_width;
    double stop_radius;
    double bus_label_font_size;
    double bus_label_offset;
    double underlayer_width;
    svg::Color underlayer_color;
    std::vector<svg::Color> color_palette;
    
    double min_lat;
    double min_lon;
    double max_lat;
    double max_lon;
    double height_zoom_coef;
    double width_zoom_coef;
};

}

}

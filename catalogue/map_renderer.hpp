#pragma once

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include "transport_catalogue.hpp"
#include "geo.h"
#include "svg_package.hpp"


namespace transport_catalogue {

namespace map_renderer {

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
    double min_lat;
    double max_lat;
    double min_lng;
    double max_lng;
    double zoom_coef;
};

class MapRenderer {
public:
    void CreateMap(std::vector<transport_catalogue::detail::Stop> stops, std::vector<transport_catalogue::detail::Bus> buses);
    void DrawMap();
    
    void SetMapSettings(MapSettings settings_);
private:
    svg::Document map_document;
    std::map<std::string, std::pair<bool, std::vector<std::string>>> bus_to_stops;
    std::map<std::string, svg::Point> stop_to_position;
    std::map<std::string, svg::Color> bus_route_to_color;
    MapSettings settings;
    MapCoefficients coefficients;
    
    double min_lat;
    double max_lat;
    double min_lng;
    double max_lng;
    
    double zoom_coef = 0;
    
    svg::Point TranslateCoordinatesToPoint(transport_catalogue::detail::Coordinates coordinates);
    void CalculateCoefficients(std::vector<transport_catalogue::detail::Coordinates> stops_coordinates);
    svg::Color GetColor();
    
    void DrawBusRoutes();
    void DrawBusNames();
    void DrawStopMarks();
    void DrawStopNames();
    
    int color_idx = 0;
};

}

}

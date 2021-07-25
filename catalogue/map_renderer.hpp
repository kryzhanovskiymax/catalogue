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

class MapRenderer {
public:
    void CreateMap(std::vector<transport_catalogue::detail::Stop> stops, std::vector<transport_catalogue::detail::Bus> buses);
    void CalculateCoefficients(std::vector<transport_catalogue::detail::Coordinates> stops_coordinates);
    
    void SetWidth(double width_);
    void SetHeight(double height_);
    void SetPadding(double padding_);
    void SetLineWidth(double line_width_);
    void SetStopRadius(double stop_radius_);
    void SetBusLabelFontSize(double bus_label_font_size_);
    void SetBusLabelOffset(double bus_label_offset_);
    void SetUnderlayerWidth(double underlayer_width_);
    void SetUnderlayerColor(svg::Color underlayer_color_);
    void SetColorPalette(std::vector<svg::Color> color_palette_);
private:
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
    double max_lat;
    double min_lng;
    double max_lng;
    
    double height_zoom_coef = 0;
    double width_zoom_coef = 0;
    
    svg::Point TranslateCoordinatesToPoint(transport_catalogue::detail::Coordinates coordinates);
};

}

}

#pragma once

#include "transport_catalogue.hpp"
#include "json_reader.hpp"
#include "svg.hpp"

#include <map>
#include <string>
#include <vector>
#include <utility>

namespace transport_catalogue {

namespace map_renderer {

class MapRenderer {
public:
    void SetWidth(double width_);
    void SetHeight(double height_);
    void SetPadding(double padding_);
    void SetLineWidth(double line_width_);
    void SetStopRadius(double stop_radius_);
    void SetBusLabelFontSize(double bus_label_font_size_);
    void SetBusLabelOffset(double bus_label_offset_);
    void SetUnderlayerColor(double underlayer_color_);
private:
    std::map<std::string, std::pair<std::vector<std::string>, bool>> buses_to_stops;
    std::map<std::string, svg::Point> stops_to_position;
    
    double width;
    double height;
    double padding;
    double line_width;
    double stop_radius;
    double bus_label_font_size;
    double bus_label_offset;
    double underlayer_color;
};

}

}

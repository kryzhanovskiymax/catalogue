#include "map_renderer.hpp"

using namespace transport_catalogue;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::map_renderer;

void MapRenderer::CalculateCoefficients(std::vector<transport_catalogue::detail::Coordinates> stops_coordinates) {
    std::vector<double> longitudes;
    std::vector<double> latitudes;
    
    for(const auto& coordinates : stops_coordinates) {
        longitudes.push_back(coordinates.lng);
        latitudes.push_back(coordinates.lat);
    }
    
    min_lat = *std::min(latitudes.begin(), latitudes.end());
    max_lat = *std::max(longitudes.begin(), longitudes.end());
    min_lng = *std::min(longitudes.begin(), longitudes.end());
    max_lng = *std::max(longitudes.begin(), longitudes.end());
    
    height_zoom_coef = (height - 2*padding)/(max_lat - min_lat);
    width_zoom_coef = (width - 2*padding)/(max_lng-min_lng);
}



void MapRenderer::SetWidth(double width_) {
    width = width_;
}

void MapRenderer::SetHeight(double height_) {
    height = height_;
}

void MapRenderer::SetPadding(double padding_) {
    padding = padding_;
}

void MapRenderer::SetLineWidth(double line_width_) {
    line_width = line_width_;
}

void MapRenderer::SetStopRadius(double stop_radius_) {
    stop_radius = stop_radius_;
}

void MapRenderer::SetBusLabelFontSize(double bus_label_font_size_) {
    bus_label_font_size = bus_label_font_size_;
}

void MapRenderer::SetBusLabelOffset(double bus_label_offset_) {
    bus_label_offset = bus_label_offset_;
}

void MapRenderer::SetUnderlayerWidth(double underlayer_width_) {
    underlayer_width = underlayer_width_;
}

void MapRenderer::SetUnderlayerColor(svg::Color underlayer_color_) {
    underlayer_color = underlayer_color_;
}

void MapRenderer::SetColorPalette(std::vector<svg::Color> color_palette_) {
    color_palette = color_palette_;
}

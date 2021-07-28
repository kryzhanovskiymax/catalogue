#include "map_renderer.hpp"

using namespace transport_catalogue;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::map_renderer;

void MapRenderer::CreateMap(std::vector<transport_catalogue::detail::Stop> stops, std::vector<transport_catalogue::detail::Bus> buses) {
    
}

void MapRenderer::CalculateCoefficients(std::vector<Coordinates> stops_coordinates) {
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
    
    double height_zoom_coef = (settings.height - 2*settings.padding)/(max_lat - min_lat);
    double width_zoom_coef = (settings.width - 2*settings.padding)/(max_lng-min_lng);
    
    if(height_zoom_coef == 0) {
        zoom_coef = width_zoom_coef;
    } else if(width_zoom_coef == 0) {
        zoom_coef = height_zoom_coef;
    } else {
        zoom_coef = std::min(height_zoom_coef, width_zoom_coef);
    }
}

svg::Point MapRenderer::TranslateCoordinatesToPoint(Coordinates coordinates) {
    double x = (coordinates.lng - min_lng)*zoom_coef + settings.padding;
    double y = (max_lat - coordinates.lat)*zoom_coef + settings.padding;
    
    return svg::Point{x, y};
}

void MapRenderer::SetWidth(double width_) {
    settings.width = width_;
}

void MapRenderer::SetHeight(double height_) {
    settings.height = height_;
}

void MapRenderer::SetPadding(double padding_) {
    settings.padding = padding_;
}

void MapRenderer::SetLineWidth(double line_width_) {
    settings.line_width = line_width_;
}

void MapRenderer::SetStopRadius(double stop_radius_) {
    settings.stop_radius = stop_radius_;
}

void MapRenderer::SetBusLabelFontSize(double bus_label_font_size_) {
    settings.bus_label_font_size = bus_label_font_size_;
}

void MapRenderer::SetBusLabelOffset(std::vector<double> bus_label_offset_) {
    settings.bus_label_offset = bus_label_offset_;
}

void MapRenderer::SetUnderlayerWidth(double underlayer_width_) {
    settings.underlayer_width = underlayer_width_;
}

void MapRenderer::SetUnderlayerColor(svg::Color underlayer_color_) {
    settings.underlayer_color = underlayer_color_;
}

void MapRenderer::SetColorPalette(std::vector<svg::Color> color_palette_) {
    settings.color_palette = color_palette_;
}

void MapRenderer::SetStopLabelFontSize(double stop_label_font_size_) {
    settings.stop_label_font_size = stop_label_font_size_;
}

void MapRenderer::SetStopLabelOffset(std::vector<double> stop_label_offset_) {
    settings.stop_label_offset = stop_label_offset_;
}

void MapRenderer::SetMapSettings(MapSettings settings_) {
    settings = settings_;
}


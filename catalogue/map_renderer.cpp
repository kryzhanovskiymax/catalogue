#include "map_renderer.hpp"

using namespace transport_catalogue;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::map_renderer;

void MapRenderer::CreateMap(std::vector<std::string, std::pair<bool, std::vector<std::string>>> buses, std::vector<std::pair<std::string, Coordinates>> stops) {
    
}

void MapRenderer::CalculateCoefficients(std::vector<Coordinates> coordinates_container) {
    std::vector<double> latitudes;
    std::vector<double> longitudes;
    
    for(const auto& coordinate : coordinates_container) {
        latitudes.push_back(coordinate.lat);
        longitudes.push_back(coordinate.lng);
    }
    
    min_lat = *std::min(latitudes.begin(), latitudes.end());
    max_lat = *std::max(latitudes.begin(), latitudes.end());
    min_lon = *std::min(longitudes.begin(), longitudes.end());
    max_lon = *std::max(longitudes.begin(), longitudes.end());
    
    
    height_zoom_coef = (height - 2*padding)/(max_lat - min_lat);
    width_zoom_coef = (width - 2*padding)/(max_lon - min_lon);
}

svg::Point MapRenderer::TransformCoordinateToPoint(Coordinates coordinates) {
    double zoom_coef;
    
    if(height_zoom_coef == 0) {
        zoom_coef = width_zoom_coef;
    } else if(width_zoom_coef == 0) {
        zoom_coef = height_zoom_coef;
    } else {
        if(height_zoom_coef > width_zoom_coef) {
            zoom_coef = width_zoom_coef;
        } else {
            zoom_coef = height_zoom_coef;
        }
    }
    
    double x = (coordinates.lng - min_lon)*zoom_coef + padding;
    double y = (max_lat - coordinates.lat)*zoom_coef + padding;
    
    return svg::Point{x, y};
}

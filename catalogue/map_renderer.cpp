#include "map_renderer.hpp"

using namespace transport_catalogue;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::map_renderer;

void MapRenderer::CreateMap(std::vector<transport_catalogue::detail::Stop> stops, std::vector<transport_catalogue::detail::Bus> buses) {
    std::vector<Coordinates> stops_coordinates_;
    
    for(const auto& stop : stops) {
        stops_coordinates_.push_back(stop.coordinates);
    }
    
    CalculateCoefficients(stops_coordinates_);
    
    for(const auto& stop : stops) {
        stop_to_position.insert({stop.name, TranslateCoordinatesToPoint(stop.coordinates)});
    }
    
    for(const auto& bus : buses) {
        std::vector<std::string> stop_names;
        for(const auto& stop : bus.stops) {
            stop_names.push_back(stop->name);
        }
        
        bus_to_stops.insert({bus.name, std::make_pair(bus.is_round_trip, stop_names)});
    }
}

MapSettings MapRenderer::GetMapSettings() const {
    return settings;
}

void MapRenderer::ShowContainers() const {
    for(const auto& [stop, position] : stop_to_position) {
        std::cout << "{ " << stop << ": " << position.x <<", " << position.y << " }" << std::endl;
    }
    
    for(const auto& [bus, stops] : bus_to_stops) {
        std::cout << "{ " << bus << ": [";
        for(const auto& stop : stops.second) {
            std::cout << stop << ", ";
        }
        std::cout << "] }" << std::endl;
    }
}

void MapRenderer::ShowCoefficients() const {
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "max_lat: " << coefficients.max_lat << std::endl;
    std::cout << "min_lat: " << coefficients.min_lat << std::endl;
    std::cout << "max_lng: " << coefficients.max_lng << std::endl;
    std::cout << "min_lng: " << coefficients.min_lng << std::endl;
    std::cout << "zoom_coef: " << coefficients.zoom_coef << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
}

void MapRenderer::DrawMap(svg::Document& map) {
    DrawBusRoutes(map);
    DrawBusNames(map);
    DrawStopMarks(map);
    DrawStopNames(map);
}

void MapRenderer::DrawBusRoutes(svg::Document& map) {
    for(const auto& [bus, detail] : bus_to_stops) {
        if(detail.second.size() > 0) {
            bool is_round_trip = detail.first;
            std::vector<std::string> stops = detail.second;
            svg::Polyline route;
            
            route.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
            route.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
            route.SetFillColor(svg::NoneColor);
            route.SetStrokeWidth(settings.line_width);
            svg::Color color = GetColor();
            route.SetStrokeColor(color);
            bus_route_to_color.insert({bus, color});
            
            
            if(is_round_trip) {
                for(const auto& stop : stops) {
                    if(stop_to_position.count(stop) > 0) {
                        route.AddPoint(stop_to_position.at(stop));
                    }
                }
            } else {
                for(const auto& stop : stops) {
                    if(stop_to_position.count(stop) > 0) {
                        route.AddPoint(stop_to_position.at(stop));
                    }
                }
                
                
            }
            
            map.Add(route);
        }
    }
}

void MapRenderer::DrawBusNames(svg::Document& map) {
    for(const auto& [bus, detail] : bus_to_stops) {
        if(detail.second.size() > 0) {
            svg::Text name;
            svg::Text underlayer;
            
            svg::Point offset{settings.bus_label_offset[0], settings.bus_label_offset[1]};
            svg::Point position{stop_to_position.at(detail.second[detail.second.size() - 1])};
            
            name.SetPosition(position).SetOffset(offset).SetFontSize(settings.bus_label_font_size).SetFontFamily("Verdana").SetFontWeight("bold").SetData(bus);

            if(bus_route_to_color.count(bus) > 0) {
                svg::Color color = bus_route_to_color.at(bus);
                name.SetFillColor(color);
            }
            
            underlayer.SetPosition(position).SetOffset(offset).SetFontSize(settings.bus_label_font_size).SetFontFamily("Verdana").SetFontWeight("bold").SetData(bus);
            
            underlayer.SetFillColor(settings.underlayer_color).SetStrokeColor(settings.underlayer_color).SetStrokeWidth(settings.underlayer_width).SetStrokeLineCap(svg::StrokeLineCap::ROUND).SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
            
            if(detail.first) {
                svg::Text begin_name;

                if(stop_to_position.count(detail.second[0])) {
                    begin_name.SetPosition(stop_to_position.at(detail.second[0]));
                }
                
                begin_name.SetOffset(offset);
                begin_name.SetFontSize(settings.bus_label_font_size).SetFontWeight("bold").SetFontFamily("Verdana").SetData(bus);
                
                if(bus_route_to_color.count(detail.second[detail.second.size()-1])) {
                    begin_name.SetFillColor(bus_route_to_color.at(detail.second[detail.second.size()-1]));
                }
                
                map.Add(underlayer);
                map.Add(begin_name);
                map.Add(name);
            } else {
                map.Add(underlayer);
                map.Add(name);
            }
            
        }
    }
}

void MapRenderer::DrawStopMarks(svg::Document& map) {
    for(const auto& [stop, position] : stop_to_position) {
        svg::Circle stop_mark;
        stop_mark.SetCenter(position);
        stop_mark.SetFillColor("white");
        stop_mark.SetRadius(settings.stop_radius);
        map.Add(stop_mark);
    }
}

void MapRenderer::DrawStopNames(svg::Document& map) {
    for(const auto& [stop, position] : stop_to_position) {
        svg::Text stop_name;
        svg::Text underlayer;
        
        svg::Point offset{settings.stop_label_offset[0], settings.stop_label_offset[1]};
        stop_name.SetPosition(position).SetOffset(offset).SetFontSize(settings.stop_label_font_size).SetFontFamily("Verdana").SetData(stop);
        stop_name.SetFillColor("black");
        
        underlayer.SetPosition(position).SetOffset(offset).SetFontSize(settings.stop_label_font_size).SetFontFamily("Verdana").SetData(stop);
        underlayer.SetStrokeColor(settings.underlayer_color).SetFillColor(settings.underlayer_color).SetStrokeWidth(settings.underlayer_width).SetStrokeLineJoin(svg::StrokeLineJoin::ROUND).SetStrokeLineCap(svg::StrokeLineCap::ROUND);
        
        map.Add(underlayer);
        map.Add(stop_name);
    }
}

void MapRenderer::CalculateCoefficients(std::vector<Coordinates> stops_coordinates) {
    std::vector<double> longitudes;
    std::vector<double> latitudes;
    
    for(const auto& coordinates : stops_coordinates) {
        longitudes.push_back(coordinates.lng);
        latitudes.push_back(coordinates.lat);
    }
    
    coefficients.min_lat = *std::min(latitudes.begin(), latitudes.end());
    coefficients.min_lng = *std::min(longitudes.begin(), longitudes.end());
    
    for(const double& lat : latitudes) {
        if(lat >= coefficients.max_lat) {
            coefficients.max_lat = lat;
        }
    }
    
    for(const double& lng : longitudes) {
        if(lng >= coefficients.max_lng) {
            coefficients.max_lng = lng;
        }
    }
    
    
    double height_zoom_coef = (settings.height - 2*settings.padding)/(coefficients.max_lat - coefficients.min_lat);
    double width_zoom_coef = (settings.width - 2*settings.padding)/(coefficients.max_lng - coefficients.min_lng);
    
    if(height_zoom_coef == 0) {
        coefficients.zoom_coef = width_zoom_coef;
    } else if(width_zoom_coef == 0) {
        coefficients.zoom_coef = height_zoom_coef;
    } else {
        coefficients.zoom_coef = std::min(height_zoom_coef, width_zoom_coef);
    }
}

svg::Point MapRenderer::TranslateCoordinatesToPoint(Coordinates coordinates) {
    double x = (coordinates.lng - coefficients.min_lng)*coefficients.zoom_coef + settings.padding;
    double y = (coefficients.max_lat - coordinates.lat)*coefficients.zoom_coef + settings.padding;
    
    return svg::Point{x, y};
}

void MapRenderer::SetMapSettings(MapSettings settings_) {
    settings = settings_;
}

svg::Color MapRenderer::GetColor() {
    svg::Color result = settings.color_palette[color_idx%settings.color_palette.size()];
    ++color_idx;
    return result;
}


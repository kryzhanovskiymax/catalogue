#include "map_renderer.hpp"

using namespace transport_catalogue;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::map_renderer;

void MapRenderer::CreateMap(std::vector<transport_catalogue::detail::Stop> stops, std::vector<transport_catalogue::detail::Bus> buses) {
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

void MapRenderer::DrawMap() {
    DrawBusRoutes();
    DrawBusNames();
    DrawStopMarks();
    DrawStopNames();
}

void MapRenderer::DrawBusRoutes() {
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
                    route.AddPoint(stop_to_position.at(stop));
                }
            } else {
                for(const auto& stop: stops) {
                    route.AddPoint(stop_to_position.at(stop));
                }
                
                for(unsigned long int i = stops.size() - 2; i >= 0; --i) {
                    route.AddPoint(stop_to_position.at(stops[i]));
                }
            }
            
            map_document.Add(route);
        }
    }
}

void MapRenderer::DrawBusNames() {
    for(const auto& [bus, detail] : bus_to_stops) {
        if(detail.second.size() > 0) {
            svg::Text name;
            svg::Text underlayer;
            
            svg::Point offset{settings.bus_label_offset[0], settings.bus_label_offset[1]};
            svg::Point position{stop_to_position.at(detail.second[detail.second.size() - 1])};
            
            name.SetPosition(position).SetOffset(offset).SetFontSize(settings.bus_label_font_size).SetFontFamily("Verdana").SetFontWeight("bold").SetData(bus);
            
            name.SetFillColor(bus_route_to_color.at(detail.second[detail.second.size()-1]));

            underlayer.SetPosition(position).SetOffset(offset).SetFontSize(settings.bus_label_font_size).SetFontFamily("Verdana").SetFontWeight("bold").SetData(bus);
            
            underlayer.SetFillColor(settings.underlayer_color).SetStrokeColor(settings.underlayer_color).SetStrokeWidth(settings.underlayer_width).SetStrokeLineCap(svg::StrokeLineCap::ROUND).SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
            
            if(detail.first) {
                svg::Text begin_name;
                begin_name.SetPosition(stop_to_position.at(detail.second[0]));
                begin_name.SetOffset(offset);
                begin_name.SetFontSize(settings.bus_label_font_size).SetFontWeight("bold").SetFontFamily("Verdana").SetData(bus);
                begin_name.SetFillColor(bus_route_to_color.at(detail.second[detail.second.size()-1]));
                map_document.Add(underlayer);
                map_document.Add(begin_name);
                map_document.Add(name);
            } else {
                map_document.Add(underlayer);
                map_document.Add(name);
            }
            
        }
    }
}

void MapRenderer::DrawStopMarks() {
    for(const auto& [stop, position] : stop_to_position) {
        svg::Circle stop_mark;
        stop_mark.SetCenter(position);
        stop_mark.SetFillColor("white");
        stop_mark.SetRadius(settings.stop_radius);
        map_document.Add(stop_mark);
    }
}

void MapRenderer::DrawStopNames() {
    for(const auto& [stop, position] : stop_to_position) {
        svg::Text stop_name;
        svg::Text underlayer;
        
        svg::Point offset{settings.stop_label_offset[0], settings.stop_label_offset[1]};
        stop_name.SetPosition(position).SetOffset(offset).SetFontSize(settings.stop_label_font_size).SetFontFamily("Verdana").SetData(stop);
        stop_name.SetFillColor("black");
        
        underlayer.SetPosition(position).SetOffset(offset).SetFontSize(settings.stop_label_font_size).SetFontFamily("Verdana").SetData(stop);
        underlayer.SetStrokeColor(settings.underlayer_color).SetFillColor(settings.underlayer_color).SetStrokeWidth(settings.underlayer_width).SetStrokeLineJoin(svg::StrokeLineJoin::ROUND).SetStrokeLineCap(svg::StrokeLineCap::ROUND);
        
        map_document.Add(underlayer);
        map_document.Add(stop_name);
    }
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

void MapRenderer::SetMapSettings(MapSettings settings_) {
    settings = settings_;
}

svg::Color MapRenderer::GetColor() {
    svg::Color result = settings.color_palette[color_idx/settings.color_palette.size()];
    ++color_idx;
    return result;
}


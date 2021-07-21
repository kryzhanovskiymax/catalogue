#include "map_renderer.hpp"

using namespace transport_catalogue;
using namespace transport_catalogue::map_renderer;

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

void MapRenderer::SetUnderlayerColor(double underlayer_color_) {
    underlayer_color = underlayer_color_;
}

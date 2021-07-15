#include "svg.hpp"

namespace svg {

using namespace std::literals;

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);

    context.out << std::endl;
}

// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\" "sv;
    out << "/>"sv;
}
    
    Polyline& Polyline::AddPoint(Point point) {
        points_.push_back(point);
        return *this;
    }
    
    void Polyline::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out<<"<polyline points=\""sv;
        for(size_t i = 0; i < points_.size(); ++i) {
            out<<points_[i].x<<","sv<<points_[i].y;
            if(i != points_.size() - 1) {
                out<<" "sv;
            }
        }
        out<<"\"/>"sv;
    }
    
    Text& Text::SetPosition(Point pos) {
        position_ = pos;
        return *this;
    }

    // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
    Text& Text::SetOffset(Point offset) {
        offset_ = offset;
        return *this;
    }

    // Задаёт размеры шрифта (атрибут font-size)
    Text& Text::SetFontSize(uint32_t size) {
        font_size_ = size;
        return *this;
    }

    // Задаёт название шрифта (атрибут font-family)
    Text& Text::SetFontFamily(std::string font_family) {
        font_family_ = font_family;
        return *this;
    }

    // Задаёт толщину шрифта (атрибут font-weight)
    Text& Text::SetFontWeight(std::string font_weight) {
        font_weight_ = font_weight;
        return *this;
    }

    // Задаёт текстовое содержимое объекта (отображается внутри тега text)
    Text& Text::SetData(std::string data) {
        data_ = data;
        return *this;
    }
    
    void Text::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out<<"<text x=\""<<position_.x<<"\" y=\""<<position_.y<<"\" dx=\""<<offset_.x<<"\" dy=\""<<offset_.y<<"\" font-size=\""<<font_size_<<"\"";
        if(!font_family_.empty()) {
            out<<" font-family=\""<<font_family_<<"\"";
        }
        if(!font_weight_.empty()) {
            out<<" font-weight=\""<<font_weight_<<"\"";
        }
        out << ">"<<data_<<"</text>";
    }
    
    void Document::AddPtr(std::unique_ptr<Object>&& obj) {
        objects_.push_back(std::move(obj));
    }

    void Document::Render(std::ostream& out) const {
        
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;

        for(const auto& object : objects_) {
            object->Render(out);
        }

        out << "</svg>"sv;
        
        
    }

}  // namespace svg

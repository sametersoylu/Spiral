#include "window.h"
#include <Gorgon/CGI.h>
#include <Gorgon/CGI/Line.h>
#include <Gorgon/Geometry/Line.h>
#include <Gorgon/Geometry/Point.h>
#include <Gorgon/Geometry/PointList.h>
#include <Gorgon/Graphics/Bitmap.h>
#include <Gorgon/Graphics/Color.h>
#include <Gorgon/Graphics/Line.h>
#include <Gorgon/Types.h>
#include <Gorgon/Widgets/Textarea.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <numbers>
#include <ostream>
#include <sstream>
#include <format>

std::string format_line(Gorgon::Geometry::Line<> line, std::string header = "") {
    return  std::format("{}: \nStart: '{}, {}', End: '{}, {}'", header, line.Start.X, line.Start.Y, line.End.X, line.End.Y); 
} 

template<class T>
Gorgon::Widgets::Textarea& operator<<(Gorgon::Widgets::Textarea &textbox, T item) {
    textbox.SetText(std::format("{}{}", textbox.GetText(), item));
    return textbox;
}

class basic_newline {};
basic_newline NewLine; 
template<>
struct std::formatter<basic_newline, char>{
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin(); 
    }

    auto format(const basic_newline& obj, format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", "\n"); 
    }
};





void Application::AppWindow::Run() {

    auto w = Width.Get(), h = Height.Get(); 
    bmp1.Resize(w,h,Gorgon::Graphics::ColorMode::Grayscale);
    bmp1.Clear(); 
    layer.Clear();

    int r = r0.Get(); 
    int Ri = ri.Get();

    int N = n.Get(); 

    float deg = (std::numbers::pi*2)/24; 

    Gorgon::Geometry::Point center { w/2, h/2 }; 
    Gorgon::Geometry::Point last = center; 
    std::vector<Gorgon::Geometry::Line<>> linelist;


    for(int n = 0; n < N; n++) {
        for(int i = 0; i < 24; i++) {
            Gorgon::Geometry::Line<> line; 
            line.Start = last; 
            line.End = {std::round(r*cos(deg*i)) + center.X, std::round(r*sin(deg*i)) + center.Y}; 
            last = line.End; 
            
            linelist.emplace_back(line);
            r += ri;
            //asdasdas
        }
    }

    std::for_each(begin(linelist), end(linelist), [this](Gorgon::Geometry::Line<> line) {
        Gorgon::Geometry::PointList<> plist; 
        plist.Push(line.Start);
        plist.Push(line.End);
        Gorgon::CGI::DrawLines(bmp1,plist,1.0, Gorgon::CGI::SolidFill<>(Gorgon::Graphics::Color::White));
    });


    
    // Draw 
    bmp1.Prepare();
    bmp1.Draw(layer, 0, 0); 

}
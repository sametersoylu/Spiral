#include <Gorgon/Graphics/Color.h>
#include <Gorgon/Graphics/ColorSpaces.h>
#include <Gorgon/Graphics/Layer.h>
#include <Gorgon/UI/Window.h>
#include "Gorgon/Widgets/Textarea.h"
#include <Gorgon/UI/Window.h>
#include <Gorgon/UI/Organizers/Flow.h>
#include <Gorgon/Graphics/Bitmap.h>
#include <Gorgon/Widgets/Inputbox.h>
#include <Gorgon/Widgets/Panel.h>
#include <Gorgon/Widgets/Layerbox.h>
#include <Gorgon/Widgets/Button.h>
#include <Gorgon/Widgets/GeometryBoxes.h>
#include <Gorgon/UI/Dialog.h>
#include <Gorgon/UI.h>
#include <pulse/mainloop-signal.h>


namespace Application {
    namespace UI = Gorgon::UI;
    namespace Widget = Gorgon::Widgets;
    using namespace UI::literals; 

    class AppWindow {
        private: 
        UI::Window& window; 
        Widget::Panel side; 
        UI::Organizers::Flow sideorganizer;
        Widget::Layerbox layerbox;
        Gorgon::Graphics::Layer layer; 
        Gorgon::Widgets::Button run, zoomin, zoomout, resize; 
        Gorgon::Graphics::Bitmap bmp1, zoomed; 
        Gorgon::Widgets::Textarea TextBox1; 
        Gorgon::Widgets::Inputbox<int> r0, ri, n, Width, Height; 
        
        static constexpr std::array<int, 8> zoomrates = {1,2,3,4,6,8,16,32}; 

        int zoom = 0; 
        public: 
        void Run(); 

        bool Quit() {
            return true; 
        }

        void Redraw() {
              layer.Clear();

            if (bmp1.HasData()) {
                auto rate = zoomrates[zoom];
                zoomed = bmp1.ZoomMultiple(rate);

                if (zoom > 4) {
                    zoomed.ForAllPixels([rate, this](int x, int y) {
                        if (x % rate == 0 || y % rate == 0)
                        zoomed(x, y, 0) = 64 + zoomed(x, y, 0) / 2;
                    });
                }

                zoomed.Prepare();
                zoomed.Draw(layer, 0, 0);
                zoomed.DrawStretched(layer, 0, 0, layer.GetSize());
            }
        }

        explicit AppWindow(UI::Window &window) : window(window) {
            window.Add(side);
            side.SetInteriorWidth(8_u);
            side.SetHeight(100_perc);
            side.AttachOrganizer(sideorganizer);

            window.AddNextTo(layerbox);
            layerbox.SetWidth(window.GetInteriorSize().Width -
                              layerbox.GetCurrentLocation().X);
            layerbox.SetHeight(100_perc);

            window.ResizedEvent.Register([this] {
              layerbox.SetWidth(this->window.GetInteriorSize().Width -
                                layerbox.GetCurrentLocation().X);
            });

            layerbox.GetLayer().Add(layer);

            run.Text = "Run";
            window.SetDefault(run);
            run.SetWidth(8);
            run.ClickEvent.Register(*this, &AppWindow::Run);


            zoomin.Text = "Zoom in";
            zoomin.SetWidth(4);
            zoomin.ClickEvent.Register([this] {
                if (zoom < zoomrates.size() - 1) {
                    zoom++;
                    Redraw();
                }
            });

            zoomout.Text = "Zoom out";
            zoomout.SetWidth(4);
            zoomout.ClickEvent.Register([this] {
                if (zoom > 0) {
                    zoom--;
                    Redraw();
                }
            });

            resize.Text = "Resize"; 
            resize.ClickEvent.Register([this]() -> void {
                bmp1.Resize(Width.Get(), Height.Get(), Gorgon::Graphics::ColorMode::Grayscale);
                //bmp1.Clear();
                Redraw();
            });



            buildsettingsui();
        }
        void buildsettingsui() {
            r0.Set(5); 
            ri.Set(1);
            n.Set(10); 
            Width.Set(500); 
            Height.Set(500); 
            sideorganizer // Starting from here enter widgets
            << 8 << run << sideorganizer.Break
            << 4 << zoomin << 4 << zoomout << sideorganizer.Break
            << 8 << resize << sideorganizer.Break
            << 2 << "Width: " << 2 << Width << 2 << "Height: " << 2 << Height << sideorganizer.Break
            << 1 << "r0: " << 2 << r0 << 1 << "ri: " << 2 << ri << sideorganizer.Break
            << 1 << "n: " << 2 << n << sideorganizer.Break   
            << 4 << "Output; " << sideorganizer.Break
            << 8 << TextBox1 << sideorganizer.Break
            ;
        }
    }; 
}

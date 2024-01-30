#include <Gorgon/Main.h>
#include <Gorgon/UI.h>
#include <Gorgon/UI/Window.h>
#include <algorithm>
#include <vector>
#include <string>
#include "window.h"

void arg_handle(std::vector<std::string> args) {
    static bool gui = false; 
    std::for_each(begin(args), end(args), [](std::string arg) {
        if(arg == "-g" or arg == "--gui") {
            gui = true ;
        }
    }); 
    
    if(gui) {
        Gorgon::Initialize("Spiral"); 

        Gorgon::UI::Window window({1200, 900}, "Spiral");

        Gorgon::UI::Initialize(); 

        Application::AppWindow app(window); 
        window.AllowResize(); 

        window.ClosingEvent.Register([&app](bool &allow) {
            allow = app.Quit();
        });

        window.Run();
    }
}

int main(int argc, char **argv) {
    std::string executableName = argv[0]; 

    // Arguments C vector to C++ vector
    std::vector<std::string> args; 
    args.assign(argv+1, argv + argc); 

    args.push_back("-g");

    arg_handle(args); 
    


    return 0; 
}

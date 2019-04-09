// c++ -std=c++17 -o transient transient.cc -lX11
//
#include <iostream>
#include <cstdint>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(int argc, char** argv) {
    if (auto d = XOpenDisplay(nullptr); !d) {
        return 1;
    } else {
        XSizeHints h;
        XEvent e;
        auto r = DefaultRootWindow(d);
        auto f = XCreateSimpleWindow(d, r, 100, 100, 400, 400, 0, 0, 0);
        Window t = None;
        h.min_width = 400;
        h.max_width = 400;
        h.min_height = 400;
        h.max_height = 400;
        h.flags = PMinSize | PMaxSize;
        XSetWMNormalHints(d, f, &h);
        XStoreName(d, f, "floating");
        XMapWindow(d, f);

        XSelectInput(d, f, ExposureMask);
        while (true) {
            XNextEvent(d, &e);
            if (t == None) {
                sleep(5);
                t = XCreateSimpleWindow(d, r, 50, 50, 100, 100, 0, 0, 0);
                XSetTransientForHint(d, t, f);
                XStoreName(d, t, "transient");
                XMapWindow(d, t);
                XSelectInput(d, t, ExposureMask);
            }
        }
        XCloseDisplay(d);
        exit(0);
    }
}

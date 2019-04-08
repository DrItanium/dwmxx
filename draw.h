#ifndef DWMXX_DRAW_H__
#define DWMXX_DRAW_H__
#include <iostream>
#include <string>
#include <list>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include "util.h"

namespace X
{
    using Cursor = ::Cursor;
    using Display = ::Display;
    using Window = ::Window;
    using Drawable = ::Drawable;
    using GC = ::GC;
}
namespace Xft
{
    using Color = ::XftColor;
}
namespace dwm {
enum class ColorSchemeIndex
{
    Foreground,
    Background,
    Border,
};
struct Cursor {
    X::Cursor _cursor;
};

struct Font {
    X::Display* _display = nullptr;
    unsigned int _h;
    Xft::Font* _xfont = nullptr;
    FcPattern* _pattern = nullptr;
    ~Font();
};

class Draw {
public:
private:
    unsigned int _w, _h;
    X::Display* _dpy;
    int _screen;
    X::Window _root;
    X::Drawable _drawable;
};


} // end namespace dwm

#endif // end DRAW_H__

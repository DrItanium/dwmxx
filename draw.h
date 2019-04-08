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
    using Font = ::XftFont;
}
namespace Fontconfig
{
    using Pattern = ::FcPattern;
} // end namespace Fontconfig
namespace dwm {
enum class ColorSchemeIndex
{
    Foreground,
    Background,
    Border,
};
using Color = Xft::Color;
struct Cursor {
    X::Cursor _cursor;
};

class Font {
    Font(X::Display& disp, unsigned int h, Xft::Font* font, Fontconfig::Pattern* pattern);
    ~Font();
    X::Display& getDisplay() noexcept { return _display; }
    auto getH() const noexcept { return _h; }
    auto getXFont() const noexcept { return _xfont; }
    auto getPattern() const noexcept { return _pattern; }
private:
    X::Display& _display;
    unsigned int _h;
    Xft::Font* _xfont = nullptr;
    Fontconfig::Pattern* _pattern = nullptr;
};

class Draw {
public:
    using FontList = std::list<Font>;
    using FontIterator = FontList::iterator;
    using ConstFontIterator = FontList::const_iterator;
public:
    Draw(X::Display* display, int screen, X::Window win, unsigned int w, unsigned int h);
    ~Draw();
    void resize(unsigned int w, unsigned int h);
    auto getW() const noexcept { return _w; }
    auto getH() const noexcept { return _h; }
    auto getScreen() const noexcept { return _screen; }
    X::Window& getRoot() noexcept { return _root; }
    X::Drawable& getDrawable() noexcept { return _drawable; }
    X::GC& getGC() noexcept { return _gc; }
    auto begin() noexcept { return _fonts.begin(); }
    auto begin() const noexcept { return _fonts.begin(); }
    auto cbegin() const noexcept { return _fonts.cbegin(); }
    auto end() noexcept { return _fonts.end(); }
    auto end() const noexcept { return _fonts.end(); }
    auto cend() const noexcept { return _fonts.cend(); }
    void installFont(const std::string& font) noexcept;
    void installFont(const std::initializer_list<std::string>& fonts) noexcept;
private:
    unsigned int _w, _h;
    X::Display& _dpy;
    int _screen;
    X::Window _root;
    X::Drawable _drawable;
    X::GC _gc;
    std::list<Color> _colorscheme;
    std::list<Font> _fonts;
    
};


} // end namespace dwm

#endif // end DRAW_H__

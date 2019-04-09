#ifndef DWMXX_DRAW_H__
#define DWMXX_DRAW_H__
#include <iostream>
#include <string>
#include <list>
#include <tuple>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include "util.h"

namespace X {
    using Cursor = ::Cursor;
    using Display = ::Display;
    using Window = ::Window;
    using Drawable = ::Drawable;
    using GC = ::GC;
}

namespace Xft {
    using Color = ::XftColor;
    using Font = ::XftFont;
    using ::XftFontClose;
}
namespace Fontconfig {
    using Pattern = ::FcPattern;
    using ::FcPatternDestroy;
    //using Bool = ::FcBool;
    //using ::FcPatternGetBool;
} // end namespace Fontconfig

namespace dwm {
enum class ColorschemeIndex {
    Foreground,
    Background,
    Border,
};
using Color = Xft::Color;
struct Draw;
struct Cursor {
    X::Display& _display;
    X::Cursor _cursor;
    Cursor(X::Display& d, int shape);
    ~Cursor();
};

class Font {
public:
    Font(X::Display& disp, unsigned int h, Xft::Font* font, Fontconfig::Pattern* pattern) : _display(disp), _h(h), _xfont(font), _pattern(pattern) { }
    ~Font();
    X::Display& getDisplay() noexcept { return _display; }
    const X::Display& getDisplay() const noexcept { return _display; }
    auto getH() const noexcept { return _h; }
    auto getXFont() noexcept { return _xfont; }
    auto getPattern() noexcept { return _pattern; }
    void getExts(const std::string& text, unsigned int* w, unsigned int* h);
    std::tuple<unsigned int, unsigned int> getExts(const std::string& text);
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
    using Colorscheme = std::list<Color>;
    using ColorschemeIterator= Colorscheme::iterator;
    using ConstColorschemeIterator = Colorscheme::const_iterator;
public:
    Draw(X::Display& display, int screen, X::Window win, unsigned int w, unsigned int h);
    ~Draw();
    void resize(unsigned int w, unsigned int h);
    unsigned int getFontWidth(const std::string& name) const noexcept;

public: // extra helpers
    auto getW() const noexcept { return _w; }
    auto getH() const noexcept { return _h; }
    auto getScreen() const noexcept { return _screen; }
    X::Window& getRoot() noexcept { return _root; }
    X::Drawable& getDrawable() noexcept { return _drawable; }
    X::GC& getGC() noexcept { return _gc; }
public: // font set related operations
    auto font_begin() noexcept { return _fonts.begin(); }
    auto font_begin() const noexcept { return _fonts.begin(); }
    auto font_cbegin() const noexcept { return _fonts.cbegin(); }
    auto font_end() noexcept { return _fonts.end(); }
    auto font_end() const noexcept { return _fonts.end(); }
    auto font_cend() const noexcept { return _fonts.cend(); }
    void installFont(const std::string& font) noexcept;
    void installFont(const std::initializer_list<std::string>& fonts) noexcept;
public: // colorscheme related
    
public: // cursor related

public: // manipulators
    void setFonts(FontList& fonts);
    void setColorscheme(Colorscheme& scheme);
public: 
    void rect(int x, int y, unsigned int w, unsigned int h, bool filled, bool invert);
    int text(int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const std::string& text, bool invert);
public: // map functions
    void map(X::Window win, int x, int y , unsigned int w, unsigned int h);
private:
    unsigned int _w, _h;
    X::Display& _display;
    int _screen;
    X::Window _root;
    X::Drawable _drawable;
    X::GC _gc;
    std::list<Color> _colorscheme;
    std::list<Font> _fonts;
};


} // end namespace dwm

#endif // end DRAW_H__

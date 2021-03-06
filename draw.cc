#include "draw.h"
#include "util.h"

namespace X {
    using GlyphInfo = ::XGlyphInfo;
} // end namespace X

namespace Xft {
    using Char8 = ::XftChar8;
} // end namespace Xft

namespace dwm {
namespace utf8 {
    constexpr auto invalid = 0xFFFD;
    constexpr auto size = 4;
    constexpr unsigned char bytes[] = {0x80, 0, 0xc0, 0xe0, 0xf0 };
    constexpr unsigned char mask[] = {0xc0, 0x80, 0xe0, 0xf0, 0xf8};

    constexpr long min[] = {        0,    0,  0x80,  0x800,  0x1'0000};
    constexpr long max[] = {0x10'FFFF, 0x7f, 0x7ff, 0xffff, 0x10'ffff};
    long decodeByte(const char c, size_t* i) {
        for (*i = 0; *i < (size + 1); ++(*i)) {
            if (((unsigned char)c & mask[*i]) == bytes[*i]) {
                return (unsigned char)c & ~mask[*i];
            }
        }
        return 0;
    }
    size_t validate(long* u, size_t i) {
        if (between(*u, min[i], max[i]) || between(*u, 0xD800, 0xDFFF)) {
            *u = invalid;
        }
        for (i = 1; *u > max[i]; ++i);
        return i;

    }

    size_t decode(const char* c, long* u, size_t clen) {
        *u = invalid;
        if (!clen) {
            return 0;
        }
        size_t len = 0, type = 0, j = 0, i = 0;
        long udecoded = decodeByte(c[0], &len);
        if (between(len, 1, size)) {
            return 1;
        } 
        for (i = 1, j = 1; i < clen && j < len; ++i, ++j) {
            udecoded = (udecoded << 6) | decodeByte(c[i], &type);
            if (type) {
                return j;
            }
        }
        if (j < len) {
            return 0;
        }
        *u = udecoded;
        validate(u, len);
        return len;
    }
} // end namespace utf8

Font::~Font() {
    if (_pattern) {
        Fontconfig::FcPatternDestroy(_pattern);
        _pattern = nullptr;
    }
    Xft::XftFontClose(&_display, _xfont);
    _xfont = nullptr;
}

Draw::Draw(X::Display& disp, int screen, X::Window root, unsigned int w, unsigned int h)
    : _display(disp), _screen(screen), _root(root), _w(w), _h(h) {
        _drawable = XCreatePixmap(&_display, _root, _w, _h, DefaultDepth(&_display, _screen));
        _gc = XCreateGC(&_display, _root, 0, nullptr);
        XSetLineAttributes(&_display, _gc, 1, LineSolid, CapButt, JoinMiter);
}

void
Draw::resize(unsigned int w, unsigned int h)
{
    _w = w;
    _h = h;
    if (_drawable) {
        XFreePixmap(&_display, _drawable);
    }
    _drawable = XCreatePixmap(&_display, _root, _w, _h, DefaultDepth(&_display, _screen));
}

Draw::~Draw()
{
    XFreePixmap(&_display, _drawable);
    XFreeGC(&_display, _gc);
}

Cursor::Cursor(X::Display& disp, int shape) : _display(disp), _cursor(XCreateFontCursor(&disp, shape)) { }

Cursor::~Cursor() {
    XFreeCursor(&_display, _cursor);
}

std::tuple<unsigned int, unsigned int>
Font::getExts(const std::string& text) {
    
    X::GlyphInfo ext;
    XftTextExtentsUtf8(&_display, _xfont, (Xft::Char8*)text.c_str(), text.size(), &ext);
    return std::tuple<unsigned int, unsigned int>(ext.xOff, _h);
}

void
Font::getExts(const std::string& text, unsigned int* w, unsigned int* h) {
    const auto [a,b] = getExts(text);
    if (w) {
        *w = a;
    } 
    if (h) {
        *h = b;
    }
}

unsigned int
Draw::getWidth(const std::string& text) noexcept {
    if (_fonts.empty()) {
        return 0;
    } else {
        return this->text(0, 0, 0, 0, 0, text, 0);
    }
}

void
Draw::map(X::Window win, int x, int y, unsigned int w, unsigned int h) {
    XCopyArea(&_display, _drawable, win, _gc, x, y, w, h, x, y);
    XSync(&_display, False);
}

void
Draw::rect(int x, int y, unsigned int w, unsigned int h, bool filled, bool invert) {
    XSetForeground(&_display, _gc, invert ? getBackgroundColor().pixel : getForegroundColor().pixel);
    if (filled) {
        XFillRectangle(&_display, _drawable, _gc, x, y , w, h);
    } else {
        XDrawRectangle(&_display, _drawable, _gc, x, y, w - 1, h - 1);
    }
}

void
Draw::setColorscheme(Colorscheme& scheme) {

}

} // end namespace dwm

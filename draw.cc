#include "draw.h"
#include "util.h"

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
            if (((unsigned char)c & mask[*i]) == byte[*i]) {
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
    }
} // end namespace utf8
} // end namespace dwm

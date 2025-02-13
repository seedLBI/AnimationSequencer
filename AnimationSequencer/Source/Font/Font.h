#ifndef FONT_H
#define FONT_H

#include <robin_hood.h>
#include <fstream>
#include "Utils/Text/Utils.Text.h"
#include "Texture/Texture.h"

struct Bounds {
    float left;
    float bottom;
    float right;
    float top;
};

struct Glyph {
    float advance;
    Bounds plane;
    Bounds atlas;
};

struct Metrics {
    float pxRange = 0.f;
    float emSize = 0.f;
    float lineHeight = 0.f;
    float ascender = 0.f;
    float descender = 0.f;
    float underlineY = 0.f;
    float underlineThickness = 0.f;
};

class Font {
public:
    Font(const std::string& path2Image, const std::string& path2Data);

    Texture* GetTexture();
    Bounds GetSizeGlyph(int unicode_char);

    Glyph GetGlyphByUnicode(int unicode);
    Metrics GetMetrics();

private:
    Texture* atlas;
    Metrics metrics;
    robin_hood::unordered_flat_map<int, Glyph> glyphs;
    robin_hood::unordered_flat_map<int, robin_hood::unordered_flat_map<int, float>> kernings;
};




#endif
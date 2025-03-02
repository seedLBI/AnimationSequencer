#ifndef FONT_H
#define FONT_H

#include <robin_hood.h>
#include <fstream>
#include "Utils/Text/Utils.Text.h"
#include "Texture/Texture.h"

struct Bounds {
    float left;   //2
    float bottom; //3
    float right;  //4
    float top;    //5

    float GetX() {
        return left;
    }
    float GetY() {
        return 1.f - bottom - (top - bottom);
    }

    float GetWidth() {
        return right - left;
    }
    float GetHeight() {
        return top - bottom;
    }
};

struct Glyph {
    int unicode;
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

    float GetKerning(const int& unicode_1, const int& unicode_2);

private:
    Texture* atlas;
    Metrics metrics;
    robin_hood::unordered_flat_map<int, Glyph> glyphs;
    std::vector<std::pair<std::pair<int, int>, float>> kernings_vector;
    robin_hood::unordered_flat_map<int, robin_hood::unordered_flat_map<int, float>> kernings;
};




#endif
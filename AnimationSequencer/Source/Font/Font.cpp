#include "Font/Font.h"

#include <iostream>

Texture* Font::GetTexture() {
    return atlas;
}

Bounds Font::GetSizeGlyph(int unicode_char) {
    return glyphs[unicode_char].plane;
}
Metrics Font::GetMetrics() {
    return metrics;
}

Font::Font(const std::string& path2Image, const std::string& path2Data) {

    TextureSetting setting;
    setting.WrapX = TextureWrap::CLAMP_TO_EDGE;
    setting.WrapY = TextureWrap::CLAMP_TO_EDGE;
    setting.Min = TextureFilter::LINEAR;
    setting.Max = TextureFilter::LINEAR;

    // Step1: Load Image
    atlas = new Texture(setting, path2Image);
    

    // Step2: Load Data character
    std::ifstream ifn(path2Data);
    int countEmpty = 0;
    std::string line;

    while (std::getline(ifn, line)) {
        auto data = Split(line, ",");

        if (line.empty()) {
            countEmpty++;
            continue;
        }

        if (countEmpty == 0) {
            metrics.pxRange = stof(data[0]);
            metrics.emSize = stof(data[5]);
            metrics.lineHeight = stof(data[6]);
            metrics.ascender = stof(data[7]);
            metrics.descender = stof(data[8]);
            metrics.underlineY = stof(data[9]);
            metrics.underlineThickness = stof(data[10]);
        }
        else if (countEmpty == 1) {
            Glyph tempGlyph;

            int unicode = stoi(data[0]);
            tempGlyph.advance = stof(data[1]);


            

            if (data.size() > 2) {
                tempGlyph.plane = { stof(data[2]),stof(data[3]), stof(data[4]),stof(data[5]) };
                tempGlyph.atlas = { stof(data[6]) / (float)atlas->GetWidth(),
                                    stof(data[7]) / (float)atlas->GetHeight(),
                                    stof(data[8]) / (float)atlas->GetWidth(),
                                    stof(data[9]) / (float)atlas->GetHeight() };
            }
            else {
                tempGlyph.plane = { 0,0,0,0 };
                tempGlyph.atlas = { 0,0,0,0 };
            }

            glyphs[unicode] = tempGlyph;
        }
        else if (countEmpty == 2) {
            int unicode1 = stoi(data[0]);
            int unicode2 = stoi(data[1]);
            float advance = stof(data[2]);

            kernings[unicode1][unicode2] = advance;
        }
    }
}

Glyph Font::GetGlyphByUnicode(int unicode) {
    return glyphs[unicode];
}
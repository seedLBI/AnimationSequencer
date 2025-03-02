#include "TextCreator.h"


TextCreator::TextCreator(AnimationDrawer* animationDrawer) {
    this->animationDrawer = animationDrawer;
    fontMSDF = this->animationDrawer->GetMSDF_Font();
}

TextCreator::~TextCreator() {

}

void TextCreator::DeleteLastSymbols() {
    for (int i = 0; i < symbols_temp.size(); i++) {
        animationDrawer->Get_ptr_Quad_SymbolMSDF_instance()->DeleteSymbol(symbols_temp[i]);
        delete symbols_temp[i];
    }
    symbols_temp.clear();
}


void TextCreator::Draw() {

    static char TextInput[256] = { 0, };

    {
        static bool inited = false;
        if (inited == false) {
            for (int i = 0; i < 256; i++)
                TextInput[i] = 0;

            inited = true;
        }
    }

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);


    bool WasPressedAccept = false;

    if (ImGui::InputText("##SymbolsInputText", &TextInput[0], 256, ImGuiInputTextFlags_EnterReturnsTrue)) {
        WasPressedAccept = true;
    }

    if (ImGui::Button("Accept", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
        WasPressedAccept = true;
    }

    if (WasPressedAccept) {

        DeleteLastSymbols();

        if (TextInput[0] != 0) {

            std::string Line = TextInput;

            UTF8_SPLITER_ERROR error;
            auto symbolss = utf8_splitter(Line, error);

            std::vector<uint64_t> unicode_values(symbolss.size());
            for (int i = 0; i < symbolss.size(); i++) {
                unicode_values[i] = strUTF8Symbol_to_UnicodeValue(symbolss[i]);
            }

            for (int i = 0; i < symbolss.size(); i++)
            {
                SymbolMSDF_Data* temp = new SymbolMSDF_Data(i, fontMSDF->GetGlyphByUnicode(unicode_values[i]));
                animationDrawer->Get_ptr_Quad_SymbolMSDF_instance()->AddSymbol(temp);

                symbols_temp.push_back(temp);

            }

            for (int i = 0; i < 256; i++)
                TextInput[i] = 0;
        }
    }


    if (symbols_temp.empty() == false) {

        static float
            pos_x = 0.f,
            pos_y = 0.f,
            size_x = 80.f,
            size_y = 80.f,
            spacing = 0.f,
            thikness = 0.f;

        ImGui::SliderFloat(
            "PosX ",
            &pos_x,
            -animationDrawer->GetVirtualScreenSize().x * 2.f,
            animationDrawer->GetVirtualScreenSize().x * 2.f);
        ImGui::SliderFloat(
            "PosY ",
            &pos_y,
            -animationDrawer->GetVirtualScreenSize().y * 2.f,
            animationDrawer->GetVirtualScreenSize().y * 2.f);



        ImGui::SliderFloat(
            "SizeX",
            &size_x,
            -animationDrawer->GetVirtualScreenSize().x * 2.f,
            animationDrawer->GetVirtualScreenSize().x * 2.f);
        ImGui::SliderFloat(
            "SizeY",
            &size_x,
            -animationDrawer->GetVirtualScreenSize().x * 2.f,
            animationDrawer->GetVirtualScreenSize().x * 2.f);

        ImGui::SliderFloat(
            "Spacing",
            &spacing,
            -animationDrawer->GetVirtualScreenSize().x ,
            animationDrawer->GetVirtualScreenSize().x);

        ImGui::SliderFloat(
            "Thikness",
            &thikness,
            -10.f,
            10.f);

        size_y = size_x;

        float temp_size_x = size_x / animationDrawer->GetVirtualScreenSize().x * 2.f;
        float temp_size_y = size_y / animationDrawer->GetVirtualScreenSize().x * 2.f;

        float x = pos_x / animationDrawer->GetVirtualScreenSize().x;
        float y = pos_y / animationDrawer->GetVirtualScreenSize().y;

        for (int i = 0; i < symbols_temp.size(); i++) {

            
            symbols_temp[i]->Position.x.default_value = x +        symbols_temp[i]->glyph.plane.GetX()  * temp_size_x + symbols_temp[i]->glyph.plane.GetWidth()/2.f * temp_size_x;

            float res_y = y - (1.f - symbols_temp[i]->glyph.plane.bottom) * temp_size_y + symbols_temp[i]->glyph.plane.GetHeight() * temp_size_y/2.f;

            symbols_temp[i]->Position.y.default_value = res_y;


            symbols_temp[i]->Size.x.default_value = temp_size_x/2.f;
            symbols_temp[i]->Size.y.default_value = temp_size_y/2.f;

            symbols_temp[i]->Thikness.x.default_value = thikness;

            // Применяем кернинг, если есть следующий символ
            if (i + 1 <= symbols_temp.size() - 1) {
                float kerning = fontMSDF->GetKerning(symbols_temp[i]->glyph.unicode, symbols_temp[i + 1]->glyph.unicode);
                x += kerning * temp_size_x;
            }


            // Перемещаем курсор на следующий символ
            x += (symbols_temp[i]->glyph.advance) * temp_size_x +  (spacing/ animationDrawer->GetVirtualScreenSize().x) * temp_size_x;
        }


        if (ImGui::Button("CREATE")) {
            symbols_temp.clear();
        }
    }
}
#include "AnimationSequencer.h"



void AnimationSequencer::AddKey(CurveBazierArray& curve, float& value) {

    glm::vec2 point = { (float)currentFrame / (float)FPS,value };

    std::cout << "Add: [" << point.x << "] [" << point.y << "]\n";

    curve.AddPoint(point);
}


float AnimationSequencer::GetTime() {
    return (float)currentFrame / (float)FPS;
}


void AnimationSequencer::ReadComponentAnimation(CurveBazierArray& curve_data, const nlohmann::json& data) {

    curve_data.default_value = data["default_value"].get<float>();
    curve_data.OnlyOnePoint = data["OnlyOnePoint"].get<bool>();

    if (data.contains("Curves")) {
        for (auto& [key, value] : data["Curves"].items()) {
            CubicCurveBezier curve;

            std::vector<float> LeftBase = value["B_left"].get<std::vector<float>>();
            std::vector<float> LeftAdd = value["A_left"].get<std::vector<float>>();
            std::vector<float> RightAdd = value["A_right"].get<std::vector<float>>();
            std::vector<float> RightBase = value["B_right"].get<std::vector<float>>();


            curve.SetLeftBase(glm::vec2{ LeftBase[0],LeftBase[1] });
            curve.SetLeftAdditional(glm::vec2{ LeftAdd[0],LeftAdd[1] });

            curve.SetRightBase(glm::vec2{ RightBase[0],RightBase[1] });
            curve.SetRightAdditional(glm::vec2{ RightAdd[0],RightAdd[1] });

            curve_data.SetCurve(curve, stoi(key));
        }

        curve_data.DiscritizeAll();
        curve_data.MakeSamples();
    }


}

void AnimationSequencer::ReadSave(const nlohmann::json& data) {




    FPS = data["Timeline"]["FPS"].get<int32_t>();
    currentFrame = data["Timeline"]["currentFrame"].get<int32_t>();
    TimeAnimation = data["Timeline"]["TimeAnimation"].get<float>();

    CubicCurveBezier curve_temp;



    if (data.contains("symbols") ) {

        for (auto& [key, value] : data["symbols"].items()) {


            int unicode_glyph = value["unicode"].get<int32_t>();
            int index_symbol = value["ID"].get<int32_t>();

            Glyph glyph = fontMSDF->GetGlyphByUnicode(unicode_glyph);

            SymbolMSDF_Data* symbol_data = new SymbolMSDF_Data(index_symbol, glyph);

            ReadComponentAnimation(symbol_data->Position.x, value["Position"]["x"]);
            ReadComponentAnimation(symbol_data->Position.y, value["Position"]["y"]);

            ReadComponentAnimation(symbol_data->Size.x, value["Size"]["x"]);
            ReadComponentAnimation(symbol_data->Size.y, value["Size"]["y"]);

            ReadComponentAnimation(symbol_data->Thikness.x, value["Thikness"]["x"]);
            ReadComponentAnimation(symbol_data->Rotate.x, value["Rotate"]["x"]);

            ReadComponentAnimation(symbol_data->ColorText.x, value["ColorText"]["x"]);
            ReadComponentAnimation(symbol_data->ColorText.y, value["ColorText"]["y"]);
            ReadComponentAnimation(symbol_data->ColorText.z, value["ColorText"]["z"]);

            ReadComponentAnimation(symbol_data->ColorOutline.x, value["ColorOutline"]["x"]);
            ReadComponentAnimation(symbol_data->ColorOutline.y, value["ColorOutline"]["y"]);
            ReadComponentAnimation(symbol_data->ColorOutline.z, value["ColorOutline"]["z"]);

            SymbolsManager->AddSymbol(symbol_data);
        }
        


    }

}


void AnimationSequencer::SaveCurve(CurveBazierArray& curve, nlohmann::json& CurveJSON) {

    CurveJSON["default_value"] = curve.default_value;
    CurveJSON["OnlyOnePoint"] = curve.OnlyOnePoint;


    auto curve_data = curve.GetCurves();
    for (int i = 0; i < curve_data.size(); i++) {
        CurveJSON["Curves"][std::to_string(i)]["B_left"] = { curve_data[i].GetLeftBase().x, curve_data[i].GetLeftBase().y };
        CurveJSON["Curves"][std::to_string(i)]["A_left"] = { curve_data[i].GetLeftAdditional().x, curve_data[i].GetLeftAdditional().y };
        CurveJSON["Curves"][std::to_string(i)]["A_right"] = { curve_data[i].GetRightAdditional().x, curve_data[i].GetRightAdditional().y };
        CurveJSON["Curves"][std::to_string(i)]["B_right"] = { curve_data[i].GetRightBase().x, curve_data[i].GetRightBase().y };
    }


}

void AnimationSequencer::Update() {

    static float prev_time = 0.f;
    static float time = 0.f;

    // Reset time
    if (ImGui::IsKeyPressed(ImGuiKey_E)) {
        time = 0.f;
        prev_time = 0.f;
        currentFrame = time * (float)FPS;
        animationDrawer->SetTime(time);
        animationDrawer->SetPrevTime(prev_time);
    }

    // AddTime by deltaTime
    if (ImGui::IsKeyDown(ImGuiKey_Space)) {

        IsTimePlaying = true;


        time += engine->GetDeltaTime();
        currentFrame = time * (float)FPS;
        if (time >= (float)endFrame / (float)FPS)
        {
            time = 0.f;
            prev_time = 0.f;
        }

        animationDrawer->SetTime(time);
        animationDrawer->SetPrevTime(prev_time);

        prev_time = time;


    }
    else {
        IsTimePlaying = false;

        animationDrawer->SetTime(GetTime());
        animationDrawer->SetPrevTime((float)(currentFrame - 1) / (float)FPS);
    }

    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
    {
        if (ImGui::IsKeyPressed(ImGuiKey_O)) {
            std::string path2Open = Open_FileDialog(u8"Выбери файл сохранения", u8"Json\0*.json\0");
            if (path2Open.empty() == false && isFileExist(path2Open)) {
                std::ifstream ifn(path2Open);

                std::string line;

                std::getline(ifn, line);

                nlohmann::json temp;

                temp = nlohmann::json::parse(line);

                ReadSave(temp);

                ifn.close();
            }
        }


        if (ImGui::IsKeyPressed(ImGuiKey_S))
        {
            std::string path2save = Save_FileDialog(u8"Выбери путь для сохранения", u8"Json\0*.json\0");
            path2save += ".json";



            SaveData.clear();

            SaveData["Timeline"]["FPS"] = FPS;
            SaveData["Timeline"]["TimeAnimation"] = TimeAnimation;
            SaveData["Timeline"]["currentFrame"] = currentFrame;


            for (int i = 0; i < SymbolsManager->GetSymbols()->size(); i++)
            {
                //auto symbolData = SaveData["symbols"][std::to_string(i)];
                SaveData["symbols"][std::to_string(i)]["ID"]      = SymbolsManager->GetSymbols()->at(i)->indexObject;
                SaveData["symbols"][std::to_string(i)]["unicode"] = SymbolsManager->GetSymbols()->at(i)->glyph.unicode;

                SaveCurve(SymbolsManager->GetSymbols()->at(i)->Position.x, SaveData["symbols"][std::to_string(i)]["Position"]["x"]);
                SaveCurve(SymbolsManager->GetSymbols()->at(i)->Position.y, SaveData["symbols"][std::to_string(i)]["Position"]["y"]);
                SaveCurve(SymbolsManager->GetSymbols()->at(i)->Size.x, SaveData["symbols"][std::to_string(i)]["Size"]["x"]);
                SaveCurve(SymbolsManager->GetSymbols()->at(i)->Size.y, SaveData["symbols"][std::to_string(i)]["Size"]["y"]);
                SaveCurve(SymbolsManager->GetSymbols()->at(i)->Rotate.x, SaveData["symbols"][std::to_string(i)]["Rotate"]["x"]);
                SaveCurve(SymbolsManager->GetSymbols()->at(i)->Thikness.x, SaveData["symbols"][std::to_string(i)]["Thikness"]["x"]);

                SaveCurve(SymbolsManager->GetSymbols()->at(i)->ColorOutline.x, SaveData["symbols"][std::to_string(i)]["ColorOutline"]["x"]);
                SaveCurve(SymbolsManager->GetSymbols()->at(i)->ColorOutline.y, SaveData["symbols"][std::to_string(i)]["ColorOutline"]["y"]);
                SaveCurve(SymbolsManager->GetSymbols()->at(i)->ColorOutline.z, SaveData["symbols"][std::to_string(i)]["ColorOutline"]["z"]);

                SaveCurve(SymbolsManager->GetSymbols()->at(i)->ColorText.x, SaveData["symbols"][std::to_string(i)]["ColorText"]["x"]);
                SaveCurve(SymbolsManager->GetSymbols()->at(i)->ColorText.y, SaveData["symbols"][std::to_string(i)]["ColorText"]["y"]);
                SaveCurve(SymbolsManager->GetSymbols()->at(i)->ColorText.z, SaveData["symbols"][std::to_string(i)]["ColorText"]["z"]);

            }

            std::ofstream ofn;
            ofn.open(path2save);
            ofn << SaveData.dump();
            ofn.close();

        }
    }
}


inline bool AnimationSequencer::DrawParametr(const std::string& Name, const int& index,const bool& ParametrChoosed , CurveBazierArray& curveArray, const float& min_value, const float& max_value, const float& Multiplier) {

    bool Result = false;

    if (ImGui::RadioButton(std::string("## CheckBox " + Name + " " + std::to_string(index)).c_str(), curveArray.OnlyOnePoint)) {
        curveArray.OnlyOnePoint = !curveArray.OnlyOnePoint;
    }

    ImGui::SameLine();

    if (ImGui::Selectable(std::string(Name + "  ## Selectable " + std::to_string(index)).c_str(), ParametrChoosed, ImGuiSelectableFlags_AllowOverlap)){
        Result = true;
    }

    ImGui::SameLine();
    
    float Current_Value = curveArray.GetValueFromTime(GetTime()) * Multiplier;
    if (
        ImGui::SliderFloat(
            ("## Slider " + Name + std::to_string(index)).c_str(),
            &Current_Value,
            min_value,
            max_value)
        )
    {
        if (curveArray.OnlyOnePoint)
            curveArray.default_value = Current_Value / Multiplier;
        else if (IsTimePlaying == false)
            curveArray.AddPoint({ GetTime(),Current_Value / Multiplier });
    }

    return Result;
}




void AnimationSequencer::DrawListSymbols() {
    if (ImGui::BeginChild("SymbolsParametrs", ImVec2(1 * ImGui::GetWindowWidth() / 5, 0))) {

        for (int i = 0; i < symbols->size(); i++)
        {
            int unicode = symbols->at(i)->glyph.unicode;
            std::string symbol = UnicodeValue_to_UTF8str(unicode);


            ImGui::PushID(i); // Уникальный идентификатор для каждого элемента

            const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_AllowItemOverlap;

            bool symbolIsOpened = ImGui::TreeNodeEx(std::string(symbol + " ##" + std::to_string(i)).c_str(), flags);


            // Получаем размеры узла
            const ImVec2 node_min = ImGui::GetItemRectMin();
            const ImVec2 node_max = ImGui::GetItemRectMax();
            const float node_height = node_max.y - node_min.y;

            // Рассчитываем позицию кнопки
            const float button_size = node_height;
            ImVec2 button_pos(
                node_max.x - button_size - 5,
                node_min.y
            );

            // Настраиваем и рисуем кнопку
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::SetCursorScreenPos(button_pos);
            if (ImGui::Button("X", ImVec2(button_size + 5, button_size))) {

                symbols->erase(symbols->begin() + i);
                i--;
                ImGui::PopStyleVar();
                ImGui::PopID();
                continue;

            }
            ImGui::PopStyleVar();



            if (symbolIsOpened) {

                if (ImGui::TreeNodeEx(std::string("Position ##" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
                {
                    if (
                        DrawParametr(
                            "X",
                            i,
                            choosedSetting.CheckPosition(0, i),
                            symbols->at(i)->Position.x,
                            -animationDrawer->GetVirtualScreenSize().x * 2.f,
                            animationDrawer->GetVirtualScreenSize().x * 2.f,
                            animationDrawer->GetVirtualScreenSize().x)
                        ) {
                        choosedSetting.SetPosition(0, i);
                    }

                    if (
                        DrawParametr(
                            "Y",
                            i,
                            choosedSetting.CheckPosition(1, i),
                            symbols->at(i)->Position.y,
                            -animationDrawer->GetVirtualScreenSize().y * 2.f,
                            animationDrawer->GetVirtualScreenSize().y * 2.f,
                            animationDrawer->GetVirtualScreenSize().y)) {
                        choosedSetting.SetPosition(1, i);
                    }


                    ImGui::TreePop();
                }
                if (ImGui::TreeNodeEx(std::string("Scale ##" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
                {
                    if (
                        DrawParametr(
                            "WIDTH ",
                            i,
                            choosedSetting.CheckScale(0, i),
                            symbols->at(i)->Size.x,
                            -animationDrawer->GetVirtualScreenSize().x * 2.f,
                            animationDrawer->GetVirtualScreenSize().x * 2.f,
                            animationDrawer->GetVirtualScreenSize().x
                        )) {
                        choosedSetting.SetScale(0, i);
                    }

                    if (
                        DrawParametr(
                            "HEIGHT",
                            i,
                            choosedSetting.CheckScale(1, i),
                            symbols->at(i)->Size.y,
                            -animationDrawer->GetVirtualScreenSize().y * 2.f,
                            animationDrawer->GetVirtualScreenSize().y * 2.f,
                            animationDrawer->GetVirtualScreenSize().y
                        )) {
                        choosedSetting.SetScale(1, i);
                    }


                    ImGui::TreePop();
                }
                if (ImGui::TreeNodeEx(std::string("Rotate ## " + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
                {
                    if (
                        DrawParametr(
                            "ANGLE",
                            i,
                            choosedSetting.CheckRotate(0, i),
                            symbols->at(i)->Rotate.x,
                            -720.f,
                            720.f,
                            360.f/3.1415f
                        )) {
                        choosedSetting.SetRotate(0, i);
                    }


                    ImGui::TreePop();
                }

                if (ImGui::TreeNodeEx(std::string("ColorText ##" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
                {
                    if (ImGui::Selectable(std::string("R ## ColorText" + std::to_string(i)).c_str(), choosedSetting.CheckColorText(0, i), ImGuiSelectableFlags_AllowOverlap))
                        choosedSetting.SetColorText(0, i);
                    if (ImGui::Selectable(std::string("G ## ColorText" + std::to_string(i)).c_str(), choosedSetting.CheckColorText(1, i), ImGuiSelectableFlags_AllowOverlap))
                        choosedSetting.SetColorText(1, i);
                    if (ImGui::Selectable(std::string("B ## ColorText" + std::to_string(i)).c_str(), choosedSetting.CheckColorText(2, i), ImGuiSelectableFlags_AllowOverlap))
                        choosedSetting.SetColorText(2, i);



                    ImGui::TreePop();
                }
                if (ImGui::TreeNodeEx(std::string("ColorOutline ##" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
                {
                    if (ImGui::Selectable(std::string("R ## ColorOutline" + std::to_string(i)).c_str(), choosedSetting.CheckColorOutline(0, i), ImGuiSelectableFlags_AllowOverlap))
                        choosedSetting.SetColorOutline(0, i);
                    if (ImGui::Selectable(std::string("G ## ColorOutline" + std::to_string(i)).c_str(), choosedSetting.CheckColorOutline(1, i), ImGuiSelectableFlags_AllowOverlap))
                        choosedSetting.SetColorOutline(1, i);
                    if (ImGui::Selectable(std::string("B ## ColorOutline" + std::to_string(i)).c_str(), choosedSetting.CheckColorOutline(2, i), ImGuiSelectableFlags_AllowOverlap))
                        choosedSetting.SetColorOutline(2, i);

                    ImGui::TreePop();
                }
                if (
                    DrawParametr(
                        "Thikness",
                        i,
                        choosedSetting.CheckThikness(0, i),
                        symbols->at(i)->Thikness.x,
                        -10.f,
                        10.f,
                        1.f
                    )) {
                    choosedSetting.SetThikness(0, i);
                }

                ImGui::TreePop();
            }

            ImGui::PopID();
        }

        ImGui::EndChild();
    }
}

void AnimationSequencer::DrawGraphChoosedParametr() {

    if (ImGui::BeginChild("GraphChoosedParametr", ImVec2(0, 0))) {

        if (symbols->empty() == false && choosedSetting.index_choosed_curve != -1 && choosedSetting.index_choosed_curve <= symbols->size() - 1)
        {

            CurveBazierArray* curve = nullptr;

            if (choosedSetting.Position) {
                switch (choosedSetting.NumberParametr) {
                case 0:
                    curve = &symbols->at(choosedSetting.index_choosed_curve)->Position.x;
                    break;
                case 1:
                    curve = &symbols->at(choosedSetting.index_choosed_curve)->Position.y;
                    break;
                default:
                    break;
                }
            }
            else if (choosedSetting.Scale) {
                switch (choosedSetting.NumberParametr) {
                case 0:
                    curve = &symbols->at(choosedSetting.index_choosed_curve)->Size.x;
                    break;
                case 1:
                    curve = &symbols->at(choosedSetting.index_choosed_curve)->Size.y;
                    break;
                default:
                    break;
                }
            }
            else if (choosedSetting.Rotate) {
                curve = &symbols->at(choosedSetting.index_choosed_curve)->Rotate.x;
            }
            else if (choosedSetting.Thikness) {
                curve = &symbols->at(choosedSetting.index_choosed_curve)->Thikness.x;
            }
            else if (choosedSetting.ColorText) {
                switch (choosedSetting.NumberParametr) {
                case 0:
                    curve = &symbols->at(choosedSetting.index_choosed_curve)->ColorText.x;
                    break;
                case 1:
                    curve = &symbols->at(choosedSetting.index_choosed_curve)->ColorText.y;
                    break;
                case 2:
                    curve = &symbols->at(choosedSetting.index_choosed_curve)->ColorText.z;
                    break;
                default:
                    break;
                }
            }
            else if (choosedSetting.ColorOutline) {
                switch (choosedSetting.NumberParametr) {
                case 0:
                    curve = &symbols->at(choosedSetting.index_choosed_curve)->ColorOutline.x;
                    break;
                case 1:
                    curve = &symbols->at(choosedSetting.index_choosed_curve)->ColorOutline.y;
                    break;
                case 2:
                    curve = &symbols->at(choosedSetting.index_choosed_curve)->ColorOutline.z;
                    break;
                default:
                    break;
                }
            }



            curvesPointsManager.UpdatePoints(*curve);
            curvesPointsManager.Draw((float)currentFrame / (float)FPS);
        }

        ImGui::EndChild();
    }

}

void AnimationSequencer::Draw() {

    static float angle = 0.f;
    static float position_x = 0.f, position_y = 0.f;
    static float width = 0.f, height = 0.f;
    static float thikness = 0.f;

    static ImVec4 ColorText{ 0.f,0.f,0.f,0.f };
    static ImVec4 ColorOutline{ 0.f,0.f,0.f,0.f };






    if (ImGui::Begin("Timeline")) {


        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::SliderInt("Frames", &currentFrame, 0, endFrame,"%d", ImGuiSliderFlags_NoLabel | ImGuiSliderFlags_NoInput);

        DrawListSymbols();

        ImGui::SameLine();

        DrawGraphChoosedParametr();


        ImGui::End();
    }
    


    if (ImGui::Begin("Menu")) {
        if (ImGui::SliderInt("FPS", &FPS, 30, 240)) {
            endFrame = (FPS * TimeAnimation);
        }
        if (ImGui::SliderFloat("TimeEnd", &TimeAnimation, 0.2f, 60.f)) {
            endFrame = (FPS * TimeAnimation);
        }


        ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 4.f);

        textCreator->Draw();

        {
            static char TextInput[256] = { 0, };

            {
                static bool inited = false;
                if (inited == false) {
                    for (int i = 0; i < 256; i++)
                        TextInput[i] = 0;

                    inited = true;
                }
            }

            static float size_symbols = 120.f;

            ImGui::SliderFloat(u8"Размер", &size_symbols, 0.f, animationDrawer->GetVirtualScreenSize().y);

            if (ImGui::Button("Create")) {

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
                        SymbolMSDF_Data* temp = new SymbolMSDF_Data(indexObject, fontMSDF->GetGlyphByUnicode(unicode_values[i]));

                        temp->Size.x.default_value = size_symbols / animationDrawer->GetVirtualScreenSize().y;
                        temp->Size.y.default_value = size_symbols / animationDrawer->GetVirtualScreenSize().y;

                        symbols->push_back(temp);

                    }

                    for (int i = 0; i < 256; i++)
                        TextInput[i] = 0;
                }

                indexObject++;
            }
            ImGui::SameLine();
            ImGui::InputText("##SymbolName", &TextInput[0], 256);






        }









    }








    
}

AnimationSequencer::AnimationSequencer(AnimationDrawer* animationDrawer, Engine* engine) {
    this->engine = engine;
    this->animationDrawer = animationDrawer;
    SymbolsManager = animationDrawer->Get_ptr_Quad_SymbolMSDF_instance();
    symbols = SymbolsManager->GetSymbols();
    fontMSDF = animationDrawer->GetMSDF_Font();

    textCreator = new TextCreator(animationDrawer);

    InfoOpened.resize(100);
}

AnimationSequencer::~AnimationSequencer() {

}
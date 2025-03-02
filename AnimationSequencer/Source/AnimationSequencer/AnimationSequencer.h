#ifndef ANIMATION_SEQUENCER_H
#define ANIMATION_SEQUENCER_H

#include <fstream>
#include <nlohmann/json.hpp>

#include "Utils/File/Utils.File.h"
#include "Utils/Text/Encoding/Unicode/Utils.Encoding.Unicode.h"
#include "Utils/Text/Encoding/UTF8/Utils.Encoding.UTF8.h"

#include "Engine/Engine.h"
#include "RenderObject/OpenGL.Quad.SymbolMSDF.Instance.h"
#include "AnimationDrawer/AnimationDrawer.h"
#include "CurvesPointsManager/CurvesPointsManager.h"
#include "TextCreator/TextCreator.h"

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/implot.h"
#include "ThirdParty/ImGuiSequencerNeo/imgui_neo_sequencer.h"

class AnimationSequencer
{
public:
	AnimationSequencer(AnimationDrawer* animationDrawer, Engine* engine);
	~AnimationSequencer();

	void Draw();
    void Update();

private:
    Engine* engine;
    CurvesPointsManager curvesPointsManager;
    TextCreator* textCreator = nullptr;

    float TimeAnimation = 1.f;
    int32_t FPS = 60.f;
    float CurrentTime = 0.f;
    float PrevTime = 0.f;
    std::vector<SymbolMSDF_Data*>* symbols;

	Quad_SymbolMSDF_instance* SymbolsManager;
    AnimationDrawer* animationDrawer;
    Font* fontMSDF;

    struct CurveOpenedInfo{
        bool Curve = false;
    };

    int indexObject = 0;

    std::vector<CurveOpenedInfo> InfoOpened;

    std::string currentSetting = "";
    int IndexChoosedCurve = -1;

    bool IsTimePlaying = false;
    int32_t currentFrame = 0;
    int32_t startFrame = 0;
    int32_t endFrame = (FPS * TimeAnimation);

    nlohmann::json SaveData;

    void AddKey(CurveBazierArray& curve, float& value);
    

    void SaveCurve(CurveBazierArray& curve, nlohmann::json& CurveJSON);

    void ReadComponentAnimation(CurveBazierArray& curve_data, const nlohmann::json& data);
    void ReadSave(const nlohmann::json& data);

    float GetTime();

    inline bool DrawParametr(const std::string& Name, const int& index, const bool& ParametrChoosed, CurveBazierArray& curveArray, const float& min_value, const float& max_value, const float& Multiplier);


    void DrawListSymbols();
    void DrawGraphChoosedParametr();


    struct ChoosedSetting {
        bool Position = false;
        bool Scale = false;
        bool Rotate = false;
        bool ColorText = false;
        bool ColorOutline = false;
        bool Thikness = false;

        int index_choosed_curve = -1;
        int NumberParametr = 0;

        bool CheckPosition(const int& param, const int& index_curve) {
            return Position == true && NumberParametr == param && index_choosed_curve == index_curve;
        }
        bool CheckScale(const int& param, const int& index_curve) {
            return Scale == true && NumberParametr == param && index_choosed_curve == index_curve;
        }
        bool CheckRotate(const int& param, const int& index_curve) {
            return Rotate == true && NumberParametr == param && index_choosed_curve == index_curve;
        }
        bool CheckThikness(const int& param, const int& index_curve) {
            return Thikness == true && NumberParametr == param && index_choosed_curve == index_curve;
        }
        bool CheckColorText(const int& param, const int& index_curve) {
            return ColorText == true && NumberParametr == param && index_choosed_curve == index_curve;
        }
        bool CheckColorOutline(const int& param, const int& index_curve) {
            return ColorOutline == true && NumberParametr == param && index_choosed_curve == index_curve;
        }
        void SetPosition(const int& param, const int& index_curve) {
            Position = true;
            Scale = false;
            Rotate = false;
            ColorText = false;
            ColorOutline = false;
            Thikness = false;
            NumberParametr = param;
            index_choosed_curve = index_curve;
        }
        void SetScale(const int& param, const int& index_curve) {
            Position = false;
            Scale = true;
            Rotate = false;
            ColorText = false;
            ColorOutline = false;
            Thikness = false;
            NumberParametr = param;
            index_choosed_curve = index_curve;
        }
        void SetRotate(const int& param, const int& index_curve) {
            Position = false;
            Scale = false;
            Rotate = true;
            ColorText = false;
            ColorOutline = false;
            Thikness = false;
            NumberParametr = param;
            index_choosed_curve = index_curve;
        }
        void SetColorText(const int& param, const int& index_curve) {
            Position = false;
            Scale = false;
            Rotate = false;
            ColorText = true;
            ColorOutline = false;
            Thikness = false;
            NumberParametr = param;
            index_choosed_curve = index_curve;
        }
        void SetColorOutline(const int& param, const int& index_curve) {
            Position = false;
            Scale = false;
            Rotate = false;
            ColorText = false;
            ColorOutline = true;
            Thikness = false;
            NumberParametr = param;
            index_choosed_curve = index_curve;
        }
        void SetThikness(const int& param, const int& index_curve) {
            Position = false;
            Scale = false;
            Rotate = false;
            ColorText = false;
            ColorOutline = false;
            Thikness = true;
            NumberParametr = param;
            index_choosed_curve = index_curve;
        }

    } choosedSetting;

};




#endif
#include <iostream>

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_impl_glfw.h"
#include "ThirdParty/ImGui/imgui_impl_opengl3.h"

#include "ThirdParty/ImGuiSequencerNeo/imgui_neo_sequencer.h"

#include "ThirdParty/ImGui/implot.h"
#include "ThirdParty/ImGui/implot_internal.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Engine.h"
#include "Shaders/Shader.h"
#include "Font/Font.h"
#include "RenderObject/OpenGL.Quad.SymbolMSDF.Instance.h"
#include "CurveBezier/ImGui/CurveArrayEditor.h"
#include "UBO/UBO.h"
#include "UBO/DataUBO.h"
#include "Framebuffer/Framebuffer.h"

#include "AnimationDrawer/AnimationDrawer.h"
#include "AnimationSequencer/AnimationSequencer.h"
#include "AnimationSequencer/Viewport/Viewport.h"


void LoadFont() {
    ImGuiIO& io = ImGui::GetIO();


    ImFontConfig font_cfg;
    font_cfg.FontDataOwnedByAtlas = false;
    font_cfg.MergeMode = false;


    io.Fonts->Clear();  // Очистить текущие шрифты
    io.Fonts->Fonts.clear();



    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2000, 0x206f,
        0x20ac,0x20ac,
        0x2112, 0x2122,
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0,
    };

    io.Fonts->AddFontFromFileTTF("Data/Fonts/JetBrainsMonoNL-Regular.ttf", 22, &font_cfg, ranges);


    io.Fonts->Build();  // Перестроить атлас шрифтов


    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    if (ImGui_ImplOpenGL3_CreateDeviceObjects() == false)
    {
        std::cout << "Error CreateDeviceObjects" << std::endl;
    }
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    if (ImGui_ImplOpenGL3_CreateFontsTexture() == false) {
        std::cout << "Error CreateFontsTexture" << std::endl;
    }
}


int main() {

    float width_screen = 1280.f;
    float height_screen = 720.f;

    float TargetFPS = 60.f;


    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    float time = 0.f;
    float smothness = 0.2f;

    Engine engine;
    //engine.DisableVSync();
    engine.EnableVSync();
    engine.SetTargetFPS(TargetFPS);
    engine.GetWindowManager()->GetMainWindow()->SetSize(glm::vec2{ width_screen, height_screen });
    engine.GetWindowManager()->GetMainWindow()->Centerize();
    engine.GetWindowManager()->GetMainWindow()->SetTitle("Animation Sequencer");


    AnimationDrawer animationDrawer(&engine);

    animationDrawer.SetSmothness(smothness);
    animationDrawer.SetTargetFPS(TargetFPS);
    animationDrawer.SetClearColor(glm::vec3(clear_color.x, clear_color.y, clear_color.z));

    AnimationSequencer animationSequencer(&animationDrawer,&engine);

    Viewport viewport(&animationDrawer);


    //width_screen = 1280.f/100.f;
    //height_screen = 720.f/100.f;
    

    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();


    ImGui_ImplGlfw_InitForOpenGL(engine.GetWindowManager()->GetMainWindow()->GetHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    LoadFont();

    bool show_demo_window = true;
    bool show_another_window = false;



    while (!engine.GetWindowManager()->GetMainWindow()->IsShouldClose()){

        //std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        //std::cout << "engine.BeginDraw()\n";
        engine.BeginDraw();

        float w_s = engine.GetWindowManager()->GetMainWindow()->GetSize().x;
        float h_s = engine.GetWindowManager()->GetMainWindow()->GetSize().y;


        animationDrawer.Update();
        animationDrawer.Render();




        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(BufferBit::COLOR_BUFFER_BIT | BufferBit::DEPTH_BUFFER_BIT);




        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        bool DrawWindow = true;

        ImPlot::ShowDemoWindow(&DrawWindow);
        ImGui::ShowDemoWindow(&show_demo_window);



        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            if (ImGui::SliderFloat("smoothness", &smothness, -2.0f, 2.0f)) {            // Edit 1 float using a slider from 0.0f to 1.0f
                animationDrawer.SetSmothness(smothness);
            }

            if (ImGui::SliderFloat("target fps", &TargetFPS, 10.0f, 500.0f)) {            // Edit 1 float using a slider from 0.0f to 1.0f
                engine.SetTargetFPS(TargetFPS);
                animationDrawer.SetTargetFPS(TargetFPS);
            }

            if (ImGui::ColorEdit3("clear color", (float*)&clear_color)) {
                animationDrawer.SetClearColor(glm::vec3(clear_color.x, clear_color.y, clear_color.z));
            }


            ImGui::End();




        }

        animationSequencer.Update();
        animationSequencer.Draw();

        viewport.Draw();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        engine.EndDraw();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    return 0;
}
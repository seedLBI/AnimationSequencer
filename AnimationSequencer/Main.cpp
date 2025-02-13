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

    float width_screen = 1920.f;
    float height_screen = 1200.f;

    float TargetFPS = 60.f;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    float time = 0.f;


    Engine engine;
    engine.DisableVSync();
    //engine.EnableVSync();
    engine.SetTargetFPS(TargetFPS);
    engine.GetWindowManager()->GetMainWindow()->SetSize(glm::vec2{ width_screen, height_screen });
    engine.GetWindowManager()->GetMainWindow()->Centerize();
    engine.GetWindowManager()->GetMainWindow()->SetTitle("Animation Sequencer");


    AnimationDrawer animationDrawer(&engine);
    animationDrawer.SetTargetFPS(TargetFPS);
    animationDrawer.SetClearColor(glm::vec3(clear_color.x, clear_color.y, clear_color.z));
    SymbolMSDF_Data* firstSymbol = new SymbolMSDF_Data(0, animationDrawer.GetMSDF_Font()->GetGlyphByUnicode(0x0426));
    SymbolMSDF_Data* SecondSymbol = new SymbolMSDF_Data(1, animationDrawer.GetMSDF_Font()->GetGlyphByUnicode(0x0041));


    animationDrawer.Get_ptr_Quad_SymbolMSDF_instance()->AddSymbol(firstSymbol);
    animationDrawer.Get_ptr_Quad_SymbolMSDF_instance()->AddSymbol(SecondSymbol);

    firstSymbol->Size.x.default_value = 250.f / 1280.f;
    firstSymbol->Size.y.default_value = 250.f / 1280.f;



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


        animationDrawer.SetTime(time);
        animationDrawer.Update();

        animationDrawer.Render();

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(BufferBit::COLOR_BUFFER_BIT | BufferBit::DEPTH_BUFFER_BIT);




        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        bool DrawWindow = true;

        //ImPlot::ShowDemoWindow(&DrawWindow);
        ImGui::ShowDemoWindow(&show_demo_window);


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
        if(ImGui::Begin("Viewport")) {


            // Основной контент с индивидуальным отступом
            ImGui::BeginChild("Viewport_Content", ImVec2(0, -ImGui::GetFrameHeight() - 1.f), false, ImGuiWindowFlags_None);
            


            ImVec2 avail_size = ImGui::GetContentRegionAvail();
            ImVec2 pos = ImGui::GetCursorScreenPos();

            float min_sizeImage = min(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

            ImVec2 sizeImage = ImVec2(width_screen, height_screen);
            float minnn = min(width_screen, height_screen);

            float scale_x = ImGui::GetContentRegionAvail().x / width_screen;
            float scale_y = ImGui::GetContentRegionAvail().y / height_screen;

            float scale = min(scale_x, scale_y);

            sizeImage.x *= scale;
            sizeImage.y *= scale;

            ImVec2 currentPos = ImVec2(pos.x + ImGui::GetContentRegionAvail().x / 2.f - sizeImage.x / 2.f, pos.y + ImGui::GetContentRegionAvail().y / 2.f - sizeImage.y / 2.f);

            

            ImGui::SetCursorScreenPos(currentPos);
            //ImGui::Image((void*)pbo->tex_id, sizeImage, ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 0));



            ImGui::Image((void*)animationDrawer.GetFinalTexture()->GetID(), ImVec2(sizeImage.x, sizeImage.y), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 0));

            ImGui::EndChild();


            // Нижняя панель меню без отступов

            ImGui::BeginChild("Viewport_BottomMenu", ImVec2(0, 0), false,
                ImGuiWindowFlags_MenuBar |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoDecoration);
            {
                if (ImGui::BeginMenuBar())
                {

                    if (ImGui::BeginMenu(u8"Файл")) { /* ... */ ImGui::EndMenu(); }
                    if (ImGui::BeginMenu(u8"Правка")) { /* ... */ ImGui::EndMenu(); }

                    ImGui::EndMenuBar();
                }
            }
            ImGui::EndChild();




            ImGui::End();
        }
        ImGui::PopStyleVar(2);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            if(ImGui::TreeNode("First")) {

                ImGui::SliderFloat("angle", &firstSymbol->Rotate.x.default_value, 0.0f, 2.f * 3.14f);            // Edit 1 float using a slider from 0.0f to 1.0f

                //firstSymbol->Rotate.x.default_value = (sinf(glfwGetTime() * 2.f)*0.5 + 0.5) * 2.f * 3.14f;



                ImGui::SliderFloat("AnchorPoint.x", &firstSymbol->Anchor_Point.x.default_value, -2.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("AnchorPoint.y", &firstSymbol->Anchor_Point.y.default_value, -2.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

                ImGui::SliderFloat("PositionPoint.x", &firstSymbol->Position.x.default_value, -2000.0f, 2000.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("PositionPoint.y", &firstSymbol->Position.y.default_value, -2000.0f, 2000.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

                ImGui::SliderFloat("Size.x", &firstSymbol->Size.x.default_value, 0.0f, 250.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("Size.y", &firstSymbol->Size.y.default_value, 0.0f, 250.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

                ImGui::SliderFloat("Thikness", &firstSymbol->Thikness.x.default_value, -12.0f, 12.0f);            // Edit 1 float using a slider from 0.0f to 1.0f


                float col1[3] = { firstSymbol->ColorText.x.default_value,
                     firstSymbol->ColorText.y.default_value,
                     firstSymbol->ColorText.z.default_value };

                if (ImGui::ColorEdit3("ColorText", col1)) {
                    firstSymbol->ColorText.x.default_value = col1[0];
                    firstSymbol->ColorText.y.default_value = col1[1];
                    firstSymbol->ColorText.z.default_value = col1[2];
                }

                float colout1[3] = { firstSymbol->ColorOutline.x.default_value,
                     firstSymbol->ColorOutline.y.default_value,
                     firstSymbol->ColorOutline.z.default_value };

                if (ImGui::ColorEdit3("ColorOutline", colout1)) {
                    firstSymbol->ColorOutline.x.default_value = colout1[0];
                    firstSymbol->ColorOutline.y.default_value = colout1[1];
                    firstSymbol->ColorOutline.z.default_value = colout1[2];
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Second")) {
                //ImGui::SliderFloat("2angle", &SecondSymbol->Rotate.x.default_value, 0.0f, 2.f * 3.14f);            // Edit 1 float using a slider from 0.0f to 1.0f

                SecondSymbol->Rotate.x.default_value = (sinf(glfwGetTime() * 4.f) * 0.5 + 0.5) * 2.f * 3.14f;



                ImGui::SliderFloat("2AnchorPoint.x", &SecondSymbol->Anchor_Point.x.default_value, -2.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("2AnchorPoint.y", &SecondSymbol->Anchor_Point.y.default_value, -2.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

                ImGui::SliderFloat("2PositionPoint.x", &SecondSymbol->Position.x.default_value, -200.0f, 200.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("2PositionPoint.y", &SecondSymbol->Position.y.default_value, -200.0f, 200.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

                ImGui::SliderFloat("2Size.x", &SecondSymbol->Size.x.default_value, 0.0f, 250.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat("2Size.y", &SecondSymbol->Size.y.default_value, 0.0f, 250.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

                ImGui::SliderFloat("2Thikness", &SecondSymbol->Thikness.x.default_value, -12.0f, 12.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

                float col2[3] = {
                     SecondSymbol->ColorText.x.default_value,
                     SecondSymbol->ColorText.y.default_value,
                     SecondSymbol->ColorText.z.default_value };

                if (ImGui::ColorEdit3("2ColorText", col2)) {
                    SecondSymbol->ColorText.x.default_value = col2[0];
                    SecondSymbol->ColorText.y.default_value = col2[1];
                    SecondSymbol->ColorText.z.default_value = col2[2];
                }

                float colout2[3] = {
                     SecondSymbol->ColorOutline.x.default_value,
                     SecondSymbol->ColorOutline.y.default_value,
                     SecondSymbol->ColorOutline.z.default_value };

                if (ImGui::ColorEdit3("2ColorOutline", colout2)) {
                    SecondSymbol->ColorOutline.x.default_value = colout2[0];
                    SecondSymbol->ColorOutline.y.default_value = colout2[1];
                    SecondSymbol->ColorOutline.z.default_value = colout2[2];
                }
                ImGui::TreePop();
            }
            

           
            static float smothness = 0.2f;

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

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);


            static int32_t currentFrame = 0;
            static int32_t startFrame = -10;
            static int32_t endFrame = 64;
            static bool transformOpen = false;

            if (ImGui::BeginNeoSequencer("Sequencer", &currentFrame, &startFrame, &endFrame)) {
                if (ImGui::BeginNeoGroup("Transform", &transformOpen)) {
                    static std::vector<ImGui::FrameIndexType> keys = { 0, 10, 24 };
                    if (ImGui::BeginNeoTimeline("Position", keys,0, ImGuiNeoSequencerFlags_EnableSelection)) {
                        ImGui::EndNeoTimeLine();
                    }
                    ImGui::EndNeoGroup();
                }

                ImGui::EndNeoSequencer();
            }




            ImGui::End();




        }




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
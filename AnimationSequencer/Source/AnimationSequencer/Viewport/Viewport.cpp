#include "Viewport.h"


Viewport::Viewport(AnimationDrawer* animationDrawer) {
    this->animationDrawer = animationDrawer;
}

Viewport::~Viewport()
{
}


void Viewport::Draw() {

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
    if (ImGui::Begin("Viewport")) {


        // Основной контент с индивидуальным отступом
        ImGui::BeginChild("Viewport_Content", ImVec2(0, -ImGui::GetFrameHeight() - 1.f), false, ImGuiWindowFlags_None);



        ImVec2 avail_size = ImGui::GetContentRegionAvail();
        ImVec2 pos = ImGui::GetCursorScreenPos();

        float min_sizeImage = std::min(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

        ImVec2 sizeImage = ImVec2(animationDrawer->GetVirtualScreenSize().x, animationDrawer->GetVirtualScreenSize().y);

        if (DrawMSDFTexture)
            sizeImage = ImVec2(animationDrawer->GetMSDF_Font()->GetTexture()->GetWidth(), animationDrawer->GetMSDF_Font()->GetTexture()->GetHeight());


        float scale_x = ImGui::GetContentRegionAvail().x / sizeImage.x;
        float scale_y = ImGui::GetContentRegionAvail().y / sizeImage.y;

        float scale = std::min(scale_x, scale_y);

        sizeImage.x *= scale;
        sizeImage.y *= scale;

        ImVec2 currentPos = ImVec2(pos.x + ImGui::GetContentRegionAvail().x / 2.f - sizeImage.x / 2.f, pos.y + ImGui::GetContentRegionAvail().y / 2.f - sizeImage.y / 2.f);



        ImGui::SetCursorScreenPos(currentPos);


        if (DrawVelocityTexture)
            ImGui::Image(
                (void*)animationDrawer->GetVelocityTexture()->GetID(),
                ImVec2(sizeImage.x, sizeImage.y),
                ImVec2(0, 0),
                ImVec2(1, 1),
                ImColor(255, 255, 255, 255),
                ImColor(255, 255, 255, 0)
            );
        else if (DrawImageTexture)
            ImGui::Image(
                (void*)animationDrawer->GetImageTexture()->GetID(),
                ImVec2(sizeImage.x, sizeImage.y),
                ImVec2(0, 0),
                ImVec2(1, 1),
                ImColor(255, 255, 255, 255),
                ImColor(255, 255, 255, 0)
            );
        else if (DrawFinalTexture)
            ImGui::Image(
                (void*)animationDrawer->GetFinalTexture()->GetID(),
                ImVec2(sizeImage.x, sizeImage.y),
                ImVec2(0, 0),
                ImVec2(1, 1),
                ImColor(255, 255, 255, 255),
                ImColor(255, 255, 255, 0)
            );
        else if (DrawMSDFTexture)
            ImGui::Image(
                (void*)animationDrawer->GetMSDF_Font()->GetTexture()->GetID(),
                ImVec2(sizeImage.x, sizeImage.y),
                ImVec2(0, 1),
                ImVec2(1, 0),
                ImColor(255, 255, 255, 255),
                ImColor(255, 255, 255, 0)
            );

        ImGui::EndChild();


        // Нижняя панель меню без отступов

        ImGui::BeginChild("Viewport_BottomMenu", ImVec2(0, 0), false,
            ImGuiWindowFlags_MenuBar |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoDecoration);
        {
            if (ImGui::BeginMenuBar())
            {

                if (ImGui::Checkbox(u8"FontTexture", &DrawMSDFTexture)) {
                    DrawMSDFTexture = true;
                    DrawVelocityTexture = false;
                    DrawImageTexture = false;
                    DrawFinalTexture = false;
                }

                if (ImGui::Checkbox(u8"Velocity", &DrawVelocityTexture)) {
                    DrawMSDFTexture = false;
                    DrawVelocityTexture = true;
                    DrawImageTexture = false;
                    DrawFinalTexture = false;
                }

                if (ImGui::Checkbox(u8"Image", &DrawImageTexture)) {
                    DrawMSDFTexture = false;
                    DrawVelocityTexture = false;
                    DrawImageTexture = true;
                    DrawFinalTexture = false;
                }

                if (ImGui::Checkbox(u8"Final", &DrawFinalTexture)) {
                    DrawMSDFTexture = false;
                    DrawVelocityTexture = false;
                    DrawImageTexture = false;
                    DrawFinalTexture = true;
                }

                ImGui::EndMenuBar();
            }
        }
        ImGui::EndChild();




        ImGui::End();
    }
    ImGui::PopStyleVar(2);
}
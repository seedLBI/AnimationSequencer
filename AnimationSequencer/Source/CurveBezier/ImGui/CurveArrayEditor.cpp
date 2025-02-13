#include "CurveArrayEditor.h"


void CurveEditor() {

    static ImVec2 canvas_p0, canvas_p1;  // ������� �������
    static ImVec2 offset = { 0, 0 };       // �������� (��� ����������)
    static bool is_panning = false;      // ���� ����������� ������

    ImGui::Begin("Canvas Window");

    // �������� ��������� ������������ � ����
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
    if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;

    // ���������� ������� �������
    canvas_p0 = ImGui::GetCursorScreenPos();
    canvas_p1 = ImVec2(canvas_p0.x + canvas_size.x, canvas_p0.y + canvas_size.y);

    // ������ ��������� ������ ��� ��������� �������
    ImGui::InvisibleButton("canvas", canvas_size, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    bool is_hovered = ImGui::IsItemHovered();
    bool is_active = ImGui::IsItemActive();

    // ������������ ����������� ������ (panning)
    ImVec2 mouse_delta = ImGui::GetIO().MouseDelta;
    if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        offset.x += mouse_delta.x;
        offset.y += mouse_delta.y;
    }

    // �������� ������ ��� ���������
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // ������ ���
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // ������ ��������� ����� (10x10)
    for (float x = canvas_p0.x + offset.x; x < canvas_p1.x; x += 10.0f)
        draw_list->AddLine(ImVec2(x, canvas_p0.y), ImVec2(x, canvas_p1.y), IM_COL32(100, 100, 100, 255));
    for (float y = canvas_p0.y + offset.y; y < canvas_p1.y; y += 10.0f)
        draw_list->AddLine(ImVec2(canvas_p0.x, y), ImVec2(canvas_p1.x, y), IM_COL32(100, 100, 100, 255));

    // ������ ��������� ��������� (������� ����)
    draw_list->AddCircleFilled(ImVec2(canvas_p0.x + 100, canvas_p0.y + 100), 20.0f, IM_COL32(255, 0, 0, 255));

    ImGui::End();
}
#include "CurvesPointsManager.h"



CurvesPointsManager::CurvesPointsManager()
{
}

CurvesPointsManager::~CurvesPointsManager()
{
}

void CurvesPointsManager::PlotInfRectangle_Before_Point(const ImPlotPoint& pointGraph, const ImU32& rect_color) {
	ImDrawList* draw_list = ImPlot::GetPlotDrawList();

	ImPlotPoint Custom = ImPlot::PlotToPixels(pointGraph);

	const ImVec2 plot_size = ImPlot::GetPlotSize();
	const ImVec2 plot_pos = ImPlot::GetPlotPos();

	draw_list->AddRectFilled(
		plot_pos,
		ImVec2(Custom.x, plot_pos.y + plot_size.y),
		rect_color
	);
}
void CurvesPointsManager::PlotInfRectangle_After_Point(const ImPlotPoint& pointGraph, const ImU32& rect_color) {
	ImDrawList* draw_list = ImPlot::GetPlotDrawList();

	ImPlotPoint Custom = ImPlot::PlotToPixels(pointGraph);

	const ImVec2 plot_size = ImPlot::GetPlotSize();
	const ImVec2 plot_pos = ImPlot::GetPlotPos();

	draw_list->AddRectFilled(
		ImVec2(Custom.x, plot_pos.y),
		ImVec2(plot_pos.x + plot_size.x, plot_pos.y + plot_size.y),
		rect_color
	);
}



void CurvesPointsManager::Draw(float currentTime) {
	const ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
	const ImPlotAxisFlags ax_flags =  ImPlotAxisFlags_NoTickMarks ;

	const ImVec4 ColorCurve(0, 1.f, 0, 1);
	const ImVec4 ColorTimeline(1, 0.5f, 1, 1);

	const ImVec4 ColorBasePoints(1, 0, 0, 1);
	const ImVec4 ColorAdditionalPoints(0, 0, 1, 1);
	const ImVec4 ColorlineBase2Add(0.1, 0.1f, 1, 1);

	if (ImPlot::BeginPlot("##Bezier", ImVec2(-1, -1), ImPlotFlags_CanvasOnly | ImPlotFlags_Crosshairs)) {
		ImPlot::SetupAxes(nullptr, nullptr, ax_flags, ax_flags);


		if (ImGui::IsKeyPressed(ImGuiKey_MouseRight))
		{
			ImPlotPoint MouseOnGraph = ImPlot::PixelsToPlot(ImGui::GetMousePos());

			curveArray->AddPoint(glm::vec2(MouseOnGraph.x, MouseOnGraph.y));


		}
		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Z))
			{
				curveArray->AddOffset_X(currentTime - curveArray->GetCurves_ptr()->front().GetLeftBase().x);
				curveArray->DiscritizeAll();
				curveArray->MakeSamples();
			}
		}


		auto samples = curveArray->GetSamples();
		std::vector<ImPlotPoint> points(samples.size());

		for (int i = 0; i < samples.size(); i++)
			points[i] = ImPlotPoint(samples[i].x, samples[i].y);

		ImPlot::SetNextLineStyle(ColorCurve, 1.f);
		ImPlot::PlotLine("Samples", &points[0].x, &points[0].y, samples.size(), 0, 0, sizeof(ImPlotPoint));


		ImPlot::SetNextLineStyle(ColorTimeline, 2.f);
		ImPlot::PlotInfLines("Timeline", &currentTime, 1, ImPlotItemFlags_NoFit);



		for (int i = 0; i < BasePoints_ptrs.size(); i++) {

			double base_x = (*BasePoints_ptrs[i][0]).x;
			double base_y = (*BasePoints_ptrs[i][0]).y;

			double prev_base_x = -1000.0, prev_base_y = -1000.0;
			double next_base_x = -1000.0, next_base_y = -1000.0;


			bool PrevBaseHave = i > 0;
			if (PrevBaseHave) {
				prev_base_x = (*BasePoints_ptrs[i - 1][0]).x;
				prev_base_y = (*BasePoints_ptrs[i - 1][0]).y;
			}

			bool NextBaseHave = i < BasePoints_ptrs.size() - 1;
			if (NextBaseHave){
				next_base_x = (*BasePoints_ptrs[i + 1][0]).x;
				next_base_y = (*BasePoints_ptrs[i + 1][0]).y;
			}

			int base_id = i;


			if (ImPlot::DragPoint(base_id, &base_x, &base_y, ColorBasePoints, 5, flags)) {
				for (auto* point : BasePoints_ptrs[i]) {
					point->x = base_x;
					point->y = base_y;
				}
				curveArray->DiscritizeAll();
				curveArray->MakeSamples();
			}

			for (int j = 0; j < AdditionalPoints_ptrs[i].size(); j++) {

				double add_x = (*AdditionalPoints_ptrs[i][j]).x;
				double add_y = (*AdditionalPoints_ptrs[i][j]).y;

				int add_id = i * 2 + j + BasePoints_ptrs.size();

				int plotLine_id = add_id + BasePoints_ptrs.size() * 2;
				ImPlotPoint Base2Add_Line[2] = { ImPlotPoint(base_x,base_y), ImPlotPoint(add_x,add_y)};

				ImPlot::SetNextLineStyle(ColorlineBase2Add, 1.f);
				ImPlot::PlotLine("##base2add", &Base2Add_Line[0].x, &Base2Add_Line[0].y, 2, 0, 0, sizeof(ImPlotPoint));




				if (ImPlot::DragPoint(add_id, &add_x, &add_y, ColorAdditionalPoints, 4, flags)) {

					if (AdditionalPoints_ptrs[i].size() == 1)
					{
						if (i == 0) {
							PlotInfRectangle_Before_Point(ImPlotPoint(base_x, base_y), ImColor(255, 0, 0, 50));

							PlotInfRectangle_After_Point(ImPlotPoint(next_base_x, next_base_y), ImColor(255, 0, 0, 50));

							if (add_x < base_x)
								add_x = base_x;

							if (add_x > next_base_x)
								add_x = next_base_x;
						}
						else {
							PlotInfRectangle_Before_Point(ImPlotPoint(prev_base_x, prev_base_y), ImColor(255, 0, 0, 50));
							PlotInfRectangle_After_Point(ImPlotPoint(base_x, base_y), ImColor(255, 0, 0, 50));

							if (add_x < prev_base_x)
								add_x = prev_base_x;

							if (add_x > base_x)
								add_x = base_x;
						}

					}
					else {
						if (j == 0) {
							PlotInfRectangle_Before_Point(ImPlotPoint(prev_base_x, prev_base_y), ImColor(255, 0, 0, 50));
							PlotInfRectangle_After_Point(ImPlotPoint(base_x, base_y), ImColor(255, 0, 0, 50));

							if (add_x < prev_base_x)
								add_x = prev_base_x;

							if (add_x > base_x)
								add_x = base_x;

							if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
								double delta_x = base_x - add_x;
								double delta_y = base_y - add_y;

								double next_add_x = base_x + delta_x;
								double next_add_y = base_y + delta_y;


								if (next_add_x < base_x)
									next_add_x = base_x;

								if (next_add_x > next_base_x)
									next_add_x = next_base_x;

								AdditionalPoints_ptrs[i][1]->x = next_add_x;
								AdditionalPoints_ptrs[i][1]->y = next_add_y;

							}





						}
						else {
							PlotInfRectangle_Before_Point(ImPlotPoint(base_x, base_y), ImColor(255, 0, 0, 50));
							PlotInfRectangle_After_Point(ImPlotPoint(next_base_x, next_base_y), ImColor(255, 0, 0, 50));

							if (add_x < base_x)
								add_x = base_x;

							if (add_x > next_base_x)
								add_x = next_base_x;


							if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
								double delta_x = base_x - add_x;
								double delta_y = base_y - add_y;

								double prev_add_x = base_x + delta_x;
								double prev_add_y = base_y + delta_y;


								if (prev_add_x < prev_base_x)
									prev_add_x = prev_base_x;

								if (prev_add_x > base_x)
									prev_add_x = base_x;

								AdditionalPoints_ptrs[i][0]->x = prev_add_x;
								AdditionalPoints_ptrs[i][0]->y = prev_add_y;

							}

						}
					}
					


					auto* point = AdditionalPoints_ptrs[i][j];
					point->x = add_x;
					point->y = add_y;

					curveArray->DiscritizeAll();
					curveArray->MakeSamples();



				}

			}


		}

		ImPlot::EndPlot();
	}

}

void CurvesPointsManager::UpdatePoints(CurveBazierArray& curveArray) {
	this->curveArray = &curveArray;

	BasePoints_ptrs.clear();
	AdditionalPoints_ptrs.clear();

	auto curves = curveArray.GetCurves_ptr();

	for (int i = 0; i < curves->size(); i++) {

		if (i != 0)
			BasePoints_ptrs.back().emplace_back(curves->at(i).Get_ptr_LeftBase());
		else
			BasePoints_ptrs.push_back({ curves->at(i).Get_ptr_LeftBase() });

		BasePoints_ptrs.push_back({ curves->at(i).Get_ptr_RightBase() });


		if (i != 0)
			AdditionalPoints_ptrs.back().emplace_back(curves->at(i).Get_ptr_LeftAdditional());
		else
			AdditionalPoints_ptrs.push_back({ curves->at(i).Get_ptr_LeftAdditional() });

		AdditionalPoints_ptrs.push_back({ curves->at(i).Get_ptr_RightAdditional() });
	}





}

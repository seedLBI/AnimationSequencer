#ifndef CURVES_POINTS_MANAGER_H
#define CURVES_POINTS_MANAGER_H

#include <vector>
#include <string>
#include <iostream>

#include "CurveBezier/CurveBezier.h"
#include "CurveBezier/CurveBezierArray.h"

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/implot.h"

class CurvesPointsManager {
public:
	CurvesPointsManager();
	~CurvesPointsManager();

	void Draw(float currentTime);
	void UpdatePoints(CurveBazierArray& curveArray);

private:
	std::vector<std::vector<glm::vec2*>> BasePoints_ptrs;
	std::vector<std::vector<glm::vec2*>> AdditionalPoints_ptrs;


	CurveBazierArray* curveArray;

	void PlotInfRectangle_Before_Point(const ImPlotPoint& pointGraph,const ImU32& rect_color);
	void PlotInfRectangle_After_Point(const ImPlotPoint& pointGraph, const ImU32& rect_color);

};



#endif
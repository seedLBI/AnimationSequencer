#include "CurveBezierArray.h"

CurveBazierArray::CurveBazierArray() {

}

std::vector<glm::vec2> CurveBazierArray::GetSamples() {
	return samples;
}

std::vector<CubicCurveBezier> CurveBazierArray::GetCurves() {
	return curves;
}
std::vector<CubicCurveBezier>* CurveBazierArray::GetCurves_ptr() {
	return &curves;
}

void CurveBazierArray::AddOffset_X(const float& offset) {
	for (auto& curve : curves) {
		curve.AddOffset_X(offset);
	}
}


void CurveBazierArray::SetCurve(CubicCurveBezier& curve, const int& index) {
	if (curves.empty()){
		curves.push_back(curve);
		return;
	}

	if (curves.size() - 1 < index)
	{
		curves.resize(index + 1);

	}
	curves[index] = curve;
}

void CurveBazierArray::AddCurve(CubicCurveBezier& curve) {

	curves.push_back(curve);


	//curves.push_back(curve);
}

void CurveBazierArray::DiscritizeAll() {
	for (auto& curve : curves)
		curve.Discretize();
}


void CurveBazierArray::MakeSamples() {
	samples = curves[0].GetSamples();
	for (int i = 1; i < curves.size(); ++i) {
		auto c_sample = curves[i].GetSamples();
		samples.insert(samples.end(), c_sample.begin() + 1, c_sample.end());
	}
}


void CurveBazierArray::AddPoint(const glm::vec2& point) {

	bool IsOnePoint = false;

	if (curves.size() == 1) {

		if (curves[0].GetLeftBase() != curves[0].GetRightBase() || curves[0].GetLeftAdditional() != curves[0].GetRightAdditional() || curves[0].GetLeftAdditional() != curves[0].GetLeftBase()) {
			IsOnePoint = false;
		}
		else {
			IsOnePoint = true;
		}
	}

	if (IsOnePoint && point.x == curves[0].GetLeftBase().x) {

		curves[0].Get_ptr_LeftBase()->y = point.y;
		curves[0].Get_ptr_RightBase()->y = point.y;
		curves[0].Get_ptr_LeftAdditional()->y = point.y;
		curves[0].Get_ptr_RightAdditional()->y = point.y;

		DiscritizeAll();
		MakeSamples();

		return;
		
	}


	if (curves.size() == 0) {

		curves.push_back(CubicCurveBezier(point, point, point, point));

	}
	else if (point.x < curves.front().GetLeftBase().x) {
		glm::vec2 Left_Curve_LeftBase = point;
		glm::vec2 Left_Curve_LeftAdditional;
		glm::vec2 Left_Curve_RightAdditional;
		glm::vec2 Left_Curve_RightBase = curves.front().GetLeftBase();

		Left_Curve_LeftAdditional  = CubicCurveBezier::Lerp(Left_Curve_LeftBase, Left_Curve_RightBase, 0.25f);
		Left_Curve_RightAdditional = CubicCurveBezier::Lerp(Left_Curve_LeftBase, Left_Curve_RightBase, 0.75f);

		CubicCurveBezier Left(Left_Curve_LeftBase, Left_Curve_LeftAdditional, Left_Curve_RightAdditional, Left_Curve_RightBase);


		if (curves.size() == 1 && curves.front().GetLeftBase().x == curves.front().GetRightBase().x) {
			curves.front() = Left;
		}
		else {
			curves.insert(curves.begin(), Left);
		}

	}
	else if (point.x > curves.back().GetRightBase().x)
	{
		glm::vec2 Right_Curve_LeftBase = curves.back().GetRightBase();
		glm::vec2 Right_Curve_LeftAdditional;
		glm::vec2 Right_Curve_RightAdditional;
		glm::vec2 Right_Curve_RightBase = point;

		Right_Curve_LeftAdditional  = CubicCurveBezier::Lerp(Right_Curve_LeftBase, Right_Curve_RightBase, 0.25f);
		Right_Curve_RightAdditional = CubicCurveBezier::Lerp(Right_Curve_LeftBase, Right_Curve_RightBase, 0.75f);

		CubicCurveBezier Right(Right_Curve_LeftBase, Right_Curve_LeftAdditional, Right_Curve_RightAdditional, Right_Curve_RightBase);


		if (curves.size() == 1 && curves.back().GetLeftBase().x == curves.back().GetRightBase().x) {
			curves.back() = Right;
		}
		else {
			curves.insert(curves.end(), Right);
		}
	}
	else {

		for (int i = 0; i < curves.size(); i++)
		{
			if (point.x == curves[i].GetLeftBase().x){
				
				curves[i].Get_ptr_LeftBase()->y = point.y;
				if (i > 0)
					curves[i-1].Get_ptr_RightBase()->y = point.y;
				break;

			}
			else if (point.x == curves[i].GetRightBase().x){
				curves[i].Get_ptr_RightBase()->y = point.y;
				if (i < curves.size() - 1)
					curves[i + 1].Get_ptr_LeftBase()->y = point.y;
				break;

			}
			else if (point.x > curves[i].GetLeftBase().x && point.x < curves[i].GetRightBase().x) {

				float t = curves[i].GetTfromPositionX(point.x);


				glm::vec2 newpoint = curves[i].GetDotOnCurve(t);

				glm::vec2 BasePosLeft = CubicCurveBezier::Lerp(curves[i].GetLeftBase(), curves[i].GetLeftAdditional(), t);
				glm::vec2 AddPosLeft  = CubicCurveBezier::LerpQuadBezier(curves[i].GetLeftBase(),       curves[i].GetLeftAdditional(),  curves[i].GetRightAdditional(), t);
				glm::vec2 AddPosRight = CubicCurveBezier::LerpQuadBezier(curves[i].GetLeftAdditional(), curves[i].GetRightAdditional(), curves[i].GetRightBase(),       t);
				glm::vec2 BasePosRight = CubicCurveBezier::Lerp(curves[i].GetRightAdditional(), curves[i].GetRightBase(), t);

				glm::vec2 Left_Curve_LeftBase	     = curves[i].GetLeftBase();
				glm::vec2 Left_Curve_LeftAdditional  = BasePosLeft;
				glm::vec2 Left_Curve_RightAdditional = AddPosLeft;
				glm::vec2 Left_Curve_RightBase		 = newpoint;

				CubicCurveBezier Left(Left_Curve_LeftBase, Left_Curve_LeftAdditional, Left_Curve_RightAdditional, Left_Curve_RightBase);

				glm::vec2 Right_Curve_LeftBase          = newpoint;
				glm::vec2 Right_Curve_LeftAdditional    = AddPosRight;
				glm::vec2 Right_Curve_RightAdditional   = BasePosRight;
				glm::vec2 Right_Curve_RightBase			= curves[i].GetRightBase();

				CubicCurveBezier Right(Right_Curve_LeftBase, Right_Curve_LeftAdditional, Right_Curve_RightAdditional, Right_Curve_RightBase);

				curves[i] = Left;

				curves.insert(curves.begin() + i + 1, Right);

				break;
			}

		}

	}

	DiscritizeAll();
	MakeSamples();
}



float CurveBazierArray::GetValueFromTime(const float& time) {
	if (curves.empty() || samples.empty() || OnlyOnePoint) {
		return default_value;
	}

	if (time <= samples.front().x)
		return samples.front().y;

	if (time >= samples.back().x)
		return samples.back().y;

	if (samples.size() == 1) {
		return samples[0].y;
	}

	int left = 0;
	int right = samples.size() - 1;

	while (left < right) {
		int middle = left + (right - left + 1) / 2;

		if (time < samples[middle].x)
			right = middle - 1;
		else
			left = middle;
	}

	if (time == samples[left].x) {
		return samples[left].y;
	}

	float x0 = samples[left].x;
	float y0 = samples[left].y;
	float x1 = samples[left + 1].x;
	float y1 = samples[left + 1].y;

	float t = (time - x0) / (x1 - x0);
	return y0 + t * (y1 - y0);

}


#ifndef CURVE_BEZIER_ARRAY_H
#define CURVE_BEZIER_ARRAY_H

#include "CurveBezier.h"

class CurveBazierArray {
private:
	std::vector<CubicCurveBezier> curves;
	std::vector<glm::vec2> samples;

public:
	float default_value = 0.f;

	CurveBazierArray();

	void DiscritizeAll();
	void MakeSamples();

	std::vector<glm::vec2> GetSamples();

	std::vector<CubicCurveBezier> GetCurves();
	void AddCurve(const CubicCurveBezier& curve);

	float GetValueFromTime(const float& time);
};


struct CurveBazierArray_Vector1 {
	CurveBazierArray x;
};
struct CurveBazierArray_Vector2 {
	CurveBazierArray x, y;
};
struct CurveBazierArray_Vector3 {
	CurveBazierArray x, y, z;
};


#endif

#ifndef CURVE_BEZIER_ARRAY_H
#define CURVE_BEZIER_ARRAY_H

#include "CurveBezier.h"

class CurveBazierArray {
private:
	std::vector<CubicCurveBezier> curves;
	std::vector<glm::vec2> samples;

public:
	float default_value = 0.f;
	bool OnlyOnePoint = true;



	CurveBazierArray();

	void DiscritizeAll();
	void MakeSamples();

	std::vector<glm::vec2> GetSamples();

	std::vector<CubicCurveBezier> GetCurves();
	std::vector<CubicCurveBezier>* GetCurves_ptr();

	void AddOffset_X(const float& offset);


	void AddCurve(CubicCurveBezier& curve);
	void SetCurve(CubicCurveBezier& curve, const int& index);



	void AddPoint(const glm::vec2& point);

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

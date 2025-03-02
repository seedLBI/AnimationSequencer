#ifndef CURVE_BEZIER_H
#define CURVE_BEZIER_H

#include <glm/glm.hpp>
#include <vector>

class CubicCurveBezier {
private:
	float default_value = 0.f;
	glm::vec2 P1{0.f,0.f}, P2{ 0.f,0.f }, P3{ 0.f,0.f }, P4{ 0.f,0.f };
	int samplingFrequency = 24;
	std::vector<glm::vec2> samples;

public:
	CubicCurveBezier();
	CubicCurveBezier(glm::vec2 P1, glm::vec2 P2, glm::vec2 P3, glm::vec2 P4);

	static  glm::vec2 Lerp(const glm::vec2& p1, const glm::vec2& p2, const float& t);
	static  glm::vec2 LerpQuadBezier(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const float& t);

	void AddOffset_X(const float& offset);

	inline glm::vec2 GetDotOnCurve(const float& t);

	float GetTfromPositionX(const float& x);

	std::vector<glm::vec2> GetSamples();

	void  SetDefaultValue(const float& value);
	float GetDefaultValue();


	void Discretize();

	void SetSamplingFrequency(const int& frequency);
	int  GetSamplingFrequency();

	glm::vec2  GetLeftBase();
	glm::vec2* Get_ptr_LeftBase();
	void	   SetLeftBase(const glm::vec2& point);

	glm::vec2  GetLeftAdditional();
	glm::vec2* Get_ptr_LeftAdditional();
	void	   SetLeftAdditional(const glm::vec2& point);

	glm::vec2  GetRightBase();
	glm::vec2* Get_ptr_RightBase();
	void	   SetRightBase(const glm::vec2& point);

	glm::vec2  GetRightAdditional();
	glm::vec2* Get_ptr_RightAdditional();
	void	   SetRightAdditional(const glm::vec2& point);

};



#endif



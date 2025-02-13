#ifndef CURVE_BEZIER_H
#define CURVE_BEZIER_H

#include <glm/glm.hpp>
#include <vector>

class CubicCurveBezier {
private:
	float default_value = 0.f;
	glm::vec2 P1, P2, P3, P4;
	int samplingFrequency = 24;
	std::vector<glm::vec2> samples;

public:
	CubicCurveBezier();
	CubicCurveBezier(glm::vec2 P1, glm::vec2 P2, glm::vec2 P3, glm::vec2 P4);

	inline glm::vec2 Lerp(const glm::vec2& p1, const glm::vec2& p2, const float& t) const;

	inline glm::vec2 GetDotOnCurve(const float& t);

	std::vector<glm::vec2> GetSamples();

	void  SetDefaultValue(const float& value);
	float GetDefaultValue();


	void Discretize();

	void SetSamplingFrequency(const int& frequency);
	int  GetSamplingFrequency();

	glm::vec2 GetLeftBase();
	void	  SetLeftBase(const glm::vec2& point);
	glm::vec2 GetLeftAdditional();
	void	  SetLeftAdditional(const glm::vec2& point);

	glm::vec2 GetRigthBase();
	void	  SetRigthBase(const glm::vec2& point);
	glm::vec2 GetRightAdditional();
	void	  SetRightAdditional(const glm::vec2& point);

};


#endif



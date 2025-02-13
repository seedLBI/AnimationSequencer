#include "CurveBezier.h"

CubicCurveBezier::CubicCurveBezier() {
	default_value = 0.f;
}

CubicCurveBezier::CubicCurveBezier(glm::vec2 P1, glm::vec2 P2, glm::vec2 P3, glm::vec2 P4) {
	default_value = 0.f;
	this->P1 = P1;
	this->P2 = P2;
	this->P3 = P3;
	this->P4 = P4;
}


inline glm::vec2 CubicCurveBezier::Lerp(const glm::vec2& p1, const glm::vec2& p2, const float& t) const {
	return p1 + t * (p2 - p1);
}


inline glm::vec2 CubicCurveBezier::GetDotOnCurve(const float& t) {
	glm::vec2 a1 = Lerp(P1, P2, t);
	glm::vec2 a2 = Lerp(P2, P3, t);
	glm::vec2 a3 = Lerp(P3, P4, t);

	glm::vec2 b1 = Lerp(a1, a2, t);
	glm::vec2 b2 = Lerp(a2, a3, t);

	glm::vec2 c1 = Lerp(b1, b2, t);

	return c1;
}


void CubicCurveBezier::Discretize() {
	samples.clear();
	samples.resize(samplingFrequency);

	const float step = 1.0 / (float)(samplingFrequency - 1);

	for (int i = 0; i < samplingFrequency; ++i)
		samples[i] = GetDotOnCurve(float(i) * step);
}

std::vector<glm::vec2> CubicCurveBezier::GetSamples() {
	return samples;
}

void CubicCurveBezier::SetDefaultValue(const float& value) {
	default_value = value;
}
float CubicCurveBezier::GetDefaultValue() {
	return default_value;
}

void CubicCurveBezier::SetSamplingFrequency(const int& frequency) {
	samplingFrequency = frequency;
}
int  CubicCurveBezier::GetSamplingFrequency() {
	return samplingFrequency;
}

glm::vec2 CubicCurveBezier::GetLeftBase() {
	return P1;
}
glm::vec2 CubicCurveBezier::GetLeftAdditional() {
	return P2;
}
glm::vec2 CubicCurveBezier::GetRightAdditional() {
	return P3;
}
glm::vec2 CubicCurveBezier::GetRigthBase() {
	return P4;
}


void CubicCurveBezier::SetLeftBase(const glm::vec2& point) {
	P1 = point;
}
void CubicCurveBezier::SetLeftAdditional(const glm::vec2& point) {
	P2 = point;
}
void CubicCurveBezier::SetRightAdditional(const glm::vec2& point) {
	P3 = point;
}
void CubicCurveBezier::SetRigthBase(const glm::vec2& point) {
	P4 = point;
}

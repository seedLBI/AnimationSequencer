#include "CurveBezierArray.h"

CurveBazierArray::CurveBazierArray() {

}

std::vector<glm::vec2> CurveBazierArray::GetSamples() {
	return samples;
}

std::vector<CubicCurveBezier> CurveBazierArray::GetCurves() {
	return curves;
}

void CurveBazierArray::AddCurve(const CubicCurveBezier& curve) {
	curves.push_back(curve);
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


float CurveBazierArray::GetValueFromTime(const float& time) {
	if (curves.empty() || samples.empty())
		return default_value;

	static float last_time = 0.0;
	static int last_index = 0;

	int left = 0;
	int middle = 0;
	int right = samples.size() - 1;

	if (time >= last_time)
		left = last_index;


	while (left <= right) {
		middle = (left + right) / 2;

		if (time < samples[middle].x)
			right = middle - 1;
		else if (time > samples[middle].x)
			left = middle + 1;
		else
			break;
	}

	last_index = middle;
	last_time = time;

	if (time > samples[middle].x) {

		if (middle < samples.size()) {
			float length = samples[middle + 1].x - samples[middle].x;
			float cur_length = time - samples[middle].x;

			float temp_lerp_t = cur_length / length;

			float lerp_y = samples[middle].y + temp_lerp_t * (samples[middle + 1].y - samples[middle].y);

			return lerp_y;
		}
		else
			return samples.back().y;
	}
	else if (time < samples[middle].x) {

		if (middle > 0) {
			float length = samples[middle].x - samples[middle - 1].x;
			float cur_length = samples[middle].x - time;

			float temp_lerp_t = cur_length / length;

			float lerp_y = samples[middle - 1].y + temp_lerp_t * (samples[middle].y - samples[middle - 1].y);

			return lerp_y;
		}
		else
			return samples.front().y;
	}
	else
		return samples[middle].y;


}


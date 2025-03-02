#ifndef SYMBOLMSDF_H
#define SYMBOLMSDF_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Font/Font.h"

#include "CurveBezier/CurveBezierArray.h"


struct InstanceSymbolData {
	glm::vec4 UV_Atlas;
	float     outlineThikness;
	glm::vec3 ColorText;
	glm::vec3 ColorOutline;
	glm::mat4 MatrixTransform;
	glm::mat4 Prev_MatrixTransform;
};

struct SymbolMSDF_Data {
public:
	SymbolMSDF_Data(const int& indexObject, const Glyph& glyph);
	~SymbolMSDF_Data();

	InstanceSymbolData GetData(float time);

	Glyph glyph;
	int indexObject;

	CurveBazierArray_Vector2 Position;
	CurveBazierArray_Vector2 Size;
	CurveBazierArray_Vector1 Rotate;

	CurveBazierArray_Vector1 Thikness;
	CurveBazierArray_Vector3 ColorText;
	CurveBazierArray_Vector3 ColorOutline;
private:
	glm::mat4 PrevMatrix;
};



#endif !SYMBOLMSDF_H

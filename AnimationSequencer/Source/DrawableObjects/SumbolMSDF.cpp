#include "SymbolMSDF.h"

InstanceSymbolData SymbolMSDF_Data::GetData(float time) {
	InstanceSymbolData result;

	result.ColorText.r = ColorText.x.GetValueFromTime(time);
	result.ColorText.g = ColorText.y.GetValueFromTime(time);
	result.ColorText.b = ColorText.z.GetValueFromTime(time);

	result.ColorOutline.r = ColorOutline.x.GetValueFromTime(time);
	result.ColorOutline.g = ColorOutline.y.GetValueFromTime(time);
	result.ColorOutline.b = ColorOutline.z.GetValueFromTime(time);

	result.outlineThikness = Thikness.x.GetValueFromTime(time);

	result.UV_Atlas.x = glyph.atlas.left;
	result.UV_Atlas.y = glyph.atlas.top;
	result.UV_Atlas.z = glyph.atlas.right - glyph.atlas.left;
	result.UV_Atlas.w = glyph.atlas.bottom - glyph.atlas.top;

	glm::vec2 AnchorPointLerp(Anchor_Point.x.GetValueFromTime(time), Anchor_Point.y.GetValueFromTime(time));

	glm::vec2 PosLerp(Position.x.GetValueFromTime(time), Position.y.GetValueFromTime(time));
	glm::vec2 SizeLerp(Size.x.GetValueFromTime(time), Size.y.GetValueFromTime(time));

	float angle = Rotate.x.GetValueFromTime(time);

	glm::vec2 plane_size(glyph.plane.left - glyph.plane.right, glyph.plane.top - glyph.plane.bottom);
	glm::vec2 plane_offset(glyph.plane.left, glyph.plane.top);

	glm::mat4 matrix(1.f);

	matrix = glm::scale(matrix, glm::vec3(plane_size.x, plane_size.y, 1.f));
	matrix = glm::translate(matrix, glm::vec3(plane_offset.x, plane_offset.y, -0.0000001f * float(indexObject)));
	
	matrix = glm::translate(matrix, glm::vec3(PosLerp.x, PosLerp.y, 0.f));


	matrix = glm::translate(matrix, glm::vec3(AnchorPointLerp.x, AnchorPointLerp.y, 0.f));


	matrix = glm::rotate(matrix, angle, glm::vec3(0.f, 0.f, 1.f));

	matrix = glm::scale(matrix, glm::vec3(SizeLerp.x, SizeLerp.y, 1.f));



	matrix = glm::translate(matrix, glm::vec3(-AnchorPointLerp.x, -AnchorPointLerp.y, 0.f));


	result.MatrixTransform = matrix;

	result.Prev_MatrixTransform = PrevMatrix;

	PrevMatrix = matrix;

	return result;
}


SymbolMSDF_Data::SymbolMSDF_Data(const int& indexObject, const Glyph& glyph) {
	this->glyph = glyph;
	this->indexObject = indexObject;

	Size.x.default_value = 1.f;
	Size.y.default_value = 1.f;
	Rotate.x.default_value = 0.f;
	Position.x.default_value = 0.f;
	Position.y.default_value = 0.f;
	Anchor_Point.x.default_value = 0.f;
	Anchor_Point.y.default_value = 0.f;

	Thikness.x.default_value = -0.1f;

	ColorText.x.default_value = 1.f;
	ColorText.y.default_value = 1.f;
	ColorText.z.default_value = 1.f;

	ColorOutline.x.default_value = 1.f;
	ColorOutline.y.default_value = 1.f;
	ColorOutline.z.default_value = 1.f;

}
SymbolMSDF_Data::~SymbolMSDF_Data() {

}
#include "RenderableObject.h"



RenderableObject::RenderableObject(const bool& isTransparent) {
	this->isTransparent = isTransparent;
}

void RenderableObject::Draw() {

}

bool RenderableObject::IsTransparent() {
	return isTransparent;
}
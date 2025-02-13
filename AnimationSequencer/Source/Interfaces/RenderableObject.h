#ifndef RENDERABLEJECT_H
#define RENDERABLEJECT_H

class RenderableObject {
public:
	RenderableObject(const bool& isTransparent);
	~RenderableObject() = default;

	virtual void Draw();

	bool IsTransparent();

private:
	bool isTransparent = false;
};




#endif RENDERABLEJECT_H
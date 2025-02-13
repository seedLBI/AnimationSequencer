#ifndef OPENGL_QUAD_H
#define OPENGL_QUAD_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Quad {
public:
	Quad();
	~Quad();

	void Draw();

protected:
	GLuint VAO,VBO;

	void Init();
};





#endif
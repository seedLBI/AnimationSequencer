#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture/Texture.h"


class Framebuffer
{
public:
	Framebuffer();
	Framebuffer(Texture& texture);
	~Framebuffer();

	void AttachTexture(Texture& texture);
	void Bind();
	static void UnBind();

private:
	GLuint ID = 0;
	GLuint renderbufferID = 0;
	Texture texture;

	void Init();
};


#endif
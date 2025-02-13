#include "Framebuffer.h"

Framebuffer::Framebuffer() {
	Init();
}
Framebuffer::Framebuffer(Texture& texture) {
	Init();
	AttachTexture(texture);
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &ID);
}

void Framebuffer::AttachTexture(Texture& texture) {
	Bind();
	this->texture = texture;

	glViewport(0, 0, texture.GetWidth(), texture.GetHeight());

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetID(), 0);


	glGenRenderbuffers(1, &renderbufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, TextureInternalFormat::DEPTH_COMPONENT, texture.GetWidth(), texture.GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbufferID);

	UnBind();
}
void Framebuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}
void Framebuffer::UnBind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Framebuffer::Init() {
	glGenFramebuffers(1, &ID);
}
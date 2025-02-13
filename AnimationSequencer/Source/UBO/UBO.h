#ifndef UBO_H
#define UBO_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shaders/Shader.h"

#include <string>
#include <iostream>

template <typename T>
class UBO {
public:

	UBO(const std::string& name);
	UBO(const char* name);
	UBO();
	~UBO();

	int GetID();
	std::string GetName();
	void SetName(const std::string& name);

	void BindUBO(const GLuint& bindingPoint);
	void BindProgramm(const Shader& programm);

	void Update(const T& data);

private:
	std::string name;
	GLuint ID;
	GLuint bindingPoint;

	void Constructor(const std::string& name);

};



#endif
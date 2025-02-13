#ifndef DATA_UBO_H
#define DATA_UBO_H

#include <glm/glm.hpp>

struct DataBlockShader {
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec4 times; // 0 - Time
					 // 1 - deltaTime
					 // 2 - Resolution.x
					 // 3 - Resolution.y
};

#endif
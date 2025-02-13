#version 330 core

layout (location = 0)  in vec3  attrib_Position;
layout (location = 1)  in vec2  attrib_TexCoord;
layout (location = 2)  in vec4  attrib_instance_AtlasUV;
layout (location = 3)  in float attrib_instance_ThiknessOutline;
layout (location = 4)  in vec3  attrib_instance_ColorText;
layout (location = 5)  in vec3  attrib_instance_ColorOutline;
layout (location = 6)  in mat4  attrib_instance_MatrixTransform;
layout (location = 10) in mat4  attrib_instance_PrevMatrixTransform;


layout(std140) uniform DataBlock {
    mat4  view;
    mat4  projection;
    float time;
    float deltaTime;
    vec2  Resolution;
};

out vec2  TexCoord;
out float Thikness;
out vec3  ColorText;
out vec3  ColorOutline;

void main() {
	ColorText	 = attrib_instance_ColorText;
	ColorOutline = attrib_instance_ColorOutline;
	Thikness	 = attrib_instance_ThiknessOutline;

	TexCoord = vec2(attrib_instance_AtlasUV.x + attrib_TexCoord.x * attrib_instance_AtlasUV.z,
					attrib_instance_AtlasUV.y + attrib_TexCoord.y * attrib_instance_AtlasUV.w);

	//---------attrib_instance_AtlasUV---------:
	// x - x pos
	// y - y pos
	// z - width size
	// w - height size


	gl_Position = projection * view * attrib_instance_MatrixTransform * vec4(attrib_Position, 1.f);
}
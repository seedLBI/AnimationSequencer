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


out vec4 gFragPrevPos;
out vec4 gFragCurrPos;

void main(){
    gFragCurrPos = projection * view * attrib_instance_MatrixTransform     * vec4(1.1 * attrib_Position, 1.0);
    gFragPrevPos = projection * view * attrib_instance_PrevMatrixTransform * vec4(1.1 * attrib_Position, 1.0);
    gl_Position = gFragCurrPos;
}
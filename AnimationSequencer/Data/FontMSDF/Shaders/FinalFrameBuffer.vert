#version 330 core

layout (location = 0)  in vec3  attrib_Position;
layout (location = 1)  in vec2  attrib_TexCoord;

layout(std140) uniform DataBlock {
    mat4  view;
    mat4  projection;
    float time;
    float deltaTime;
    vec2  Resolution;
};

out vec2 TexCoord;

void main(){
    TexCoord = attrib_TexCoord;
    gl_Position = vec4(attrib_Position,1.0);
}
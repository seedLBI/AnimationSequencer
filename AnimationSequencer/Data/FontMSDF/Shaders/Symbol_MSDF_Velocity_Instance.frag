#version 330 core

in vec4 gFragPrevPos;
in vec4 gFragCurrPos;

out vec4 FragColor;

layout(std140) uniform DataBlock {
    mat4  view;
    mat4  projection;
    float time;
    float deltaTime;
    vec2  Resolution;
};

void main(){
	vec2 a = (gFragCurrPos.xy / gFragCurrPos.w) * 0.5 + 0.5;
	vec2 b = (gFragPrevPos.xy / gFragPrevPos.w) * 0.5 + 0.5;
    vec2 newVelocity = a - b;

    FragColor = vec4(newVelocity, 0.f, 0.5f);
}
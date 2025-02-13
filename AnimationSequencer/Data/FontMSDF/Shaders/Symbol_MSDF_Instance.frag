#version 330 core

out vec4 FragColor;

in vec2  TexCoord;
in float Thikness;
in vec3  ColorText;
in vec3  ColorOutline;

layout(std140) uniform DataBlock {
    mat4 view;
    mat4 projection;
    float time;
    float deltaTime;
    vec2 Resolution;
};

uniform float pxRange = 12.6;
uniform float smoothness;

uniform sampler2D Texture_MSDF_Atlas;


float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}


void main(){
	vec3 sample = texture(Texture_MSDF_Atlas, TexCoord).rgb;

	float sigDist = (median(sample.r, sample.g, sample.b) - 0.5) * pxRange;

	float textAlpha = 1.0 - smoothstep(smoothness, -smoothness, sigDist);

	float outlineAlpha = 1.0 - smoothstep(smoothness, -smoothness, sigDist - Thikness) - textAlpha;

	vec3 finalColor = ColorText.rgb * textAlpha + ColorOutline.rgb * outlineAlpha;

    float finalAlpha = clamp(textAlpha + outlineAlpha, 0.0, 1.0);

	FragColor = vec4(finalColor, finalAlpha);
}
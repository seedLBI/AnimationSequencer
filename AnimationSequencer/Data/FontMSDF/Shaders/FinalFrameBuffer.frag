#version 330 core

in vec2  TexCoord;
out vec4 FragColor;

layout(std140) uniform DataBlock {
    mat4  view;
    mat4  projection;
    float time;
    float deltaTime;
    vec2  Resolution;
};

uniform float targetFPS;

uniform sampler2D texture_color;
uniform sampler2D texture_velocity;

uniform int MAX_SAMPLES = 32;

void main(){
    vec2 texelSize = 1.0 / Resolution.xy;

    vec2 velocity = texture(texture_velocity, TexCoord).rg;
    //velocity = vec2(pow(velocity.x, 1.0 / 3.0),pow(velocity.y, 1.0 / 3.0) );
    float currentFPS = (1.0/deltaTime);
    float uVelocityScale = currentFPS / targetFPS;
    velocity = velocity * uVelocityScale;


    float speed = length(velocity / texelSize);
    float nSamples = clamp(int(speed), 1, MAX_SAMPLES);

    vec4 oResult = texture(texture_color, TexCoord);

    for (int i = 1; i < nSamples; ++i) {
      vec2 offset = velocity * (float(i) / float(nSamples - 1) - 0.5);
      oResult += texture(texture_color, TexCoord + offset);
    }

    FragColor = oResult / float(nSamples) ;

}
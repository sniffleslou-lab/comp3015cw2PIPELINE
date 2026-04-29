#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;


out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

//noise uniforms 
uniform float uTime;
uniform float uNoiseStrength;

//harsh based noise

float harsh(vec2 p){
    return fract(sin(dot(p,vec2(127.1, 311.7))) * 43758.543123);
}

float noise(vec2 p){
    vec2 i = floor(p);
    vec2 f = fract(p);

    float a = harsh(i);
    float b = harsh(i + vec2(1.0, 0.0));
    float c = harsh(i + vec2(0.0, 1.0));
    float d = harsh(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) + 
              (c - a) * u.y *
              (1.0 - u.x) +
              (d - b) * u.x * u.y;
}

void main() {
    vec3 pos = VertexPosition;

    //only deform the plane
   if (pos.y < -0.44) {
        //noise ripples
        float n = noise(pos.xz * 0.5 + uTime * 0.2);
        float noiseWave = n * uNoiseStrength;

        //smooth sine wave
        float sineWave = sin(pos.x * 2.0 + uTime * 1.5) * 0.05;

        float sineWave2 = sin(pos.z * 3.0 + uTime * 1.2) * 0.03;

        pos.y += uNoiseStrength + sineWave + sineWave2;
    }




    Normal = normalize(NormalMatrix * VertexNormal);
    Position = (ModelViewMatrix * vec4(pos, 1.0)).xyz;
    TexCoord = VertexTexCoord;
    gl_Position = MVP * vec4(pos, 1.0);
}

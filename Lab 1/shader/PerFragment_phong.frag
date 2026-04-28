#version 460

in vec3 Position;
in vec3 Normal;

out vec4 FragColor;

// Single light (per-fragment)
uniform vec3 LightPosition;   // in eye space
uniform vec3 LightLa;         // ambient intensity
uniform vec3 LightLd;         // diffuse intensity
uniform vec3 LightLs;         // specular intensity

// Material
uniform vec3 Ka;              // ambient reflectivity
uniform vec3 Kd;              // diffuse reflectivity
uniform vec3 Ks;              // specular reflectivity
uniform float Shininess;

///for textures
uniform int UseTexture;
uniform sampler2D Tex;
in vec2 TexCoord;

//fog
uniform vec3 FogColor;
uniform float FogStart;
uniform float FogEnd;

vec3 phongModel(vec3 position, vec3 n)
{
    vec3 s = normalize(LightPosition - position);
    vec3 v = normalize(-position);
    vec3 r = reflect(-s, n);

    // Ambient
    vec3 ambient = LightLa * Ka;

    // Diffuse
    float sDotN = max(dot(s, n), 0.0);
    vec3 diffuse = LightLd * Kd * sDotN;

    // Specular
    vec3 specular = vec3(0.0);
    if (sDotN > 0.0) {
        specular = LightLs * Ks * pow(max(dot(r, v), 0.0), Shininess);
    }

    return ambient + diffuse + specular;
}

void main()
{
    vec3 n = normalize(Normal);
    vec3 texColor = texture(Tex, TexCoord).rgb;

    vec3 lighting = phongModel(Position, n);

    vec3 baseColor = (UseTexture == 1) ? texColor : Kd;

    //toon shading 
    float levels = 4.0;
    vec3 toonLighting = floor(lighting * levels) / levels;
    vec3 litColor = baseColor * toonLighting;

    //fog
    float dist = length(Position);
    float fogFactor = clamp((dist - FogStart) / (FogEnd - FogStart), 0.0, 1.0);

    //FragColor = vec4(baseColor * lighting, 1.0);

    vec3 finalColor = mix(litColor, FogColor, fogFactor);
    FragColor = vec4(pow(finalColor, vec3(1.0/2.2)), 1.0); //gamma
}

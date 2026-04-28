#version 460


layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 Colour;   
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform mat3 NormalMatrix;

// ---- Light + Material structs from the lab ----
uniform struct LightInfo {
    vec4 Position;   // Light position in eye coords
    vec3 La;         // Ambient light intensity
    vec3 L;          // Diffuse + specular light intensity
} lights[3];

uniform struct MaterialInfo {
    vec3 Ka;         // Ambient reflectivity
    vec3 Kd;         // Diffuse reflectivity
    vec3 Ks;         // Specular reflectivity
    float Shininess; // Specular shininess factor
} Material;

//Phong model function from the lab 
vec3 phongModel(int light, vec3 position, vec3 n)
{
    // Ambient
    vec3 ambient = lights[light].La * Material.Ka;

    // Diffuse
    vec3 s = normalize(vec3(lights[light].Position) - position);
    float sDotN = max(dot(s, n), 0.0);
    vec3 diffuse = Material.Kd * sDotN;

    // Specular
    vec3 spec = vec3(0.0);
    if (sDotN > 0.0) {
        vec3 v = normalize(-position.xyz);
        vec3 r = reflect(-s, n);
        spec = Material.Ks * pow(max(dot(r, v), 0.0), Material.Shininess);
    }

    return ambient + lights[light].L * (diffuse + spec);
}

void main()
{
    // Convert to eye space
    vec3 n = normalize(NormalMatrix * VertexNormal);
    vec4 camCoords4 = ModelViewMatrix * vec4(VertexPosition, 1.0);
    vec3 camCoords = camCoords4.xyz;

    // Accumulate contributions from 3 lights
    Colour = vec3(0.0);
    for (int i = 0; i < 3; i++) {
        Colour += phongModel(i, camCoords, n);
    }

    // Final position
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}

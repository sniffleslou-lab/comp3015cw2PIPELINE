#version 330 core

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube SkyBoxTex;

void main(){
	vec4 texColor = texture(SkyBoxTex, TexCoords);
	FragColor = texColor;
}